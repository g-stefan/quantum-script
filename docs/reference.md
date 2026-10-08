# API reference

Namespace `XYO::QuantumScript` (it does `using namespace XYO::System;`).
Include `<XYO/QuantumScript.hpp>`; the Console extension adds
`<XYO/QuantumScript.Extension/Console.hpp>`.

## Script globals

| Global | Members |
|--------|---------|
| `Script` | `isUndefined isDefined isNull isNil isBoolean isNumber isString isArray isObject isAssociativeArray isFunction isNativeFunction isResource isStackTrace isNaN isFinite isInfinite signBit` · `include includeOnce includeSkipLines execute executeSkipLines getIncludedFile setIncludePath getIncludePath resetIncludePath hasIncludeSource` · `getFunctionFromString getFunctionFromFile setFunctionFromString setFunctionFromFile protectSource` · `requireExtension requireExtensionExternalOrInternal requireExtensionInternalOrExternal requireExternalExtension requireInternalExtension getExtensionList` · `exit setExitCode getVersion compare reverseCompare getPrototypeOf stackTraceWithLevel forEach with` |
| `Convert` | `toBoolean toNumber toString toStringSource numberFormat` |
| `String.prototype` | `indexOf lastIndexOf substring replace split trim toLowerCaseASCII toUpperCaseASCII matchASCII getElement encodeC encodeCX` |
| `Array.prototype` | `join push pop shift sort reverseSort concat` |
| `Function.prototype` | `bind` (`call` / `apply` are syntax) |
| `Object.prototype` | `create` |
| `Resource.prototype` | `close drop` |
| `Error` | constructor, `message`, `stackTrace`, `toString()` |
| constructors | `Boolean Number String Array Object Function AssociativeArray Resource` |
| values | `undefined null true false NaN Infinity global` |
| `Console` (extension) | `write writeLn readLn keyHit getChar getKey` |

Details: [Standard library](standard-library.md).

## ExecutiveX — simple host API

Static members; one engine per thread.

| Member | |
|--------|-|
| `static bool initExecutive(int cmdN, char **cmdS, QuantumScriptInitExecutiveProc initProc)` | create the engine; `initProc(Executive *)` registers extensions / functions |
| `static bool executeFile(const char *fileName)` | run a script or bytecode file |
| `static bool executeString(const char *source)` | run source text |
| `static bool executeUnit(CodeUnit &unit)` | run a unit as the main script (embedded bytecode) |
| `static bool executeFileSkipLines(const char *fileName, size_t skipLines)` | |
| `static bool executeStringSkipLines(const char *source, size_t skipLines)` | |
| `static TPointer<Variable> returnValue()` | top level `return` value of the last execute |
| `static String getError()` | `"Error: ..."` of the last failure |
| `static String getStackTrace()` | stack trace of the last uncaught exception |
| `static bool isExit()` / `static int getExitCode()` | `Script.exit` called / exit code |
| `static void includePath(const char *path)` | add an include path folder |
| `static void setIncludeSource(String fileName, String source)` / `removeIncludeSource(String fileName)` | virtual files |
| `static Executive &getExecutive()` | the engine of this thread |
| `static void endProcessing()` | free the engine |

## Executive — the engine

`class Executive : public Object`, not copyable.

| Group | Members |
|-------|---------|
| State | `int mainCmdN; char **mainCmdS; String pathExecutable; int exitCode; bool isExit; TPointer<TDoubleEndedQueue<String>> includePath; ErrorInfo errorInfo; TPointer<InstructionContext> instructionContext; int configPrintStackTraceLimit;` |
| Native functions | `setFunction(name, Variable *)`, `setFunction2(name, FunctionProcedure)`, `setFunction3(name, proc, Object *super)`, `setFunction4(name, proc, void *valueSuper)`, `setFunction5(name, proc, super, valueSuper)`, `setFunctionWithYield2..5(...)` with `FunctionProcedureWithYield`, `setVmFunction(name, InstructionProcedure, Variable *operand)` |
| Script code at init | `void compileStringX(const char *source, const char *tag = nullptr)` (throws `Error`) |
| Extensions | `registerInternalExtension(String name, QuantumScriptExtensionInitProc)`, `initExtension(QuantumScriptExtensionInitProc)`, `isExtensionLoaded(String name)`, `setExtensionName / setExtensionInfo / setExtensionVersion(void *extensionId, String)`, `setExtensionPublic(void *extensionId, bool)`, `setExtensionDeleteContext(void *extensionId, QuantumScriptExtensionDeleteContextProc)`, `executeExtension(context, QuantumScriptExtensionProc, void *extensionId)` |
| Include | `setIncludeSource / removeIncludeSource / hasIncludeSource(String fileName ...)`, `setIncludeUnit(String name, CodeUnit *) / removeIncludeUnit / hasIncludeUnit`, `setIncludeFunction(String name, Variable *function) / removeIncludeFunction / hasIncludeFunction` (a unit compiled to C++), `includeAndExecuteFile(context, fileName)`, `includeAndExecuteString(context, source, tag)`, `...SkipLines` variants, `includeAndExecuteUnit(context, CodeUnit &)` |
| Compile | `compileFile(fileName)`, `compileString(source, tag)`, `...SkipLines`, `compileEnd()`, `compileFileToUnit(fileName, CodeUnit &, skipLines = 0)`, `compileStringToUnit(source, CodeUnit &, tag)`, `setVmFunctionFromFile / FromString(name, ...)` and the `...X(context, ...)` variants |
| Run | `initExecutive()`, `execute()`, `endProcessing()` |
| Values | `TPointer<Variable> callVmFunction(Variable *fn, Variable *this_, Variable *arguments)`, `TPointer<Variable> newFunctionFromUnit(CodeUnit &, int functionHint = ParserFunctionHint::All)`, `Variable *cloneVariable(SymbolList &, Variable *)` |
| Static | `static void initMemory()` |

