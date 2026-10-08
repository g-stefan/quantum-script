# Compile to C++

`--compile-cpp` translates a script or a `.qsbc` bytecode file to a **C++
source file** for the **native runtime** (`quantum-script.native-runtime`).
Each script function, and the top level code, becomes a native function of
the engine: the program has no bytecode and no source, and nothing is added
to the instruction list of the interpreter. Compiled and linked with the
engine, it does what `quantum-script app.qsbc` does.

```bash
quantum-script --compile-cpp=app.cpp app.js                        # from source
quantum-script --compile-cpp=app.cpp app.qsbc                      # from bytecode
quantum-script --compile-cpp=app.cpp --strip --add=lib.js app.js   # --strip / --add as for --compile
quantum-script --compile-cpp=app.cpp --cpp-no-inline app.js        # every instruction calls the native runtime
```

The generated file includes `main` (as the interpreter: the `Console`
extension is internal, errors and stack traces on stdout, exit code from
`Script.exit`). Build it like any program that embeds the engine, with the
`quantum-script.native-runtime`, `quantum-script--console` and
`quantum-script` libraries, for example with fabricare:

```json
{
	"solution" : {
		"name" : "app",
		"type" : "xyo-cpp",
		"projects" : [
			{
				"name" : "app",
				"make" : "exe",
				"sourcePath" : "app",
				"dependency" : [
					"quantum-script.native-runtime",
					"quantum-script--console",
					"quantum-script"
				]
			}
		]
	}
}
```

with the generated file in `source/app/app.cpp`.

## How it works

The engine still does the values, objects, prototypes, the standard library,
extensions and the code that is not compiled. The compiled code is native
functions on top of it, and the **native runtime** library has the
instructions of the virtual machine as C++ functions for them:

- **Each script function is a C++ function** with the signature of a native
  function (`FunctionProcedure`), created as a `VariableFunction` when the
  script creates the function; a function with `yield` is a
  `VariableFunctionWithYield`. Calls between compiled functions are C++
  calls, without the interpreter.
- The **operand stack** and the **local variables** are C++ locals. Local
  variables used by inner functions (closures) are kept in an array, as the
  interpreter does, and inner functions find them through
  `VariableFunction::functionParent`.
- **Jumps, loops, `switch`, `break`, `continue`, `try` / `catch` /
  `finally`** are resolved when generating: the contexts of the interpreter
  are known for each instruction, the code is C++ `goto`s.
- A **thrown value is a C++ exception** (`NativeRuntime::Exception`, an
  `Error` with the value), an error of a native function (`throw Error(...)`)
  is caught the same way. Stack traces have the file and the line of each
  compiled function, as the interpreter (not with `--strip`).
- **Coroutines**: on `yield`, the state of the function (C++ locals, the
  position) is saved in the function value, the next call restores it and
  continues after the `yield`.
- `return f(...)` where `f` is the running function starts the C++ function
  again (a tail call without stack). Other recursion uses the C++ stack of
  the thread that runs the program: with the default stack of 1 MB on
  Windows, about 1500 levels of a small function; deeper, the process ends
  with a stack overflow.
- The units added with `--add` are native functions too, set as **include
  functions** of the engine: `Script.include(name)` calls them.
- `Script.exit` stops the compiled code (a C++ exception that script code
  can not catch).
- Simple instructions are **C++ code**: constants, variables, properties,
  arithmetic, comparisons, logical operators, assignments, `++` / `--`.
  Intermediate values of an expression are C++ locals (`double`, `bool`), a
  property that is only assigned is set directly, and `+` / comparisons on
  two numbers do not call the generic operators.

`--cpp-no-inline` keeps the data of the interpreter: every value is a
`Variable`, every instruction is a call of the native runtime, local
variables are in an array; a reference to compare with when looking for a
problem in the generated code.

## What is compiled

| Code | |
|------|---|
| the script given, and the units added with `--add` | compiled |
| `Script.include` of a file that is not a unit | read and run at run time, interpreted |
| `Script.execute`, `Script.getFunctionFromString` | interpreted |
| the standard library, extensions written in script | interpreted |

