---
name: quantum-script
description: >-
  How to use Quantum Script (namespace XYO::QuantumScript), the embeddable
  prototype-based JavaScript-like script engine of the XYO C++ stack, on top
  of xyo-system: writing scripts (.js run by the quantum-script interpreter,
  fabricare build scripts, .cmd launchers with --cmd) and their differences
  from JavaScript (typeof(x) needs parentheses, && / || return booleans,
  null == 0, sort returns a new array, no hoisting, no let / const / arrow /
  class / regex / exponent literals, Error needs new, yield coroutines that
  resume on each call); the standard library Script (include, includeOnce, execute,
  requireExtension, getExtensionList, exit, setExitCode, is* type tests,
  getFunctionFromString, compare), Convert, String.prototype, Array.prototype,
  AssociativeArray, Error, Console; extension loading (internal /
  quantum-script--name.dll) and the quantum-script--* extension catalog;
  bytecode .qsbc (--compile, --strip, --add bundles, --verify, Bytecode,
  CodeUnit, Linker); translation to C++ for the native runtime (--compile-cpp, NativeRuntime); embedding in C++ with ExecutiveX (initExecutive,
  executeFile, executeString, getError, getStackTrace, returnValue,
  endProcessing), Executive (setFunction2..5, setFunctionWithYield,
  compileStringX, registerInternalExtension, setIncludeSource, setIncludeUnit),
  the Variable API (VariableNumber / String / Array / Object / Resource
  ::newVariable, Context::getSymbol, getPropertyBySymbol, functionApply), and
  writing extensions (initExecutive, quantumScriptExtension entry point). Use
  when writing, reviewing or debugging Quantum Script code or fabricare .js
  scripts, code that includes <XYO/QuantumScript.hpp>, depends on
  "quantum-script" in fabricare.json, or when working inside the
  quantum-script repository or a quantum-script--* extension.
---

# quantum-script

Embeddable script engine of the XYO C++ libraries, on top of `xyo-system`
(see the `xyo-system`, `xyo-managed-memory`, `xyo-data-structures` and
`fabricare` skills; their rules apply to the C++ side). Purpose: **run
JavaScript-like scripts inside XYO programs** — the `quantum-script`
interpreter, fabricare build scripts, configurable tools — with native C++
functions exposed to scripts and script functions callable from C++.

Full documentation: `docs/` in the quantum-script repository
(`X:\Storage\XYO\Gitea\CPP\quantum-script\docs` on this machine): README,
getting-started, command-line, **language** (differences from JavaScript),
standard-library, modules, **embedding**, writing-extensions, bytecode,
compile-cpp, reference. Read the matching page when you need more than this summary.
When in doubt, run it: `output/bin/quantum-script --run="..."` (after
`fabricare make`) or the installed `quantum-script`; tests in `test/` show
real usage.

## Pick a tool

| Need | Use |
|------|-----|
| Print | `Script.requireExtension("Console"); Console.writeLn(x);` (`write` = no new line) |
| Read stdin lines | `Console.readLn(max)` — line **with** its end, `undefined` at EOF |
| Include a file | `Script.include("f.js", [args])` / `Script.includeOnce(...)`; returns the file's top level `return` |
| Run a string | `Script.execute(code)`; a function from text: `Script.getFunctionFromString(body)` |
| Load native module | `Script.requireExtension("JSON")` (external DLL first, then internal) |
| Exit code | `Script.exit(n)` (stops now) / `Script.setExitCode(n)` |
| Type test | `Script.isUndefined / isNil / isString / isNumber / isArray / isObject / isFunction(x)`, `typeof(x)` |
| Convert | `Convert.toNumber(x)`, `Convert.toString(x)`, `"" + x`, `Convert.numberFormat(x, 2)` |
| Map with any key, ordered | `var m = new AssociativeArray(); m[k] = v;` |
| Sort | `var sorted = list.sort(function(a, b) { return a - b; });` (new array) |
| Files, processes, JSON, math | extensions `Shell`, `File`, `ProcessInteractive`, `JSON`, `Math` (not in the core) |
| Script as a `.cmd` | line 1 `@quantum-script --cmd "%~f0" %*`, line 2 `@exit /b %ERRORLEVEL%`, script from line 3 |
| Ship without source | `quantum-script --compile=app.qsbc [--strip] [--add=lib.js] app.js` |
| Script as a C++ program | `quantum-script --compile-cpp=app.cpp app.js` (or `app.qsbc`), build `app.cpp` with `quantum-script.native-runtime` + `quantum-script--console` + `quantum-script`; regenerate when the engine changes |
| Run scripts from C++ | `ExecutiveX::initExecutive(cmdN, cmdS, initProc)` → `executeFile` / `executeString` → `endProcessing` |
| C++ function for scripts | in `initProc`: `compileStringX("var Host={};"); setFunction2("Host.f(a,b)", proc);` |
| Script function from C++ | `fn->functionApply(this_, arguments)`; catch `const Error &` |
| Native handle | `VariableResource::newVariable(ptr, deleteProc)` |

