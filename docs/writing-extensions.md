# Writing extensions

An extension is a C++ module that defines script objects and native
functions, loaded by `Script.requireExtension("Name")`. The same code can be:

- **internal**: linked into a host and registered with
  `registerInternalExtension` (static builds, single executables);
- **external**: a shared library `quantum-script--name.dll` / `.so` found next
  to the interpreter or in the include path.

The repository `quantum-script--example` is the template; `source/XYO/QuantumScript.Extension/Console` in this
repository is a complete small extension. Read [Embedding](embedding.md)
first: native functions and the `Variable` API are the same.

## Layout

```
quantum-script--example/
    fabricare.json
    source/XYO/QuantumScript.Extension/Example.hpp            umbrella header
    source/XYO/QuantumScript.Extension/Example.Amalgam.cpp    includes every .cpp
    source/XYO/QuantumScript.Extension/Example/
        Dependency.hpp       includes <XYO/QuantumScript.hpp>, export macro
        Library.hpp/.cpp     initExecutive, registerInternalExtension, the native functions
        Copyright, License, Version (.hpp/.cpp/.rh), Library.rc/.rh
    test/test.0001.js
```

`fabricare.json`:

```json
{
	"name": "quantum-script--example",
	"make": "dll-or-lib",
	"SPDX-License-Identifier": "MIT",
	"sourcePath": "XYO/QuantumScript.Extension/Example",
	"dependency": [
		"quantum-script"
	]
}
```

Add a `"quantum-script--example.static"` project (`"make": "lib"`,
`"dependency": ["quantum-script.static"]`, `"dependencyDefines":
["XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_LIBRARY"]`, `"crt": "static"`) for
static hosts, as the Console projects in this repository's `fabricare.json`.

## Export macro

`Dependency.hpp`:

```cpp
#ifndef XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_DEPENDENCY_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_DEPENDENCY_HPP

#ifndef XYO_QUANTUMSCRIPT_HPP
#	include <XYO/QuantumScript.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_INTERNAL
#	ifdef QUANTUM_SCRIPT__EXAMPLE_INTERNAL       // defined by fabricare while building this project
#		define XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_INTERNAL
#	endif
#endif

#ifdef XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_INTERNAL
#	define XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_EXPORT XYO_PLATFORM_LIBRARY_EXPORT
#else
#	define XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_EXPORT XYO_PLATFORM_LIBRARY_IMPORT
#endif
#ifdef XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_LIBRARY   // static build
#	undef XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_EXPORT
#	define XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_EXPORT
#endif

#endif
```

`Library.hpp` declares the two public functions:

```cpp
namespace XYO::QuantumScript::Extension::Example {
	XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_EXPORT void initExecutive(Executive *executive, void *extensionId);
	XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_EXPORT void registerInternalExtension(Executive *executive);
};
```

## Library.cpp

```cpp
#include <XYO/QuantumScript.Extension/Example/Library.hpp>
#include <XYO/QuantumScript.Extension/Example/License.hpp>
#include <XYO/QuantumScript.Extension/Example/Version.hpp>

namespace XYO::QuantumScript::Extension::Example {

	// Example.print(str)
	static TPointer<Variable> print(VariableFunction *function, Variable *this_, VariableArray *arguments) {
		printf("%s", (arguments->index(0))->toString().value());
		return Context::getValueUndefined();
	};

	// Example.process(fn) - calls a script function
	static TPointer<Variable> process(VariableFunction *function, Variable *this_, VariableArray *arguments) {
		TPointer<VariableArray> applyArguments(VariableArray::newArray());
		applyArguments->index(0) = VariableString::newVariable("Hello");
		return (arguments->index(0))->functionApply(Context::getValueUndefined(), applyArguments);
	};

	void registerInternalExtension(Executive *executive) {
		executive->registerInternalExtension("Example", initExecutive);
	};

	void initExecutive(Executive *executive, void *extensionId) {
		String info = "Example\r\n";
		info << License::shortLicense().c_str();

		executive->setExtensionName(extensionId, "Example");
		executive->setExtensionInfo(extensionId, info);
		executive->setExtensionVersion(extensionId, Extension::Example::Version::versionWithBuild());
		executive->setExtensionPublic(extensionId, true);   // listed by Script.getExtensionList()

		executive->compileStringX("var Example={};");
		executive->setFunction2("Example.print(str)", print);
		executive->setFunction2("Example.process(fn)", process);
	};

};

// entry point of the DLL / .so, looked up by name
#ifndef XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_LIBRARY
#	ifdef XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY
extern "C" XYO_QUANTUMSCRIPT_EXTENSION_EXAMPLE_EXPORT void quantumScriptExtension(XYO::QuantumScript::Executive *executive, void *extensionId) {
	XYO::QuantumScript::Extension::Example::initExecutive(executive, extensionId);
};
#	endif
#endif
```

```javascript
Script.requireExtension("Example");
Example.print(Example.process(function(x) {
	return x + " world!\r\n";
}));
```

