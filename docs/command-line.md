# Command line

```
quantum-script [options] script.js|script.qsbc
```

| Option | Effect |
|--------|--------|
| `script.js` | compile and run the script (the first argument that is not an option) |
| `script.qsbc` | run a bytecode file; detected by its content (`QSBC` magic), not by the extension |
| `--run="code"` | run `code` instead of a file |
| `--cmd` | run the script, skipping its first 2 lines (for `.cmd` launchers, below) |
| `--execution-time` | print `Execution time: N ms` at the end |
| `--execution-time-cmd` | `--execution-time` + `--cmd` |
| `--compile=out.qsbc` | compile the script to bytecode, do not run it |
| `--strip` | with `--compile`: leave out source file names, line numbers and function source |
| `--add=file` | with `--compile`: add `file` as a unit included by the name `file` (repeatable) |
| `--add=name=file` | with `--compile`: add `file` as a unit included by `name` |
| `--verify=file.qsbc` | load and check a bytecode file, print `Ok: ...`, do not run it |
| `--compile-cpp=out.cpp` | translate the script or bytecode file to C++, do not run it; `--strip` / `--add` apply to a script |
| `--cpp-no-inline` | with `--compile-cpp`: every value is a `Variable`, every instruction calls the native runtime |
| `--version` | print the version |
| `--license` | print the license |
| `--usage` | print the usage (also printed when no script is given) |
| `@file` | read more arguments from `file` (split like a command line, quotes allowed) |

Unknown `--options` are ignored. The arguments after the script name are not
parsed by the interpreter; scripts read the whole command line with the
`Application` extension (`quantum-script--application`:
`Application.getCmdN()`, `getCmdS(k)`, `getArgument(...)`, `hasFlag(...)`).

## Exit code

| Situation | Exit code |
|-----------|-----------|
| The script ended normally | `0`, or the value given to `Script.setExitCode(n)` |
| `Script.exit(n)` | `n` (stops the script at once) |
| `Script.exit()` / `Script.setExitCode()` with no value | `0` |
| A value that is not a number (`NaN`, `Infinity`) | `1` |
| Uncaught exception or compile error | `1`, message and stack trace on stdout |
| `--compile` / `--verify` / `--compile-cpp` failed | `1` |

Errors are printed to **stdout**, not stderr:

```
Error: Compile error in app.js line 7
```

```
Error: "undefinedFunction" is not a function
- file app.js line 3
- file app.js line 10
```

## Include path

Before the script runs, the interpreter adds two folders to the include path
(used by `Script.include` and by external extension loading): the folder of
the interpreter executable and the folder of the script. The script folder is
searched first. See [Modules and extensions](modules.md).

## Scripts as Windows commands

With `--cmd` the first two lines of the file are skipped, so a `.cmd` file
can launch itself:

```bat
@quantum-script --cmd "%~f0" %*
@exit /b %ERRORLEVEL%
Script.requireExtension("Console");
Console.writeLn("Hello from a .cmd file");
Script.exit(0);
```

Line 1 runs the interpreter on the file itself; line 2 returns its exit code
to the caller (`%ERRORLEVEL%` is expanded after line 1 ran). Line numbers in
error messages count from the top of the file, the skipped lines included.

## Bytecode

```bash
quantum-script --compile=app.qsbc app.js            # compile
quantum-script --compile=app.qsbc --strip app.js    # no file names / lines / function source
quantum-script --compile=app.qsbc --add=lib.js app.js   # bundle lib.js into the file
quantum-script --verify=app.qsbc                    # Ok: app.qsbc - 18 instructions, 1 units
quantum-script app.qsbc                             # run
```

A bundled file is used by `Script.include("lib.js")` when no `lib.js` exists
on disk or in the include path. See [Bytecode](bytecode.md).

## C++

```bash
quantum-script --compile-cpp=app.cpp app.js         # or app.qsbc; build app.cpp with the native runtime and the engine
```

See [Compile to C++](compile-cpp.md).

## Examples

```bash
quantum-script --run="Script.requireExtension('Console'); Console.writeLn(6 * 7);"
quantum-script --execution-time benchmark.js
quantum-script @arguments.txt
```