## Hard rules: the language is not JavaScript

Verified against the interpreter (engine 7.0). Write scripts this way:

1. **`typeof(x)`** with parentheses (`typeof x` is a compile error). Results
   are capitalized: `"Number" "String" "Boolean" "Object" "Array" "Function"
   "AssociativeArray"`, and `"undefined"`, `"null"`. Prefer `Script.is*`.
2. **`&&` / `||` return `true` / `false`**, never an operand: no
   `x = y || defaultValue`; write `Script.isUndefined(y) ? d : y`. `!!x` does
   not parse: `!(!x)` or `Convert.toBoolean(x)`.
3. **`null == 0` is true** (and `undefined == null`). Use `===`, `!==` or
   `Script.isNull` / `Script.isNil`.
4. **No hoisting**: `function f(){}` is executed where it stands; call it only
   after its definition ran.
5. Not supported: `let`, `const`, `=>`, `class`, template strings, regular
   expressions, `void`, labels (`break outer`), method shorthand, spread,
   destructuring, exponent literals (`1e3`). `var` is function scoped.
6. **No methods on numbers or functions**: not `x.toString()`, `toFixed`;
   use `"" + x`, `String(x)`, `Convert.numberFormat(x, n)`.
7. **Strings are UTF-8 bytes**: `length` counts bytes, no `s[i]` (gives
   `undefined`): `s.substring(i, 1)` or `s.getElement(i)` (byte value).
   **`substring(start, length)`** takes a length, not an end index.
   `replace(a, b)` replaces **all**, plain text. `split("")` gives the
   characters (UTF-8 sequences kept whole).
8. **Arrays**: `sort` / `reverseSort` return a **new** array (source
   unchanged, stable); `join(sep)` needs the separator; `length` is read
   only; reading out of range gives `undefined` without growing; writing a
   non-numeric index throws. Append: `a[a.length] = x` or `a.push(x)`.
9. **`yield`** in any function makes it a coroutine **without generator
   objects**: each call resumes after the last `yield`; after `return` /
   end, the next call starts over. State belongs to the function value: use a
   factory closure for independent sequences.
10. Core has no `Math`, `JSON`, `parseInt`, `Date`, `print`, `console`:
    `Script.requireExtension("Math" / "JSON" / "DateTime" / "Console")`,
    `Convert.toNumber`. `Console.writeLn` ends lines with `"\r\n"`.
11. `this` is `undefined` in a plain call; `global` is the global object
    (top level `this` is not). `self` is the running function (recursion of
    anonymous functions). `arguments` is an `Array`. Do not name a method
    `apply` (`o.apply()` is parsed as `Function.apply`).
12. Exceptions: `throw new Error("msg")` / `throw(new Error("msg"))`.
    **Never `Error("msg")` without `new`**: unlike JavaScript it does not
    build an error, it throws `setPropertyBySymbol` (`Error` sets
    `this.message`, and `this` is `undefined` in a plain call). Any thrown
    value is wrapped so `catch (e)` has `e.message`; `e.stackTrace` +
    `Script.stackTraceWithLevel(e.stackTrace, n)`. Uncaught: interpreter
    prints `Error: msg` + `- file f.js line N` on **stdout**, exit code 1.
13. `in` does not see `AssociativeArray` keys; test
    `Script.isUndefined(m[k])`. A number key and the same number as a string
    are one key. Plain objects iterate in symbol order, not insertion order;
    use `AssociativeArray` when order matters.
14. Style of XYO scripts: tabs, `;` after every statement **and block**
    (`};`), `var` at the top of functions, double quotes.

## Hard rules: C++ side

1. **One engine per thread** (`ExecutiveX` is a `TSingletonThread`):
   `initExecutive` once, then any number of `executeFile` / `executeString`
   (same global scope), then `endProcessing`. Values, symbols
   (`Context::getSymbol`) and functions belong to their thread; copy values
   across threads with `Executive::cloneVariable`.