Rules:

- `initExecutive` runs **once per engine** (per thread) the first time a
  script requires the extension. It must define everything the scripts use;
  it can load other extensions it depends on with
  `executive->compileStringX("Script.requireExtension(\"Buffer\");")`.
- Create the parent object before registering functions on it
  (`compileStringX("var Example={};")`).
- The extension name is case insensitive for loading; the DLL file name is
  `quantum-script--` + the name in lower case.
- Script functions written in JavaScript can be part of the extension:
  `compileStringX(source)` (keep longer sources in a `.js` file converted to
  a C string at build time, as `fabricare/make.prepare.js` does for the
  standard library).
- Throw `Error("...")` for invalid arguments; it becomes a script exception.

## Native handles: resources

The simple way to give a script a C++ object is a `VariableResource`: a
pointer plus a delete function, called when the script value is freed or
closed:

```cpp
struct Counter {
	int value;
};

static void counterDelete(void *resource) {
	delete (Counter *)resource;
};

// Example.counterNew(start)
static TPointer<Variable> counterNew(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	Counter *counter = new Counter();
	counter->value = (int)arguments->index(0)->toNumber();
	return VariableResource::newVariable(counter, counterDelete);
};

// Example.counterNext(counter)
static TPointer<Variable> counterNext(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	TPointerX<Variable> &handle = arguments->index(0);
	if (!TIsType<VariableResource>(handle)) {
		throw Error("invalid counter");
	};
	Counter *counter = (Counter *)((VariableResource *)handle.value())->resource;
	if (!counter) {
		throw Error("counter closed");
	};
	return VariableNumber::newVariable(++counter->value);
};

// Example.counterClose(counter)
static TPointer<Variable> counterClose(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	TPointerX<Variable> &handle = arguments->index(0);
	if (TIsType<VariableResource>(handle)) {
		((VariableResource *)handle.value())->close();   // calls counterDelete, resource = nullptr
	};
	return Context::getValueUndefined();
};
```

`typeof(handle)` is `"Resource"`, `Script.isResource(handle)` is true.
`close()` deletes the object now; `drop()` forgets it without deleting.
Scripts can call both directly (`handle.close()`, `Resource.prototype`);
the native functions must then accept a closed handle (`resource ==
nullptr`).

## Objects with methods: a custom Variable type

For `var f = new File(); f.openRead(name);` style objects, the extensions
define a `Variable` subclass, a constructor function and methods on its
prototype (see `quantum-script--file`, `VariableFile`):

1. `class VariableThing : public Variable` with
   `XYO_DYNAMIC_TYPE_DEFINE(<EXPORT>, VariableThing)` /
   `XYO_DYNAMIC_TYPE_IMPLEMENT(VariableThing, "{GUID}")`, the native state as
   a member, `getVariableType()` / `toString()` returning `"Thing"`,
   `instancePrototype()` returning the prototype kept in the extension
   context, `activeDestructor()` releasing the state, and a
   `TMemory<VariableThing>` specialization to `TMemoryPoolActive`.
2. A per thread context object (`TSingletonThread`) with the constructor
   symbol and a `TPointerX<Prototype>`; `initExecutive` fills it and
   registers a `deleteContext` hook with
   `executive->setExtensionDeleteContext(extensionId, deleteContext)` that
   frees it at `endProcessing`.
3. The constructor: a `VariableFunction::newVariable(nullptr, nullptr,
   nullptr, functionThing, nullptr, nullptr)` stored as the global `Thing`;
   `functionThing` returns `VariableThing::newVariable()`; keep
   `defaultPrototypeFunction->prototype` in the context.
4. Methods: `executive->setFunction2("Thing.prototype.method(x)", proc)`;
   inside, check `TIsType<VariableThing>(this_)` and throw
   `Error("invalid parameter")` otherwise.

## Native coroutines

`setFunctionWithYield2("Example.steps()", proc)` registers a function with
the signature
`TPointer<Variable> proc(VariableFunctionWithYield *function, Variable *this_, VariableArray *arguments)`
that keeps `function->yieldStep` and `function->yieldVariables` between
calls. See [Embedding](embedding.md#native-coroutines).

## Using an extension from a host

Internal (linked in):

```cpp
#include <XYO/QuantumScript.Extension/Example.hpp>

static void initExecutive(Executive *executive) {
	Extension::Example::registerInternalExtension(executive);
};
```

with `"quantum-script--example"` (or `.static`) in the host's fabricare
dependencies. External: install the DLL next to the host executable or in a
folder of the include path; nothing to register.

## Testing

Put scripts in `test/` and run them with the interpreter from a
`fabricare/test.js` (see this repository's): a test throws or calls
`Script.exit(1)` on failure, and the runner checks the exit codes. The
interpreter finds `output/bin/quantum-script--example.dll` when it runs from
`output/bin`, or add the folder with `Script.setIncludePath`.
