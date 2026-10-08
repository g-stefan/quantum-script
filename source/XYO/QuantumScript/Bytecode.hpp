// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_BYTECODE_HPP
#define XYO_QUANTUMSCRIPT_BYTECODE_HPP

#ifndef XYO_QUANTUMSCRIPT_CODEUNIT_HPP
#	include <XYO/QuantumScript/CodeUnit.hpp>
#endif

namespace XYO::QuantumScript {

	// Bytecode file (.qsbc), little endian
	//
	// header
	//   u8[4]  magic "QSBC"
	//   u16    format version
	//   u16    engine version major, minor, patch
	//   u32    engine build
	//   u32    section count
	// section directory, section count entries
	//   u32    type, BytecodeSection
	//   u32    offset from the start of the file
	//   u32    size
	// sections
	//   Strings - u32 count, count x (u32 length, bytes): symbol names and string values
	//   Code    - u32 count, count x (u16 opcode, operands by AsmOperand)
	//             labels are u32 instruction numbers, 1 is the first instruction, 0 is no label
	//   Source  - u32 run count, runs x (u32 instruction count, u32 string + 1 or 0, u32 line)
	//             consecutive instructions with the same source and line, optional
	//   Units   - u32 count, count x (u32 name length, name, u32 size, bytecode image)
	//             named units of a bundle, included by name; an image can not be a bundle
	//
	// A file loads if the format version and the engine major version are the same;
	// opcodes are stable within a major version, each opcode is checked by the loader.
	// A section with an unknown type is rejected.

	class BytecodeSection {
		public:
			enum {
				Strings = 1,
				Code = 2,
				Source = 3,
				Units = 4
			};
	};

	// Named units of a bundle
	typedef TRedBlackTree<String, TPointer<CodeUnit>> CodeUnitMap;

	class Bytecode {
		public:
			static const uint16_t formatVersion = 1;

			// true if data starts with the bytecode magic
			XYO_QUANTUMSCRIPT_EXPORT static bool isBytecode(const uint8_t *data, size_t size);

			// removed instructions are not saved, code with native or external operands can not be saved
			// units, if not empty, are saved as a bundle
			XYO_QUANTUMSCRIPT_EXPORT static bool save(CodeUnit &unit, Buffer &output, String &error, bool withSource = true, CodeUnitMap *units = nullptr);
			// the unit is emptied on error; a bundle requires units, its named units are added to units
			XYO_QUANTUMSCRIPT_EXPORT static bool load(const uint8_t *data, size_t size, CodeUnit &unit, String &error, CodeUnitMap *units = nullptr);

			XYO_QUANTUMSCRIPT_EXPORT static bool saveFile(CodeUnit &unit, const char *fileName, String &error, bool withSource = true, CodeUnitMap *units = nullptr);
			XYO_QUANTUMSCRIPT_EXPORT static bool loadFile(const char *fileName, CodeUnit &unit, String &error, CodeUnitMap *units = nullptr);

			// true if the file starts with the bytecode magic
			XYO_QUANTUMSCRIPT_EXPORT static bool isBytecodeFile(const char *fileName);
	};

};

#endif
