# Embedding

This page is about running Quantum Script inside a C++ program: starting the
engine, exposing native functions, exchanging values, calling script
functions from C++ and handling errors. Every example on this page was
compiled and run against the library. For packaging native code as a
reusable module see [Writing extensions](writing-extensions.md).

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>   // only if you use Console

using namespace XYO::QuantumScript;
```

## The engine and its lifetime

`ExecutiveX` is the simple host API: static functions over **one engine per
thread** (a `TSingletonThread`). The life of an engine:

```cpp
static void initExecutive(Executive *executive) {
	// register extensions and native functions here
};

int Application::main(int cmdN, char *cmdS[]) {
	if (!ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {   // once
		printf("%s\n", ExecutiveX::getError().value());
		ExecutiveX::endProcessing();
		return 1;
	};
	ExecutiveX::includePath("scripts");                // optional
	if (!ExecutiveX::executeFile("scripts/main.js")) {  // any number of times
		printf("%s\n", ExecutiveX::getError().value());
		printf("%s", ExecutiveX::getStackTrace().value());
	};
	int exitCode = ExecutiveX::getExitCode();
	ExecutiveX::endProcessing();                       // once, at the end
	return exitCode;
};

XYO_APPLICATION_MAIN(Application);
```

| Function | Effect |
|----------|--------|
| `initExecutive(cmdN, cmdS, initProc)` | create the engine, add the executable folder to the include path, load the standard library, call `initProc(executive)`; `false` + `getError()` on failure |
| `executeFile(fileName)` | `Script.include(fileName)`: source or `.qsbc`, include path rules apply |
| `executeString(source)` | `Script.execute(source)` |
| `executeFileSkipLines` / `executeStringSkipLines(..., n)` | skip the first `n` lines |
| `returnValue()` | the value of the top level `return` of the last execute |
| `getError()` | `"Error: <message>"` of the last failure |
| `getStackTrace()` | stack trace text of the last uncaught script exception |
| `isExit()`, `getExitCode()` | `Script.exit` was called / the exit code (`Script.exit`, `Script.setExitCode`) |
| `includePath(path)` | add a folder to the include path (searched first) |
| `setIncludeSource(name, source)`, `removeIncludeSource(name)` | a virtual file for `Script.include(name)` |
| `getExecutive()` | the `Executive` of this thread, for the advanced API |
| `endProcessing()` | free the engine and its values, call the extensions' delete-context hooks |

All `execute*` calls share the same global scope, so a script can define
functions that a later call (or C++) uses. After `Script.exit`, check
`isExit()` and stop running more code.

The application must initialize managed memory before the engine
(`XYO_APPLICATION_MAIN` does it). Every engine object belongs to the thread
that created it.

## The init callback

`initProc` runs after the standard library is loaded and before any script.
It receives the `Executive` and is the place to:

```cpp
static void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);  // internal extension
	executive->compileStringX("var Host={};");                // create the object first
	executive->setFunction2("Host.add(a,b)", hostAdd);         // native functions
	executive->setFunction4("Host.getName()", hostGetName, (void *)"my-app");
	executive->compileStringX("Host.twice = function(x) { return x * 2; };");  // script helpers
};
```

`compileStringX(source)` compiles code that runs before the first script; it
throws `Error` on a compile error. Everything defined there is global.

## Native functions

A native function has this signature:

```cpp
typedef TPointer<Variable> (*FunctionProcedure)(VariableFunction *function, Variable *this_, VariableArray *arguments);
```

```cpp
// Host.add(a,b)
static TPointer<Variable> hostAdd(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	return VariableNumber::newVariable(arguments->index(0)->toNumber() + arguments->index(1)->toNumber());
};
```

- `arguments->index(k)` is the k-th argument; a missing one is `undefined`
  (a value, never `nullptr`). `arguments->value->length()` is the count.
  `VariableArray::index(k)` creates the missing elements up to `k`, so on an
  array you keep, check the length before reading past the end.
- `this_` is the object before the dot (`undefined` for a plain call).
- Return a value; return `Context::getValueUndefined()` for "nothing". Never
  return `nullptr`.
- `throw Error("message")` becomes a script exception (`catch (e)` gets
  `e.message == "message"`).

Registration, by full name:

| Function | The native function gets |
|----------|---------------------------|
| `setFunction2(name, proc)` | nothing extra |
| `setFunction3(name, proc, Object *super)` | `function->super`, an `Object` kept alive by the function |
| `setFunction4(name, proc, void *valueSuper)` | `function->valueSuper`, a raw pointer (not owned) |
| `setFunction5(name, proc, super, valueSuper)` | both |
| `setFunctionWithYield2..5(...)` | the same for a native coroutine, below |
| `setFunction(name, Variable *nativeFunction)` | an already created `VariableFunction` |

The name is a path of properties: `"Host.add"`, `"Host.Sub.f"`,
`"MyType.prototype.method"`, or a plain global `"add"`. Every object on the
path must already exist (create it with `compileStringX("var Host={};")`);
the text from `(` on, `"(a,b)"`, is ignored: it documents the parameters.

### Native coroutines

A function registered with `setFunctionWithYield*` keeps a step number and a
state array between calls, like a script function with `yield`:

```cpp
// Host.steps(): 1, 2, "end", then again from 1
static TPointer<Variable> hostSteps(VariableFunctionWithYield *function, Variable *this_, VariableArray *arguments) {
	switch (function->yieldStep) {
	case 0:
		function->yieldStep = 1;     // non zero: the next call resumes
		return VariableNumber::newVariable(1);
	case 1:
		function->yieldStep = 2;
		return VariableNumber::newVariable(2);
	default:
		break;
	};
	function->yieldStep = 0;         // finished, the next call starts again
	return VariableString::newVariable("end");
};
```

`function->yieldVariables` is an array for values kept between steps; it is
reset when the function starts from step 0.

## Values: the Variable API

Every script value is a `Variable` (a `DynamicObject` from
`xyo-data-structures`), held by `TPointer<Variable>` in locals and returned
values and by `TPointerX<Variable>` inside containers (the rules of
`xyo-managed-memory`).

Create values:

| Script value | C++ |
|--------------|-----|
| `undefined` | `Context::getValueUndefined()` |
| `null` | `VariableNull::newVariable()` |
| `true` / `false` | `VariableBoolean::newVariable(bool)` |
| number | `VariableNumber::newVariable(Number)` (`Number` is `double`) |
| string | `VariableString::newVariable(String)` |
| array | `VariableArray::newVariable()`, or `VariableArray::newArray()` for a typed `VariableArray *` |
| object | `VariableObject::newVariable()` |
| associative array | `VariableAssociativeArray::newVariable()` |
| native handle | `VariableResource::newVariable(void *resource, ResourceDelete deleteProc)` |

Read and convert:

```cpp
Variable *value = arguments->index(0);
bool b = value->toBoolean();
Number n = value->toNumber();
String s = value->toString();
size_t index = value->toIndex();        // number as an index, 0 for invalid values
String typeName = value->getVariableType();   // "Number", "String", ...

if (TIsType<VariableString>(value)) { ... };          // type test
if (TIsTypeExact<VariableUndefined>(value)) { ... };  // undefined (VariableUndefined is Variable itself)
if (TIsType<VariableNull>(value)) { ... };
String &raw = ((VariableString *)value)->value;       // direct access after the test
```

Properties and elements:

```cpp
TPointer<Variable> point(VariableObject::newVariable());
point->setPropertyBySymbol(Context::getSymbol("x"), VariableNumber::newVariable(1));
TPointer<Variable> x = point->getPropertyBySymbol(Context::getSymbol("x"));

TPointer<Variable> list(VariableArray::newVariable());
list->setPropertyByIndex(0, VariableString::newVariable("a"));
list->setPropertyByIndex(1, point);
size_t length = list->getPropertyBySymbol(Context::getSymbolLength())->toIndex();

point->setPropertyByVariable(keyVariable, value);     // x[key] = value, any key
point->deletePropertyBySymbol(Context::getSymbol("x"));
```

`Context::getSymbol(name)` interns a property name; symbols are per thread,
so get them in the thread that uses them (cache them in a thread local
object, not in a static).

Iterate the keys or values of any value with
`value->getIteratorKey()` / `getIteratorValue()` (the `for in` / `for of`
machinery), or for an array directly:

```cpp
if (TIsType<VariableArray>(value)) {
	VariableArray *array = (VariableArray *)value;
	for (size_t k = 0; k < array->value->length(); ++k) {
		TPointer<Variable> item = array->index(k);
	};
};
```

## Calling script functions from C++

`functionApply(this_, arguments)` calls any function value, script or
native:

```cpp
TPointer<Variable> fn = Context::getGlobalObject()->getPropertyBySymbol(Context::getSymbol("scriptMain"));
TPointer<Variable> thisObject(VariableObject::newVariable());
thisObject->setPropertyBySymbol(Context::getSymbol("base"), VariableNumber::newVariable(100));

TPointer<VariableArray> fnArguments(VariableArray::newArray());
fnArguments->index(0) = VariableNumber::newVariable(1);
fnArguments->index(1) = VariableNumber::newVariable(2);

try {
	TPointer<Variable> result = fn->functionApply(thisObject, fnArguments);
} catch (const Error &e) {
	// the script threw: e.getMessage() is the exception message,
	// ExecutiveX::getStackTrace() its stack trace
};
```

- Pass `Context::getValueUndefined()` as `this_` for a plain call.
- Test `value->isFunction()` (or `Script.isFunction` semantics) before
  calling a value received from a script.
- This works from inside a native function too (a callback: the script
  passes a function, the native code calls it), to any depth.
- `Context::getGlobalObject()` is the global object of the thread: global
  variables and functions are its properties.

## Errors

| Where | How it is reported |
|-------|--------------------|
| compile error in `executeFile` / `executeString` | `false`, `getError()` = `"Error: Compile error in file line N"` |
| uncaught script exception | `false`, `getError()` = `"Error: " + message`, `getStackTrace()` |
| script exception inside `functionApply` | C++ `Error` with the message |
| `throw Error(...)` in a native function | a script exception |
| compile error in `compileStringX` | C++ `Error` |

Catch `Error` by reference (`catch (const Error &e)`): it is the
`XYO::System::Error` class, not a `std::exception`.

## Virtual files

```cpp
ExecutiveX::setIncludeSource("config.js", "var config = {name: \"demo\"};");
```

`Script.include("config.js")` then compiles that text when no real file with
that name is found. `Script.hasIncludeSource(name)` tests for one. Use it to
embed library scripts in the executable. For precompiled code use bytecode
units (`Executive::setIncludeUnit`, see [Bytecode](bytecode.md)).

## Threads

Each thread has its own engine, symbols, global object and values. Values
can not be shared between threads; to move a value, copy it with
`Executive::cloneVariable(symbolList, value)` (this is how the `Thread` and
`Job` extensions pass arguments), and to run a function in another thread,
move its code with `Linker::unlinkFunction` + `Executive::newFunctionFromUnit`
(no closures over variables of enclosing functions).

## The Executive API

`ExecutiveX::getExecutive()` gives the `Executive` for the cases `ExecutiveX`
does not cover:

| Member | Use |
|--------|-----|
| `registerInternalExtension(name, initProc)` | make an extension loadable by `Script.requireExtension(name)` |
| `initExtension(initProc)` | run an extension's init now (no name, always loaded) |
| `setIncludeSource` / `removeIncludeSource` / `hasIncludeSource` | virtual files |
| `setIncludeUnit` / `removeIncludeUnit` / `hasIncludeUnit` | named bytecode units |
| `setIncludeFunction` / `removeIncludeFunction` / `hasIncludeFunction` | named units compiled to C++, functions called by `Script.include(name)` |
| `includePath` | the include path queue |
| `compileFileToUnit`, `compileStringToUnit` | compile to a `CodeUnit` without running ([Bytecode](bytecode.md)) |
| `newFunctionFromUnit(unit)` | a script function whose body is the unit |
| `callVmFunction(fn, this_, arguments)` | what `functionApply` uses for script functions |
| `cloneVariable(symbolList, value)` | copy a value for another thread |
| `mainCmdN`, `mainCmdS`, `pathExecutable` | the command line and executable path given to `initExecutive` |
| `setVmFunction(name, instructionProc, operand)` | a function implemented as a VM instruction (used by `Script.include`; advanced) |

The lower level path (`Executive` without `ExecutiveX`: `initExecutive`,
`compileFile`, `compileEnd`, `execute`) is what `ExecutiveX::initExecutive`
does internally; prefer `ExecutiveX`.
