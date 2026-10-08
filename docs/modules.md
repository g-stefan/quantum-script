# Modules and extensions

Quantum Script has no `import` syntax. Code is split in two ways:

- **Script files**, run in the current global scope with `Script.include`.
- **Extensions**, native C++ modules that add objects such as `Console`,
  `File` or `JSON`, loaded with `Script.requireExtension`.

## Including script files

```javascript
Script.include("lib/util.js");            // run every time it is called
Script.includeOnce("lib/util.js");        // run once per real path
var r = Script.include("task.js", [1, 2]); // arguments for the file, its top level return value
```

The included file runs as if its text were at that point of the program,
with the same global scope: its top level `var` and `function` definitions
become globals. Inside it, `arguments` is the array given to `include` and a
top level `return` ends the file and gives the result of `include`.

A file name is resolved in this order; the first match wins:

1. the name as given (absolute, or relative to the current directory);
2. every folder of the **include path** + `/` + name (the folder added last
   is searched first);
3. relative names only: next to the file that is being included (so
   `lib/a.js` can include `b.js` from `lib/`);
4. a **virtual file** with that exact name, registered by the host
   (`Executive::setIncludeSource`, see [Embedding](embedding.md));
5. a **named unit** of a bytecode bundle with that exact name (see
   [Bytecode](bytecode.md));
6. a named unit compiled to C++ (an include function, see
   [Compile to C++](compile-cpp.md)).

Otherwise `include` throws `Unable to open "name"`. A file that starts with
the bytecode magic (`QSBC`) is loaded as bytecode, whatever its extension. A
compile error throws `Compile error in file line N`.

The `quantum-script` interpreter starts with two include path folders: the
folder of the script (searched first) and the folder of the interpreter
executable. Scripts add more with `Script.setIncludePath(path)`, list them
with `Script.getIncludePath()` and clear them with `Script.resetIncludePath()`.

`Script.includeSkipLines(file, args, n)` skips the first `n` lines (they
still count for line numbers). `Script.execute(code)` and
`Script.executeSkipLines(code, args, n, tag)` do the same with a string.

## Loading extensions

```javascript
Script.requireExtension("Console");
Script.requireExtension("JSON");
Console.writeLn(JSON.encode({a: 1}));
```

An extension is loaded once; later calls with the same name do nothing.
Loading runs the extension's init function, which defines its global objects
(usually one object named like the extension).

There are two kinds:

- **Internal**: compiled into the host program and registered by name before
  scripts run (`Executive::registerInternalExtension`). The `quantum-script`
  interpreter registers `Console` (but `quantum-script--console.dll` sits next
  to it, so `requireExtension` loads the DLL first); `fabricare` and `magnet`
  register many.
- **External**: a shared library named `quantum-script--<name>.dll`
  (Windows) / `.so` (Linux), with `<name>` in lower case. A name with a path
  keeps it: `"lib/JSON"` loads `lib/quantum-script--json.dll`. The file is
  searched as given, then in every include path folder (so next to the
  interpreter, or next to the script). The library must export
  `quantumScriptExtension` (see [Writing extensions](writing-extensions.md)).

| Function | Order |
|----------|-------|
| `Script.requireExtension(name)` | external, then internal |
| `Script.requireExtensionInternalOrExternal(name)` | internal, then external |
| `Script.requireExternalExtension(name)` | external; if no file is found, a registered internal one |
| `Script.requireInternalExtension(name)` | internal only (name compared case insensitive) |

A missing extension throws `Unable to open "name"`.
`Script.getExtensionList()` returns `{fileName, name, info, version}` for
every loaded public extension (`fileName` is `""` for internal ones).

External extensions link to the engine DLL (`quantum-script.dll` /
`libquantum-script.so`), so they work in hosts that use the DLL build of the
engine. A host built with `quantum-script.static` uses its extensions as
internal ones.

## Extension catalog

Only `Console` is part of this repository. The others live in their own
`quantum-script--<name>` repositories, each with a `README.md` listing its
functions, and are built and installed with fabricare like any XYO library.

| Extension | Provides |
|-----------|----------|
| `Console` | `write`, `writeLn`, `readLn`, `keyHit`, `getChar`, `getKey` — [Standard library](standard-library.md#console) |
| `Application` | command line of the host: `getCmdN`, `getCmdS`, `getArgument`, `hasFlag`, `getFlagValue`, executable path |
| `ApplicationVersion` | `VersionCompare`, `ApplicationVersion.compare` |
| `Base16`, `Base32`, `Base64` | `encode`, `decode`, `decodeToBuffer` |
| `Buffer` | byte buffer: `getU8`, `setU8`, `set`, `toHex`, `fromHex`, `fromString`, `resize`, ... — see the `quantum-script--buffer` repository `docs/` |
| `Crypt` | `encrypt` / `decrypt` of strings and files |
| `CSV` | `CSV.decode`, `CSV.encode` |
| `DateTime` | date and time object |
| `File` | `File` object: open, read, write, seek, stdin / stdout |
| `HTTP` | HTTP requests, `jsonRequest`, `downloadFile`, `post` |
| `Job` | run processes in parallel with limits and timeouts |
| `JSON` | `JSON.decode`, `JSON.encode`, `JSON.encodeWithIndentation` |
| `Make` | make-like targets with dependencies (`Make`, `target`, `build`) |
| `Math` | `Math.abs`, `floor`, `sqrt`, `sin`, `PI`, ... |
| `MD5`, `SHA256`, `SHA512` | `hash`, `hashToBuffer` |
| `OpenSSL` | TLS connections (`ctxNew`, `bioConnect`, `bioRead`, ...) |
| `Pixel32` | 32-bit pixel helpers |
| `ProcessInteractive` | child process with pipes: `execute`, `read`, `readLn`, `write` |
| `Random` | `Random` generator: `next`, `seed`, `toInteger`, `toNumber` |
| `SCard` | smart card readers |
| `Shell` | files and processes: `fileGetContents`, `filePutContents`, `system`, `execute`, `getenv`, `copyFile`, `mkdirRecursivelyIfNotExists`, ... |
| `ShellFind` | directory listing with wildcards |
| `Socket` | TCP client and server: `openClient`, `openServer`, `read`, `write` |
| `SSHRemote` | run commands over SSH (`plink`) |
| `Task` | `TaskQueue`: `setTimeout`, `setInterval`, `join` |
| `Thread` | `Thread`, `CurrentThread.sleep`, thread start from a function or a file |
| `URL` | `encodeComponent`, `decodeComponent`, URL parts |
| `XML` | XML parser and writer |
| `Magnet` | registers all the extensions above as internal ones, in one library |
| `Example` | template for a new extension |
