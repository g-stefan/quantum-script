# Quantum Script — Documentation

Quantum Script is a **small, embeddable, prototype-based script engine for
C++**, with a JavaScript-like language. It is the scripting layer of the XYO
C++ stack: the `quantum-script` command line interpreter runs `.js` files, and
the library lets any XYO C++ program run scripts, expose native functions to
them and call script functions back. The build tool
[fabricare](https://github.com/g-stefan/fabricare) is itself a Quantum Script
host: every `fabricare/*.js` build script is a Quantum Script program.

What you get:

- **A JavaScript-like language.** `var`, functions, closures, objects and
  arrays literals, prototypes and `new`, `this`, `arguments`, `for` /
  `for in` / `for of`, `while`, `do`, `switch`, `try` / `catch` / `finally`,
  `throw`. Values are `undefined`, `null`, Boolean, Number (`double`), String
  (UTF-8), Array, Object, Function, `AssociativeArray` and native `Resource`
  handles. See [Language](language.md) — it is close to JavaScript, but **not
  the same**: read the differences table before writing scripts.
- **Coroutines with `yield`.** Any function that uses `yield` keeps its state
  between calls: each call resumes after the last `yield`.
- **A compiler, an optimizer and a virtual machine.** Source is compiled to
  instructions for a stack based VM, with tail calls and closures.
- **Bytecode.** Compile scripts to `.qsbc` files, optionally stripped of
  source names, optionally bundling the included files; run them like scripts.
  See [Bytecode](bytecode.md).
- **C++ translation.** `--compile-cpp` turns a script or a `.qsbc` file into
  a C++ program: every script function is a native function of the engine,
  run by the native runtime library, no bytecode in the program.
  See [Compile to C++](compile-cpp.md).
- **Extensions.** Native modules loaded on demand by
  `Script.requireExtension("Name")`: built into the host application
  (internal) or loaded from `quantum-script--name.dll` / `.so` (external).
  This repository ships `Console`; about 30 more (File, Shell, JSON, Math,
  HTTP, Socket, Thread, ...) live in their own `quantum-script--*`
  repositories. See [Modules and extensions](modules.md).
- **Embedding API.** `ExecutiveX` runs a script in a few lines of C++;
  `Executive::setFunction*` exposes C++ functions; `functionApply` calls
  script functions from C++. See [Embedding](embedding.md) and
  [Writing extensions](writing-extensions.md).

```
applications: fabricare, magnet, quantum-script, ...
quantum-script--* extensions (File, Shell, JSON, Thread, HTTP, ...)
quantum-script        <-- this repository: engine library, Console, interpreter
xyo-system            (Shell, File, ProcessInteractive, DateTime)
xyo-encoding          (String, UTF-8)
xyo-multithreading
xyo-data-structures   (TDynamicArray, TRedBlackTree, DynamicObject)
xyo-managed-memory    (Object, TPointer, TPointerX, memory pools)
xyo-platform
```

## Why it exists

The XYO C++ tools need a scripting language for build scripts, automation and
configurable applications, with requirements that a borrowed engine does not
meet well:

| Need | What Quantum Script does |
|------|--------------------------|
| Same types as the C++ code | values are XYO objects (`String`, `TPointer`, `TDynamicArray`), memory is managed by `xyo-managed-memory` |
| Small and self-contained | one amalgam compile unit, depends only on `xyo-system`; static or DLL build |
| Native code next to scripts | a native function is one C++ function with a fixed signature, registered by name (`"Console.writeLn(str)"`) |
| Load only what a script uses | extensions are loaded by `Script.requireExtension`, from the host or from a DLL |
| Scripts as commands | a `.cmd` file can be a script (`--cmd` skips the launcher lines) |
| Ship without sources | `--compile` to `.qsbc`, `--strip` removes source names and lines, `--add` bundles included files |
| Generators without callbacks | `yield` in any function makes it a resumable coroutine |

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Run a script | `quantum-script script.js` | exit code = `Script.exit(n)` / `Script.setExitCode(n)`, 1 on uncaught error |
| Run a line of code | `quantum-script --run="code"` | |
| Print | `Script.requireExtension("Console"); Console.writeLn(x);` | no `print` in the core |
| Include another file | `Script.include("file.js")` / `Script.includeOnce(...)` | include path, then next to the including file |
| Load an extension | `Script.requireExtension("JSON")` | internal first is `requireInternalExtension`; default tries external first |
| Type test | `Script.isString(x)`, `Script.isArray(x)`, `typeof(x)` | `typeof` needs parentheses, returns `"Number"`, `"String"`, ... |
| Convert | `Convert.toNumber(x)`, `Convert.toString(x)`, `Convert.numberFormat(x, 2)` | |
| Map with any key | `new AssociativeArray()` | keeps insertion order |
| Compile to bytecode | `quantum-script --compile=app.qsbc app.js` | `--strip`, `--add=file` |
| Compile to C++ | `quantum-script --compile-cpp=app.cpp app.js` | build `app.cpp` with `quantum-script.native-runtime` and the engine |
| Run a script from C++ | `ExecutiveX::initExecutive(...)` + `ExecutiveX::executeFile(...)` | one executive per thread |
| C++ function for scripts | `executive->setFunction2("Host.add(a,b)", hostAdd)` | in the init callback |
| Script function from C++ | `fn->functionApply(this_, arguments)` | throws `Error` if the script throws |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, first script, first C++ host, depending on the library, configuration |
| [Command line](command-line.md) | `quantum-script` options, `@file` arguments, `.cmd` scripts, exit codes, bytecode commands |
| [Language](language.md) | Syntax, values, operators, functions and closures, prototypes, coroutines, exceptions, **differences from JavaScript** |
| [Standard library](standard-library.md) | `Script`, `Convert`, `String`, `Array`, `Function`, `Object`, `Error`, `AssociativeArray`, `Console` |
| [Modules and extensions](modules.md) | `Script.include*`, the include path, virtual sources, extension loading, the extension catalog |
| [Embedding](embedding.md) | `ExecutiveX`, the init callback, native functions, the `Variable` API, calling scripts from C++, errors, threads |
| [Writing extensions](writing-extensions.md) | Internal and DLL extensions, the `quantumScriptExtension` entry point, resources, native coroutines |
| [Bytecode](bytecode.md) | `.qsbc` files, strip, bundles, compatibility rules, file format, C++ API |
| [Compile to C++](compile-cpp.md) | `--compile-cpp`, compiled functions on the native runtime, what is compiled, differences, compatibility |
| [API reference](reference.md) | Every public C++ symbol and every script global on one page |

## Source map

```
source/XYO/QuantumScript.hpp                   umbrella header of the engine, include this
source/XYO/QuantumScript.Amalgam.cpp           the engine in one translation unit
source/XYO/QuantumScript/
    Dependency.hpp                             xyo-system, export macro, defaults
    Config.hpp, Config.Template.hpp            debug / optimizer switches
    ExecutiveX[.cpp]                           simple host API: init, executeFile, executeString, errors
    Executive[.cpp]                            the engine: compile, include, extensions, setFunction*, callVmFunction
    Context[.cpp]                              per thread symbols, global object, built-in prototypes
    Variable*.hpp/.cpp                         value types (Null, Boolean, Number, String, Array, Object,
                                               AssociativeArray, Function, VmFunction, Resource, StackTrace, ...)
    Token, Input, Parser*                      tokenizer and compiler (one Parser*.cpp per statement)
    Asm, AsmOpcode, ParserAsm.hpp              assembler, optimizer, stable opcode numbers
    CodeUnit, Linker                           relocatable code and the linker
    Bytecode[.cpp]                             .qsbc save / load / verify
    Instruction*, InstructionContext           virtual machine
    LibStd*.cpp / LibStd*.js                   standard library (native part / script part)
    LibStd*.Source.cpp                         generated from the .js by fabricare/make.prepare.js
source/XYO/QuantumScript.Extension/Console/    the Console extension (internal or quantum-script--console.dll)
source/XYO/QuantumScript.NativeRuntime.hpp     the native runtime, for code compiled to C++ (quantum-script.native-runtime)
source/XYO/QuantumScript.NativeRuntime/
    Module, Instruction, Exception             module constants, the instructions for compiled code, script exceptions
    Coroutine, Run                             state of yield functions, load / execute / run
source/XYO/QuantumScript.Application/          the quantum-script command line interpreter
    CompileCpp[.cpp]                           bytecode to C++ for the native runtime (--compile-cpp)
test/test.NNNN.js                              language and library tests, run by fabricare/test.js
fabricare/make.prepare.js                      embeds LibStd*.js into C++ sources before make
fabricare/test.js                              runs every test as source, as bytecode, stripped, as a bundle, and as C++
```

## AI assistant skill

A Claude Code skill describing how to use Quantum Script lives in
[`.claude/skills/quantum-script/`](../.claude/skills/quantum-script/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that write Quantum
Script code (fabricare scripts, extensions, hosts).
