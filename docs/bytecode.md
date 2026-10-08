# Bytecode

A script can be compiled once to a **`.qsbc` bytecode file** and run later
without its source: faster start for large scripts, no source to ship, and
several files in one (a *bundle*).

```bash
quantum-script --compile=app.qsbc app.js                 # compile, keep file names and lines
quantum-script --compile=app.qsbc --strip app.js         # without names, lines, function source
quantum-script --compile=app.qsbc --add=lib.js app.js    # bundle lib.js
quantum-script --compile=app.qsbc --add=util=src/util.js app.js   # bundled under the name "util"
quantum-script --verify=app.qsbc                         # Ok: app.qsbc - 18 instructions, 1 units
quantum-script app.qsbc                                  # run
```

A bytecode file is recognized by its first four bytes (`QSBC`), not by the
extension: `quantum-script file`, `Script.include(file)`,
`ExecutiveX::executeFile(file)` all accept it. `--cmd` (skip 2 lines) also
applies to `--compile`.

## What is in a file

- The compiled instructions of the main script, with jump targets as
  relocatable labels (a `CodeUnit`), linked when loaded.
- A string table: property names and string constants.
- Optionally (default on, off with `--strip`) the source file name and line
  of every instruction, for stack traces, and the source text of functions.
  A stripped file still runs the same; stack traces show no file lines, and
  converting a function to a string throws.
- Optionally, named **units** (`--add`): other compiled files, included by
  name.

The code is the compiled form of **this file only**: `Script.include` calls
still run at run time and look for their files as usual, unless the file is
bundled.

## Bundles

`--add=name=file` compiles `file` and stores it as a unit named `name`
(`--add=file` uses `file` as the name). When the bundle is loaded, its units
are registered in the engine; `Script.include(name)` /
`Script.includeOnce(name)` use a unit when no real file with that name is
found (real files and virtual sources come first, see
[Modules and extensions](modules.md)). So:

```bash
quantum-script --compile=dist/app.qsbc --add=lib/util.js app.js
```

makes `Script.include("lib/util.js")` in `app.js` work from `dist/` without
`lib/util.js` on disk. The name must match the string passed to `include`.
A unit can not be a bundle itself.

## Compatibility

| Rule | Consequence |
|------|-------------|
| The header holds the format version (now `1`) and the engine version (`major.minor.patch.build`) | |
| A file loads only if its **format version** equals the engine's | `Unsupported bytecode format N` |
| ... and its **engine major version** equals the engine's | `Bytecode is for engine version 7.x..., this engine is 8.x...` |
| Opcode numbers are fixed within a major version; new opcodes are only appended | a file from 7.0 runs on any 7.x |
| Every opcode, operand, string index and jump target is checked when loading | a damaged file is rejected, never executed |
| An unknown section type is rejected | a format change increases the format version |

Errors when running: `Error: Invalid bytecode in app.qsbc: <reason>`, exit
code 1. `--verify` prints `Error: <reason> - app.qsbc`.

Recompile the `.qsbc` files when the engine's major version changes.

## File format

All numbers little endian.

```
header (20 bytes)
  u8[4]  magic "QSBC"
  u16    format version (1)
  u16    engine version major, minor, patch
  u32    engine build
  u32    section count
section directory, section count entries of
  u32    type
  u32    offset from the start of the file
  u32    size
sections
  1 Strings  u32 count, count x (u32 length, bytes): symbol names and string values
  2 Code     u32 count, count x (u16 opcode, operands as the opcode defines)
             labels are u32 instruction numbers, 1 is the first instruction, 0 is no label
  3 Source   u32 run count, runs x (u32 instruction count, u32 string + 1 or 0, u32 line)
             consecutive instructions with the same source and line; optional (--strip)
  4 Units    u32 count, count x (u32 name length, name, u32 size, bytecode image)
             named units of a bundle; an image can not be a bundle
```

The opcode table is `source/XYO/QuantumScript/ParserAsm.hpp` (numbers) and
`AsmOpcode.cpp` (names, procedures, operand kinds). Native instructions that
exist only at run time (calls into C++ set up by `setFunction`) can not be
saved: compile script code, not host init code.

## C++ API

```cpp
#include <XYO/QuantumScript.hpp>

Executive &executive = ExecutiveX::getExecutive();   // after ExecutiveX::initExecutive

// compile without running
CodeUnit unit;
if (executive.compileFileToUnit("app.js", unit) != VmParserError::None) {
	// executive.errorInfo.compileFileName, compileLineNumber
};

// save, optionally with named units
CodeUnitMap units;
TPointer<CodeUnit> util;
util.newMemory();
executive.compileFileToUnit("lib/util.js", *util);
units.set("lib/util.js", util);

String error;
if (!Bytecode::saveFile(unit, "app.qsbc", error, true /* withSource */, &units)) {
	printf("%s\n", error.value());
};

// load and check
CodeUnit loaded;
CodeUnitMap loadedUnits;
if (!Bytecode::loadFile("app.qsbc", loaded, error, &loadedUnits)) {
	printf("%s\n", error.value());
};
```

| Symbol | Use |
|--------|-----|
| `Executive::compileFileToUnit(file, unit, skipLines)` | compile a file to a `CodeUnit`, not linked, not run |
| `Executive::compileStringToUnit(source, unit, tag)` | the same from a string |
| `Executive::includeAndExecuteUnit(context, unit)` | link and run a unit as included code |
| `Executive::setIncludeUnit(name, unit)` / `removeIncludeUnit` / `hasIncludeUnit` | register a unit for `Script.include(name)` |
| `Executive::newFunctionFromUnit(unit)` | a script function whose body is the unit |
| `Bytecode::save(unit, Buffer &, error, withSource, units)` / `load(data, size, unit, error, units)` | in memory |
| `Bytecode::saveFile` / `loadFile` | to / from a file |
| `Bytecode::isBytecode(data, size)` / `isBytecodeFile(name)` | test the magic |
| `Bytecode::formatVersion` | the format version written by this engine |
| `Linker::unlinkFunction(fn, unit, error)` | the code of a script function as a unit (to move it to another thread); no native instructions, no outside jumps, no variables of enclosing functions |

Embedding precompiled scripts in an executable: store the `.qsbc` bytes (for
example with `file-to-cs`), `Bytecode::load` them at start and register them
with `setIncludeUnit("name.js", unit)`; scripts then include them by name.