2. Register **in the init callback** (`QuantumScriptInitExecutiveProc`,
   `void initProc(Executive *)`), which runs after the standard library and
   before scripts: internal extensions
   (`Extension::Console::registerInternalExtension(executive)`), native
   functions, `compileStringX` helpers. The parent object of a dotted name
   must exist first: `compileStringX("var Host={};")`. The `(a,b)` part of
   the name is documentation only.
3. Native function signature:
   `static TPointer<Variable> f(VariableFunction *function, Variable *this_, VariableArray *arguments)`.
   `arguments->index(k)` is never `nullptr` (missing = `undefined`; `index`
   creates missing elements). Return a value, at least
   `Context::getValueUndefined()`; never `nullptr`. `throw Error("msg")`
   becomes a script exception. Extra data: `setFunction3` (`function->super`,
   an owned `Object`), `setFunction4` (`function->valueSuper`, raw pointer),
   `setFunction5` (both).
4. Check types before casting: `TIsType<VariableString>(v)`,
   `TIsTypeExact<VariableUndefined>(v)` (`VariableUndefined` is `Variable`).
   Convert with `toNumber()`, `toString()`, `toBoolean()`, `toIndex()`.
5. Create values with `VariableX::newVariable(...)`; keep them in
   `TPointer<Variable>` (locals) / `TPointerX` (members) per
   `xyo-managed-memory`. Arrays for calls: `TPointer<VariableArray> a(VariableArray::newArray()); a->index(0) = ...;`.
6. `fn->functionApply(this_, arguments)` calls script or native functions
   (also from inside a native function); a script exception comes back as a
   C++ `Error` (catch `const Error &`, it is not `std::exception`; message via
   `getMessage()`), stack trace in `ExecutiveX::getStackTrace()`.
7. `executeFile` / `executeString` return `false` on compile error or
   uncaught exception: print `getError()` and `getStackTrace()`. Exit code:
   `ExecutiveX::getExitCode()`; after `Script.exit` `isExit()` is true.
8. Resources: `VariableResource::newVariable(ptr, deleteProc)`; `close()`
   deletes now, `drop()` forgets; scripts can call `handle.close()` /
   `handle.drop()` too, so natives must accept a closed handle
   (`resource == nullptr`).
9. Native coroutine: `setFunctionWithYield2(name, proc)` with
   `VariableFunctionWithYield *function`: switch on `function->yieldStep`,
   set it non zero to resume next call, `0` to restart; keep state in
   `function->yieldVariables`.
10. Extension = `initExecutive(Executive *, void *extensionId)` (set
    name / info / version / public, define objects) +
    `registerInternalExtension(executive)` + for the DLL
    `extern "C" EXPORT void quantumScriptExtension(Executive *, void *extensionId)`
    guarded by `#ifndef XYO_..._LIBRARY` / `#ifdef XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY`.
    DLL name `quantum-script--<lowercase name>.dll` / `.so`, found as given or
    in the include path (interpreter: script folder, then exe folder). DLL
    extensions need the DLL engine; static hosts register them internal.
11. Bytecode: same format version **and engine major** required; never save
    host init code (native instructions are run time only). Bundled units
    are used by `Script.include(name)` only when no real file / virtual
    source matches.

## Minimal patterns

Script:

```javascript
Script.requireExtension("Console");

function Counter(start) {
	this.value = start;
};
Counter.prototype.next = function() {
	return ++this.value;
};

var counter = new Counter(10);
var list = [];
for (var k = 0; k < 3; ++k) {
	list[list.length] = counter.next();
};

var sorted = list.reverseSort();
Console.writeLn(sorted.join(","));                 // 13,12,11

try {
	Script.include("optional.js");
} catch (e) {
	Console.writeLn("skipped: " + e.message);
};

if (typeof(counter) == "Object" && !Script.isNil(counter.value)) {
	Script.exit(0);
};
```

C++ host:

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>

using namespace XYO::QuantumScript;

static TPointer<Variable> hostAdd(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	return VariableNumber::newVariable(arguments->index(0)->toNumber() + arguments->index(1)->toNumber());
};

static void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);
	executive->compileStringX("var Host={};");
	executive->setFunction2("Host.add(a,b)", hostAdd);
};

class Application : public virtual IApplication {
		XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Application);

	public:
		inline Application(){};

		int main(int cmdN, char *cmdS[]);
};

