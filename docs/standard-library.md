# Standard library

The core of the language has a small library, always loaded: the `Script`
and `Convert` objects, methods on the built-in prototypes, `Error`, and the
constructors. Everything else — console, files, processes, JSON, math,
network, threads — comes from extensions loaded with
`Script.requireExtension` (see [Modules and extensions](modules.md)). The
`Console` extension is part of this repository and is described at the end.

Native functions take their arguments by position; a missing argument is
`undefined`, which converts to `NaN` / `"undefined"` / `false`.

## Global names

| Name | What it is |
|------|------------|
| `undefined`, `null`, `true`, `false`, `NaN`, `Infinity` | constant values |
| `global` | the global object |
| `Boolean(x)`, `Number(x)`, `String(x)` | convert `x` (also with `new`, which gives the same plain value) |
| `Array()`, `Object()`, `Function`, `AssociativeArray()` | constructors; `new AssociativeArray()` is the only way to make an associative array |
| `Resource` | prototype of native handles: `handle.close()` frees it now, `handle.drop()` forgets it without freeing |
| `Error(message)` | error constructor; always call it with `new` (`new Error("x")`): without `new` it throws `setPropertyBySymbol` |
| `Script`, `Convert` | library objects, below |

## Script

### Type tests

| Function | True when |
|----------|-----------|
| `Script.isUndefined(x)` | `x` is `undefined` |
| `Script.isDefined(x)` | `x` is not `undefined` |
| `Script.isNull(x)` | `x` is `null` |
| `Script.isNil(x)` | `x` is `undefined` or `null` |
| `Script.isBoolean(x)`, `isNumber(x)`, `isString(x)` | the value has that type |
| `Script.isArray(x)`, `isObject(x)`, `isAssociativeArray(x)` | the value has that type (an array is not an object here) |
| `Script.isFunction(x)` | script or native function |
| `Script.isNativeFunction(x)` | function implemented in C++ |
| `Script.isResource(x)` | native handle |
| `Script.isStackTrace(x)` | `x` is a stack trace (`e.stackTrace`) |
| `Script.isNaN(x)`, `isFinite(x)`, `isInfinite(x)`, `signBit(x)` | tests on `Convert.toNumber(x)` |

### Running code and including files

| Function | Effect |
|----------|--------|
| `Script.include(file, args)` | compile and run `file` (script or `.qsbc`) in the current context; `args` (an array) is its `arguments`; returns the value of a top level `return` in the file |
| `Script.includeOnce(file, args)` | as `include`, once per real path (per unit name for bundled units) |
| `Script.includeSkipLines(file, args, n)` | as `include`, skipping the first `n` lines |
| `Script.execute(code, args, tag)` | compile and run a string; `tag` names it in error messages; returns its `return` value |
| `Script.executeSkipLines(code, args, n, tag)` | as `execute`, skipping the first `n` lines |
| `Script.getIncludedFile()` | name of the file being included, `""` at top level of a string |
| `Script.setIncludePath(path)` | add a folder to the include path (searched first) |
| `Script.getIncludePath()` | the include path, an array of strings |
| `Script.resetIncludePath()` | empty the include path |
| `Script.hasIncludeSource(name)` | the host registered a virtual file `name` (see [Embedding](embedding.md)) |

Code run by `include` / `execute` shares the global scope: a top level `var`
in the included file is a global variable. Errors (file not found, compile
error) are thrown as exceptions: `Compile error in file.js line 7`,
`Unable to open "file.js"`.

### Functions from source

| Function | Effect |
|----------|--------|
| `Script.getFunctionFromString(code)` | a new function whose body is `code` (`arguments`, `this` and `return` work) |
| `Script.getFunctionFromFile(file)` | the same, the body read from a file (searched in the include path) |
| `Script.setFunctionFromString(name, code, tag)` | assign such a function to `name` (`"Lib.f"`: `Lib` must exist) |
| `Script.setFunctionFromFile(name, file)` | the same from a file |
| `Script.protectSource(fn)` | remove the source text and the line information of a script function |

### Extensions

| Function | Effect |
|----------|--------|
| `Script.requireExtension(name)` | load an extension once: external DLL first, then internal (= `requireExtensionExternalOrInternal`) |
| `Script.requireExtensionInternalOrExternal(name)` | internal first, then external |
| `Script.requireExternalExtension(name)` | only `quantum-script--<name>.dll` / `.so` |
| `Script.requireInternalExtension(name)` | only extensions registered by the host |
| `Script.getExtensionList()` | array of `{fileName, name, info, version}` of the loaded public extensions |

### Program control and utilities

