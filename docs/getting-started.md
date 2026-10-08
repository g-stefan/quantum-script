# Getting started

## 1. Build and install

The engine is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. `xyo-platform`,
`xyo-managed-memory`, `xyo-data-structures`, `xyo-multithreading`,
`xyo-encoding` and `xyo-system` must be installed to the SDK first. From the
repository root:

```bash
fabricare make       # build into output/
fabricare test       # run every test/test.*.js as source, bytecode, stripped bytecode and bundle
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

`make` first runs `fabricare/make.prepare.js`, which converts the script part
of the standard library (`source/XYO/QuantumScript/LibStd*.js`) into C++
strings (`LibStd*.Source.cpp`) with `file-to-cs`. Edit the `.js` file, never
the generated `.Source.cpp`.

`fabricare.json` produces:

| Project | Kind | Use it when |
|---------|------|-------------|
| `quantum-script` | engine, DLL / shared library (`dll-or-lib`) | default: hosts and extensions share one engine |
| `quantum-script.static` | engine, static library, static CRT | self-contained executables |
| `quantum-script--console` | `Console` extension, DLL | loaded by the interpreter or by `requireExtension` |
| `quantum-script--console.static` | `Console` extension, static | linked into a static host |
| `quantum-script.application.static` | the interpreter as a library | to build a custom interpreter (as `magnet` does) |
| `quantum-script` (exe) | the `quantum-script` interpreter | run scripts from the command line |

After `make`, the interpreter is `output/bin/quantum-script` (next to
`quantum-script.dll` and `quantum-script--console.dll` on Windows).

## 2. First script

`hello.js`:

```javascript
Script.requireExtension("Console");

function greet(name) {
	return "Hello, " + name + "!";
};

var names = ["World", "Quantum Script"];
for (var name of names) {
	Console.writeLn(greet(name));
};
```

```bash
quantum-script hello.js
```

Nothing is printed by the core language: output comes from the `Console`
extension, which the interpreter has built in. The trailing `;` after a block
(`};`) is the style used by all XYO scripts; it is optional.

An uncaught exception prints the message and the stack trace and exits with
code 1:

```
Error: something failed
- file hello.js line 12
```

Next: [Language](language.md) (read the *Differences from JavaScript*
section), [Standard library](standard-library.md),
[Command line](command-line.md).

## 3. First C++ host

A program that runs a script, gives it two native functions and reads its
result:

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>

using namespace XYO::QuantumScript;

// Host.add(a,b)
static TPointer<Variable> hostAdd(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	return VariableNumber::newVariable(arguments->index(0)->toNumber() + arguments->index(1)->toNumber());
};

// Host.callTwice(fn) - a native function calling a script function
static TPointer<Variable> hostCallTwice(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	TPointer<VariableArray> fnArguments(VariableArray::newArray());
	fnArguments->index(0) = VariableNumber::newVariable(1);
	TPointer<Variable> first = arguments->index(0)->functionApply(Context::getValueUndefined(), fnArguments);
	fnArguments->index(0) = first;
	return arguments->index(0)->functionApply(Context::getValueUndefined(), fnArguments);
};

// called once by ExecutiveX::initExecutive, after the standard library
static void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);
	executive->compileStringX("var Host={};");
	executive->setFunction2("Host.add(a,b)", hostAdd);
	executive->setFunction2("Host.callTwice(fn)", hostCallTwice);
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

	const char *script =
	    "Script.requireExtension(\"Console\");\n"
	    "Console.writeLn(Host.add(40, 2));\n"
	    "Console.writeLn(Host.callTwice(function(x) { return x * 10; }));\n"
	    "return \"done\";\n";

	if (!ExecutiveX::executeString(script)) {
		printf("%s\n", ExecutiveX::getError().value());
		printf("%s", ExecutiveX::getStackTrace().value());
		ExecutiveX::endProcessing();
		return 1;
	};

	printf("result: %s\n", ExecutiveX::returnValue()->toString().value()); // done
	int exitCode = ExecutiveX::getExitCode(); // Script.exit / Script.setExitCode
	ExecutiveX::endProcessing();
	return exitCode;
};

XYO_APPLICATION_MAIN(Application);
```