int Application::main(int cmdN, char *cmdS[]) {
	if (!ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		printf("%s\n", ExecutiveX::getError().value());
		ExecutiveX::endProcessing();
		return 1;
	};
	if (!ExecutiveX::executeString("Script.requireExtension(\"Console\");\n"
	                               "Console.writeLn(Host.add(40, 2));\n"
	                               "function twice(x) { return x * 2; };\n")) {
		printf("%s\n%s", ExecutiveX::getError().value(), ExecutiveX::getStackTrace().value());
		ExecutiveX::endProcessing();
		return 1;
	};

	TPointer<Variable> fn = Context::getGlobalObject()->getPropertyBySymbol(Context::getSymbol("twice"));
	TPointer<VariableArray> fnArguments(VariableArray::newArray());
	fnArguments->index(0) = VariableNumber::newVariable(21);
	try {
		printf("%s\n", fn->functionApply(Context::getValueUndefined(), fnArguments)->toString().value());
	} catch (const Error &e) {
		printf("%s\n", (const_cast<Error &>(e)).getMessage().value());
	};

	int exitCode = ExecutiveX::getExitCode();
	ExecutiveX::endProcessing();
	return exitCode;
};

XYO_APPLICATION_MAIN(Application);
```

## Using it

- C++: `#include <XYO/QuantumScript.hpp>` (pulls in `XYO/System.hpp`);
  `XYO::QuantumScript` does `using namespace XYO::System`. C++17.
- fabricare consumer: `"dependency": ["quantum-script--console",
  "quantum-script"]` (DLL) or the `.static` variants (export
  `XYO_QUANTUMSCRIPT_LIBRARY`). Extensions from other repositories:
  `"quantum-script--json"`, ... Install `xyo-system` and below, then this
  library, before building dependents (see the `fabricare` skill).
- Interpreter: `quantum-script script.js|script.qsbc`, `--run="code"`,
  `--cmd`, `--execution-time`, `--compile=out.qsbc`, `--strip`,
  `--add=[name=]file`, `--verify=file.qsbc`, `--compile-cpp=out.cpp`,
  `--cpp-no-inline`, `@argsfile`. Script command
  line args: the `Application` extension.

## Working in this repository

- `fabricare make` runs `fabricare/make.prepare.js` first: `LibStd*.js` are
  converted to `LibStd*.Source.cpp` by `file-to-cs`. Edit the `.js`, not the
  generated `.Source.cpp`. New standard library natives go in
  `LibStd<Part>.cpp` `initExecutive` with `setFunction2("Object.f(x)", f)`.
- `fabricare test` runs every `test/test.NNNN.js` (and `test.exit-code.js`)
  as source, as bytecode, as stripped bytecode and one bundle, and translated
  to C++ (`--compile-cpp`, inline and `--cpp-no-inline`, all in one
  executable `temp/cpp/bin/test-cpp <name>`); a test fails
  by throwing or by a non-zero exit. New test: next number, use the
  `check(name, value, expected)` + `throw "test NNNN failed"` pattern, and
  raise the loop bounds in `fabricare/test.js`.
- Opcodes (`ParserAsm.hpp`) have explicit numbers, **append only** within a
  major version (bytecode compatibility). Bytecode format changes bump
  `Bytecode::formatVersion`; do not add reserved / speculative fields.
- `--compile-cpp` (`QuantumScript.Application/CompileCpp.cpp`) makes each
  script function a native function for the native runtime
  (`QuantumScript.NativeRuntime`, library `quantum-script.native-runtime`):
  per function an analysis (operand stack, contexts, try / finally, yield
  points) then the C++ code. `NativeRuntime/Instruction.hpp` and the inline
  code of the generator mirror `Instruction.cpp`: a change of an
  instruction's behavior goes in all of them. A new opcode needs its stack
  effect in `CompileCppFunction::step` and code in `emitInstruction` /
  `emitInline` / `emitRuntime`, else `--compile-cpp` refuses the code. A
  change of what generated code expects from the runtime bumps
  `XYO_QUANTUMSCRIPT_NATIVERUNTIME_ABI` and `XYO_QUANTUMSCRIPT_COMPILECPP_ABI`.
- `Config.hpp` is a tracked copy of `Config.Template.hpp`; keep them equal.
- C++ style: tabs, `};` after blocks, CRLF, SPDX headers (MIT in `source/`,
  Unlicense in `test/` and `fabricare/`), new `.cpp` added to
  `QuantumScript.Amalgam.cpp`, exported functions `XYO_QUANTUMSCRIPT_EXPORT`,
  include guards `XYO_QUANTUMSCRIPT_<NAME>_HPP`.
- Test on Windows (`win64-msvc-2026`) with `fabricare make` then
  `fabricare test`; clear `NoDefaultCurrentDirectoryInExePath` first (see the
  `fabricare` skill).