| Function | Effect |
|----------|--------|
| `Script.exit(code)` | stop the script now, process exit code `code` (`undefined` → `0`, not a number → `1`) |
| `Script.setExitCode(code)` | exit code to use when the script ends |
| `Script.getVersion()` | engine version, `"7.0.0.18"` |
| `Script.compare(a, b)` | `-1`, `0`, `1`; the default order of `sort` |
| `Script.reverseCompare(a, b)` | `-Script.compare(a, b)` |
| `Script.getPrototypeOf(x)` | the prototype object of `x` |
| `Script.stackTraceWithLevel(trace, n)` | the first `n` frames of `e.stackTrace` as text |
| `Script.forEach(value, fn(key, item), this_)` | call `fn` for every key of an object or array |
| `Script.with(this_, fn)` | call `fn` with `this` = `this_` |

## Convert

| Function | Result |
|----------|--------|
| `Convert.toBoolean(x)` | `false` for `undefined`, `null`, `false`, `0`, `NaN`, `""`; `true` otherwise (`"0"` is `true`) |
| `Convert.toNumber(x)` | number; a string is parsed from its start (`" 12 "` and `"12abc"` are `12`, `"0x10"` is `16`), `NaN` if it does not start with a number |
| `Convert.toString(x)` | the string form (`[1,2]` is `"1,2"`, an object is `"Object"`) |
| `Convert.toStringSource(x)` | the string as a C / script literal, with quotes: `"a\"b\n"` |
| `Convert.numberFormat(x, decimals)` | fixed number of decimals: `numberFormat(3.14159, 2)` is `"3.14"` |

## String

Strings are immutable UTF-8 byte strings; positions and lengths count
**bytes**. Methods (on `String.prototype`):

| Method | Result |
|--------|--------|
| `s.length` | length in bytes |
| `s.indexOf(text, start)` | first position of `text` at or after `start`, `-1` if not found |
| `s.lastIndexOf(text, skip)` | last position of `text`, ignoring the last `skip` bytes; `-1` if not found |
| `s.substring(start, length)` | **`length` bytes** from `start` (not an end index); without `length`: to the end |
| `s.replace(what, with)` | replace **every** occurrence of the text `what` |
| `s.split(separator)` | array of the parts; with `""`, the characters (a UTF-8 sequence stays whole) |
| `s.trim()` | without leading / trailing space, tab, CR, LF |
| `s.toLowerCaseASCII()`, `s.toUpperCaseASCII()` | ASCII letters only |
| `s.matchASCII(pattern)` | wildcard match, `*` and `?`: `"a.txt".matchASCII("*.txt")` |
| `s.getElement(pos)` | the byte at `pos` as a number (`0` outside the string) |
| `s.encodeC()` | C string literal with quotes |
| `s.encodeCX()` | C string escapes without the quotes |

There is no `s[i]`; use `s.substring(i, 1)` or `s.getElement(i)`.

## Array

| Method | Result |
|--------|--------|
| `a.length` | number of elements (read only) |
| `a.push(x, ...)` | append; returns `undefined` |
| `a.pop()` | remove and return the last element, `undefined` if empty |
| `a.shift()` | remove and return the first element |
| `a.join(separator)` | elements as strings joined by `separator` (**required**) |
| `a.sort(fnCompare)` | **new** array, stable merge sort; `fnCompare(x, y)` < 0 puts `x` first; default `Script.compare` |
| `a.reverseSort(fnCompare)` | new array, descending |
| `a.concat(x, ...)` | new array: `a`, then the elements of each array argument, or the argument itself |

## Function, Object, Error

| Name | Result |
|------|--------|
| `fn.call(this_, a, ...)` | call with `this` and arguments |
| `fn.apply(this_, array)` | call with `this` and an arguments array |
| `fn.bind(this_, a, ...)` | function that calls `fn` with `this_` and the arguments prepended |
| `Object.prototype.create(proto)` | new object whose prototype is `proto` (also `anyObject.create(proto)`) |
| `new Error(message)` | object with `message`; `toString()` gives `"Error: " + message` |
| `e.stackTrace` | stack trace of a thrown error (`Script.stackTraceWithLevel`) |

## AssociativeArray

`new AssociativeArray()` maps any key to a value, in insertion order.
`map[key]`, `map[key] = value`, `delete map[key]`, `map.length`,
`for (var key in map)`, `for (var value of map)`. A number and the same
number as a string are one key (`map[1]` is `map["1"]`); `for in` gives the
key as it was first stored. The `in` operator does not see its keys.

## Console

Part of this repository; built into the `quantum-script` interpreter, also
`quantum-script--console.dll`. Load it with
`Script.requireExtension("Console")`.

| Function | Effect |
|----------|--------|
| `Console.write(x)` | print `x` as a string, no new line |
| `Console.writeLn(x)` | print `x` and `"\r\n"` |
| `Console.readLn(max)` | read a line of at most `max` bytes from stdin, **with** its line end; `undefined` at end of input |
| `Console.keyHit()` | `true` if a key is waiting |
| `Console.getChar()` | wait for a key, return it as a one character string |
| `Console.getKey()` | the waiting key, or `""` |

stdout is unbuffered once `Console` is loaded.

```javascript
Script.requireExtension("Console");
var line;
while (!Script.isUndefined(line = Console.readLn(16384))) {
	Console.write(line.toUpperCaseASCII());
};
```