Output:

```
42
100
result: done
```

The rules that matter (details in [Embedding](embedding.md)):

- `ExecutiveX` keeps **one engine per thread**; call `initExecutive` once,
  run any number of `executeFile` / `executeString`, then `endProcessing`.
- Register native functions and internal extensions **in the init callback**:
  it runs after the standard library is loaded and before any script.
- A native function receives `arguments->index(k)` (missing arguments are
  `undefined`, never `nullptr`) and returns a value, at least
  `Context::getValueUndefined()`.

## 4. Depend on it from a fabricare project

In the consumer's `fabricare.json`:

```json
{
	"name": "my-host",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script--console",
		"quantum-script"
	]
}
```

For a self-contained executable use `"quantum-script--console.static"` and
`"quantum-script.static"` (plus `"crt": "static"`); they export
`XYO_QUANTUMSCRIPT_LIBRARY` / `XYO_QUANTUMSCRIPT_EXTENSION_CONSOLE_LIBRARY`,
which turn the export macros into nothing. `xyo-system` and the layers below
come in as transitive dependencies. Extensions from the other
`quantum-script--*` repositories are added the same way
(`"quantum-script--json"`, ...) when the host links them in; otherwise
scripts load them as DLLs at run time.

Include `<XYO/QuantumScript.hpp>` (it pulls in `<XYO/System.hpp>` and every
layer below). Namespace `XYO::QuantumScript` does `using namespace
XYO::System;`, so `String`, `TPointer`, `Shell::`, ... are available
unqualified.

## 5. Configuration

`source/XYO/QuantumScript/Config.hpp` (a copy of `Config.Template.hpp`, keep
them the same) has the build switches, all off by default:

| Macro | Effect |
|-------|--------|
| `XYO_QUANTUMSCRIPT_DEBUG_ASM` | print the generated instructions while compiling |
| `XYO_QUANTUMSCRIPT_DEBUG_RUNTIME` | print each instruction / native function as it runs |
| `XYO_QUANTUMSCRIPT_DISABLE_ASM_OPTIMIZER` | turn off the instruction optimizer |
| `XYO_QUANTUMSCRIPT_DISABLE_CLOSURE` | compile without closures |
| `XYO_QUANTUMSCRIPT_STACK_TRACE_SHOW_MEMORY_SOURCE` | stack trace lines of code compiled from a string show its tag (`- memory <tag> line N`), not only `- memory line N` |

`Dependency.hpp` has two `#ifndef` guarded defaults, override them on the
compiler command line:

| Macro | Default | Effect |
|-------|---------|--------|
| `XYO_QUANTUMSCRIPT_DEFAULT_STACK_TRACE_LEVEL` | `16` | frames printed in a stack trace |
| `XYO_QUANTUMSCRIPT_CALL_CONTEXT_POOL_SIZE` | `16` | VM contexts kept for native code calling script functions |

## 6. Building without fabricare

Compile the amalgams of every layer together with your sources:

1. Put `source/` of `xyo-platform`, `xyo-managed-memory`,
   `xyo-data-structures`, `xyo-multithreading`, `xyo-encoding`, `xyo-system`
   and `quantum-script` on the include path, and create the generated
   `Config.hpp` files of the layers below (see their documentation).
2. Generate `LibStd*.Source.cpp` if missing (they are in git, so a checkout
   has them).
3. Compile `Platform.Amalgam.cpp`, `ManagedMemory.Amalgam.cpp`,
   `DataStructures.Amalgam.cpp`, `Multithreading.Amalgam.cpp`,
   `Encoding.Amalgam.cpp`, `System.Amalgam.cpp`,
   `QuantumScript.Amalgam.cpp` and, if you use it,
   `QuantumScript.Extension/Console.Amalgam.cpp`, with
   `XYO_<LIB>_LIBRARY` defined for every layer
   (`XYO_QUANTUMSCRIPT_LIBRARY`, `XYO_QUANTUMSCRIPT_EXTENSION_CONSOLE_LIBRARY`,
   ...) and, with MSVC, `XYO_PLATFORM_COMPILE_STATIC`.
4. Link `pthread` and `dl` on Linux, `user32` on Windows.
