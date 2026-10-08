# Quantum Script

C++ prototype-based script engine
- JavaScript-like language: objects, prototypes, closures, exceptions,
`yield` coroutines, associative arrays.
- `quantum-script` command line interpreter: run `.js` scripts, scripts as
`.cmd` files, compile to `.qsbc` bytecode (stripped, bundled).
- Embeddable: run scripts from C++ with `ExecutiveX`, expose native functions with
`Executive::setFunction*`, call script functions with `functionApply`.
- Extensions loaded on demand with `Script.requireExtension`: internal or
`quantum-script--name.dll` / `.so`; `Console` included.

Built on `xyo-system`; used by `fabricare` (its build scripts are Quantum Script)
and the `quantum-script--*` extensions.

```javascript
Script.requireExtension("Console");
Console.writeLn("Hello, World!");
```

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, first script, first C++ host, depend on it, configuration
- [Command line](docs/command-line.md) - interpreter options, exit codes, `.cmd` scripts, bytecode commands
- [Language](docs/language.md) - syntax, values, functions, prototypes, coroutines, differences from JavaScript
- [Standard library](docs/standard-library.md) - `Script`, `Convert`, `String`, `Array`, `Error`, `AssociativeArray`, `Console`
- [Modules and extensions](docs/modules.md) - `Script.include`, include path, extension loading, extension catalog
- [Embedding](docs/embedding.md) - `ExecutiveX`, native functions, the `Variable` API, calling scripts from C++
- [Writing extensions](docs/writing-extensions.md) - internal and DLL extensions, resources, native types
- [Bytecode](docs/bytecode.md) - `.qsbc` files, bundles, compatibility, format, C++ API
- [API reference](docs/reference.md)

A Claude Code skill for this project is in
[.claude/skills/quantum-script](.claude/skills/quantum-script/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