Return codes of the compile / include functions: `VmParserError::None`,
`Compile`, `FileNotFound`, `Bytecode` (`errorInfo.bytecodeError` has the
reason). `ErrorInfo`: `compileError`, `compileLineNumber`,
`compileFileName`, `bytecodeError`.

Callback types:

```cpp
typedef void (*QuantumScriptInitExecutiveProc)(Executive *);                 // host init
typedef void (*QuantumScriptExtensionInitProc)(Executive *, void *extensionId); // extension init
typedef void (*QuantumScriptExtensionDeleteContextProc)();                   // at endProcessing
extern "C" typedef void (*QuantumScriptExtensionProc)(void *, void *);       // DLL entry "quantumScriptExtension"
typedef TPointer<Variable> (*FunctionProcedure)(VariableFunction *function, Variable *this_, VariableArray *arguments);
typedef TPointer<Variable> (*FunctionProcedureWithYield)(VariableFunctionWithYield *function, Variable *this_, VariableArray *arguments);
```

## Context — per thread state

| Member | |
|--------|-|
| `static Symbol getSymbol(String name)` | intern a property name |
| `static bool findSymbol(const String &name, Symbol &out)` | look up without creating |
| `static String getSymbolMirror(Symbol)` | name of a symbol |
| `static TPointer<Variable> getValueUndefined()` | `undefined` |
| `static Variable *getValueBoolean(bool)` | shared `true` / `false` |
| `static TPointer<Variable> getGlobalObject()` | global object |
| `static Symbol getSymbolLength() / getSymbolPrototype() / getSymbolSize() / getSymbolToString() / getSymbolToNumber()` | common symbols |
| `static Symbol getSymbolFunctionBoolean() ... getSymbolFunctionResource()` | constructor names |
| `static TPointer<Prototype> getPrototypeBoolean() / Number / String / Array / Function / Object / AssociativeArray / Resource` | built-in prototypes |

## Variable and value types

Basic types: `typedef double Number; typedef int64_t Integer; typedef uint64_t UInteger; typedef bool Boolean; typedef uint32_t Symbol;`
`typedef Variable VariableUndefined;`

`class Variable : public DynamicObject` (not copyable):

| Member | |
|--------|-|
| `virtual bool toBoolean()`, `virtual Number toNumber()`, `virtual String toString()`, `size_t toIndex()`, `virtual bool isString()`, `virtual bool isFunction()` | conversion |
| `virtual TPointer<Variable> getPropertyBySymbol(Symbol) / getPropertyByIndex(size_t) / getPropertyByVariable(Variable *)` | `x.y`, `x[i]` |
| `virtual void setPropertyBySymbol / setPropertyByIndex / setPropertyByVariable(..., Variable *value)` | assignment |
| `virtual bool deletePropertyBySymbol / deletePropertyByIndex / deletePropertyByVariable(...)` | `delete` |
| `virtual bool hasPropertyByVariable(Variable *)` | `in` |
| `virtual TPointer<Variable> functionApply(Variable *this_, VariableArray *arguments)` | call |
| `virtual Variable *instancePrototype()`, `bool instanceOf(Variable *)` | prototypes |
| `virtual TPointer<Iterator> getIteratorKey() / getIteratorValue()` | `for in` / `for of` |
| `virtual String getVariableType()` | `typeof` name |
| `bool isEqual / isEqualStrict / isLess / isLessOrEqual / isGreater / isGreaterOrEqual(Variable *)`, `int compare(Variable *)`, `TPointer<Variable> operatorPlus(Variable *)` | operators |
| `virtual Variable *clone(SymbolList &)` | copy for another thread |