Compiled and interpreted code call each other: a compiled function is a
function value like any other.

## Differences from the interpreter

| | Interpreter | Compiled |
|-|-------------|----------|
| source of a function (`Script.isNativeFunction(f)`, `toString`) | script function, has its source | native function, no source |
| a function sent to another thread (`Thread`, `Job`) | the source is compiled in that thread | not possible, a native function is not copied |
| recursion | on the heap, no limit | on the C++ stack of the thread, about 1500 levels with 1 MB |
| tail calls (not inside `try`) | any function | the running function (`return self(...)`, `return f(...)`) |
| stack trace of an error thrown by compiled code called from interpreted code | every frame | the frames inside that compiled code are not in it |

Typical times, interpreter / compiled (Windows, MSVC `/O2`, about 50 ms
process start included):

| Benchmark | Interpreter | Compiled |
|-----------|-------------|----------|
| loop on global variables, arithmetic | 725 ms | 299 ms |
| loop on local variables | 677 ms | 183 ms |
| recursive calls (`fib(25)`) | 122 ms | 104 ms |
| objects and arrays (constructors, methods, properties) | 6191 ms | 1071 ms |
| string append | 997 ms | 924 ms (up to 1800 ms, depends on memory reuse) |

Loops and expressions on local variables run 3 to 4 times faster, code with
many objects up to 6 times. A call still allocates the arguments array and
finds the function by name, as the interpreter.

## Compatibility

The generated code uses the API of the native runtime and of the engine.
**Generate the C++ file with the `quantum-script` of the engine it is built
with**, and generate it again when the engine changes. The generated file
checks `XYO_QUANTUMSCRIPT_NATIVERUNTIME_ABI` when compiled (`#error` if it
was generated for another native runtime) and `NativeRuntime::load` checks
it again at run time.

## C++ API

The generated file has its own namespace, `QuantumScriptCompiled_<name>` from
the name of the output file (`app.cpp` -> `QuantumScriptCompiled_app`):

| Symbol | Use |
|--------|-----|
| `int run(int cmdN, char *cmdS[], QuantumScriptInitExecutiveProc initExecutive)` | init the engine, run, return the exit code; what `main` does |
| `const NativeRuntime::ModuleInfo &getModuleInfo()` | the compiled code: symbols, constants, units |
| `XYO_QUANTUMSCRIPT_COMPILED_NO_MAIN` | define it to compile the file without `main` |

Namespace `XYO::QuantumScript::NativeRuntime`, header
`<XYO/QuantumScript.NativeRuntime.hpp>`:

| Symbol | Use |
|--------|-----|
| `TPointer<Variable> load(const ModuleInfo &info)` | after `ExecutiveX::initExecutive`: the module for the engine of this thread, the named units set as include functions; returns the main unit, a function to call with `this` = `Script` |
| `bool execute(const ModuleInfo &info, String &error, String &stackTrace)` | load and call the main unit as the interpreter runs a script; `false` on an uncaught error |
| `int run(const ModuleInfo &info, int cmdN, char *cmdS[], QuantumScriptInitExecutiveProc initExecutive)` | init, execute, print the error, end |
| `class Exception : public Error` | a thrown script value (`value`, `stackTrace`) |
| `class Exit` | `Script.exit` unwinding the compiled code |

To run compiled scripts in your own host (other internal extensions, several
compiled scripts in one program), define `XYO_QUANTUMSCRIPT_COMPILED_NO_MAIN`
and call `run` with your init callback, or, after
`ExecutiveX::initExecutive`, `NativeRuntime::load(getModuleInfo())` and call
the function it returns.

| Engine symbol | Use |
|---------------|-----|
| `Executive::setIncludeFunction(name, function)` / `removeIncludeFunction` / `hasIncludeFunction` | a named unit as a function: `Script.include(name)` calls it with the `this` and the arguments of the include and returns its result |