| Type | Factory | Data |
|------|---------|------|
| `VariableNull` | `newVariable()` | |
| `VariableBoolean` | `newVariable(bool)` | `Boolean value` |
| `VariableNumber` | `newVariable(Number)` | `Number value` |
| `VariableString` | `newVariable(String)` | `String value` |
| `VariableArray` | `newVariable()`, `newArray()` (typed) | `TPointerX<Array> value` (`TDynamicArray<TPointerX<Variable>>`), `TPointerX<Variable> &index(uint32_t)` (creates missing elements), `String join(String)` |
| `VariableObject` | `newVariable()` | `TPointerX<Property> value`, `TPointerX<Prototype> prototype` |
| `VariableAssociativeArray` | `newVariable()` | `TPointerX<AssociativeArray> value` |
| `VariableFunction` | `newVariable(functionParent, parentVariables, parentArguments, FunctionProcedure, Object *super, void *valueSuper)` | `super`, `valueSuper`, `functionProcedure`, `prototype`, `object` |
| `VariableFunctionWithYield` | as `VariableFunction` with `FunctionProcedureWithYield` | `uint32_t yieldStep`, `TPointerX<Variable> yieldVariables` |
| `VariableResource` | `newVariable(void *resource, ResourceDelete)` | `void *resource`, `close()`, `drop()`; `typedef void (*ResourceDelete)(void *)` |
| `VariableSymbol` | `newVariable(Symbol)` | `Symbol value` |
| `VariableStackTrace` | (created by the VM) | `toString()`, `toString(int level)` |

Script functions are `VariableVmFunction` (call them with `functionApply`).
Type tests: `TIsType<VariableX>(value)`, `TIsTypeExact<VariableUndefined>(value)`.

## Bytecode, CodeUnit, Linker

| Symbol | |
|--------|-|
| `Bytecode::formatVersion` | `1` |
| `static bool Bytecode::save(CodeUnit &, Buffer &output, String &error, bool withSource = true, CodeUnitMap *units = nullptr)` | |
| `static bool Bytecode::load(const uint8_t *data, size_t size, CodeUnit &, String &error, CodeUnitMap *units = nullptr)` | |
| `static bool Bytecode::saveFile / loadFile(...)` | file versions |
| `static bool Bytecode::isBytecode(const uint8_t *, size_t)`, `isBytecodeFile(const char *)` | magic test |
| `BytecodeSection::Strings / Code / Source / Units` | `1..4` |
| `typedef TRedBlackTree<String, TPointer<CodeUnit>> CodeUnitMap` | named units |
| `class CodeUnit` | `instruction`, `length()`, `at(label)`, `push()`, `pop()`, `truncate(label)`, `empty()`, `append(CodeUnit &)` |
| `class CodeInstruction` | `asmType`, `operand`, `target`, `targetRange`, `sourceSymbol`, `sourceLineNumber`; run time only: `targetExternal` |
| `class Linker` | `link(CodeUnit &, InstructionList &)`, `getProgramCounter(label)`, `static bool unlinkFunction(Variable *fn, CodeUnit &, String &error)` |

See [Bytecode](bytecode.md).

## NativeRuntime — code compiled to C++

Namespace `XYO::QuantumScript::NativeRuntime`, header
`<XYO/QuantumScript.NativeRuntime.hpp>`, library `quantum-script.native-runtime`:

| Symbol | |
|--------|-|
| `TPointer<Variable> load(const ModuleInfo &)` | the compiled module for this thread, returns the main unit |
| `bool execute(const ModuleInfo &, String &error, String &stackTrace)` | run the main unit as a script |
| `int run(const ModuleInfo &, int cmdN, char *cmdS[], QuantumScriptInitExecutiveProc)` | init, execute, end |
| `class ModuleInfo`, `class Module` | constants of the generated code / per thread data |
| `class Exception : public Error`, `class Exit` | thrown script value, `Script.exit` |
| `XYO_QUANTUMSCRIPT_NATIVERUNTIME_ABI` | interface version |

See [Compile to C++](compile-cpp.md).

## Console extension

Namespace `XYO::QuantumScript::Extension::Console`:

| Function | |
|----------|-|
| `void registerInternalExtension(Executive *)` | make `Console` loadable |
| `void initExecutive(Executive *, void *extensionId)` | the extension init |
| `extern "C" void quantumScriptExtension(Executive *, void *)` | DLL entry |

## Metadata

| Namespace | Functions |
|-----------|-----------|
| `XYO::QuantumScript::Version` | `version()` (`"7.0.0"`), `build()`, `versionWithBuild()`, `datetime()` |
| `XYO::QuantumScript::Copyright` | `copyright()`, `publisher()`, `company()`, `contact()` |
| `XYO::QuantumScript::License` | `license()`, `shortLicense()` |

## Configuration macros

| Macro | Where | Default |
|-------|-------|---------|
| `XYO_QUANTUMSCRIPT_DEBUG_ASM`, `XYO_QUANTUMSCRIPT_DEBUG_RUNTIME`, `XYO_QUANTUMSCRIPT_DISABLE_ASM_OPTIMIZER`, `XYO_QUANTUMSCRIPT_DISABLE_CLOSURE`, `XYO_QUANTUMSCRIPT_STACK_TRACE_SHOW_MEMORY_SOURCE` | `Config.hpp` | off |
| `XYO_QUANTUMSCRIPT_DEFAULT_STACK_TRACE_LEVEL` | `Dependency.hpp` | `16` |
| `XYO_QUANTUMSCRIPT_CALL_CONTEXT_POOL_SIZE` | `Dependency.hpp` | `16` |
| `XYO_QUANTUMSCRIPT_LIBRARY` | consumer of the static library | |
| `XYO_QUANTUMSCRIPT_EXPORT` | export macro | |
