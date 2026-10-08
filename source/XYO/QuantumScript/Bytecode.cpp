// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <algorithm>

#include <XYO/QuantumScript/Bytecode.hpp>
#include <XYO/QuantumScript/Version.hpp>
#include <XYO/QuantumScript/Context.hpp>
#include <XYO/QuantumScript/VariableSymbol.hpp>
#include <XYO/QuantumScript/VariableBoolean.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>
#include <XYO/QuantumScript/VariableString.hpp>
#include <XYO/QuantumScript/VariableArgumentLevel.hpp>
#include <XYO/QuantumScript/VariableOperator21.hpp>
#include <XYO/QuantumScript/VariableOperator22.hpp>
#include <XYO/QuantumScript/VariableOperator31.hpp>

namespace XYO::QuantumScript {

	static const uint8_t bytecodeMagic[4] = {'Q', 'S', 'B', 'C'};
	static const size_t bytecodeHeaderSize = 20;
	static const size_t bytecodeSectionEntrySize = 12;
	static const uint32_t bytecodeSectionCountLimit = 64;

	static void bytecodeEngineVersion(uint16_t &major, uint16_t &minor, uint16_t &patch, uint32_t &build) {
		unsigned int versionMajor = 0;
		unsigned int versionMinor = 0;
		unsigned int versionPatch = 0;
		sscanf(Version::version(), "%u.%u.%u", &versionMajor, &versionMinor, &versionPatch);
		major = (uint16_t)versionMajor;
		minor = (uint16_t)versionMinor;
		patch = (uint16_t)versionPatch;
		build = (uint32_t)strtoul(Version::build(), nullptr, 10);
	};

	class BytecodeWriter {
		public:
			Buffer &output;

			inline BytecodeWriter(Buffer &output_) : output(output_){};

			inline void reserve(size_t count) {
				if (output.length + count <= output.size) {
					return;
				};
				size_t newSize = output.size * 2;
				if (newSize < output.length + count) {
					newSize = output.length + count;
				};
				if (newSize < 4096) {
					newSize = 4096;
				};
				output.setSize(newSize);
			};

			inline void u8(uint8_t value) {
				reserve(1);
				output.buffer[output.length++] = value;
			};

			inline void u16(uint16_t value) {
				u8((uint8_t)(value & 0xFF));
				u8((uint8_t)(value >> 8));
			};

			inline void u32(uint32_t value) {
				for (int k = 0; k < 4; ++k) {
					u8((uint8_t)((value >> (8 * k)) & 0xFF));
				};
			};

			inline void f64(double value) {
				uint64_t bits;
				memcpy(&bits, &value, sizeof(bits));
				for (int k = 0; k < 8; ++k) {
					u8((uint8_t)((bits >> (8 * k)) & 0xFF));
				};
			};

			inline void bytes(const void *data, size_t count) {
				reserve(count);
				memcpy(output.buffer + output.length, data, count);
				output.length += count;
			};
	};

	// Every read is bounds checked, ok is false after the first failed read
	class BytecodeReader {
		public:
			const uint8_t *data;
			size_t size;
			size_t position;
			bool ok;

			inline BytecodeReader(const uint8_t *data_, size_t size_) {
				data = data_;
				size = size_;
				position = 0;
				ok = true;
			};

			inline bool need(size_t count) {
				if (!ok || (size - position) < count) {
					ok = false;
					return false;
				};
				return true;
			};

			inline bool isEnd() const {
				return ok && (position == size);
			};

			inline uint8_t u8() {
				if (!need(1)) {
					return 0;
				};
				return data[position++];
			};

			inline uint16_t u16() {
				if (!need(2)) {
					return 0;
				};
				uint16_t value = (uint16_t)(data[position] | (data[position + 1] << 8));
				position += 2;
				return value;
			};

			inline uint32_t u32() {
				if (!need(4)) {
					return 0;
				};
				uint32_t value = 0;
				for (int k = 0; k < 4; ++k) {
					value |= ((uint32_t)data[position + k]) << (8 * k);
				};
				position += 4;
				return value;
			};

			inline double f64() {
				if (!need(8)) {
					return 0;
				};
				uint64_t bits = 0;
				for (int k = 0; k < 8; ++k) {
					bits |= ((uint64_t)data[position + k]) << (8 * k);
				};
				position += 8;
				double value;
				memcpy(&value, &bits, sizeof(value));
				return value;
			};

			inline const uint8_t *bytes(size_t count) {
				if (!need(count)) {
					return nullptr;
				};
				const uint8_t *value = data + position;
				position += count;
				return value;
			};
	};

	// String table of a unit being saved
	class BytecodeStrings {
		public:
			TRedBlackTree<String, uint32_t> index;
			TDynamicArray<String> list;

			inline uint32_t add(const String &value) {
				uint32_t retV;
				if (index.get(value, retV)) {
					return retV;
				};
				retV = (uint32_t)list.length();
				list.index(retV) = value;
				index.set(value, retV);
				return retV;
			};
	};

	bool Bytecode::isBytecode(const uint8_t *data, size_t size) {
		if (size < sizeof(bytecodeMagic)) {
			return false;
		};
		return memcmp(data, bytecodeMagic, sizeof(bytecodeMagic)) == 0;
	};

	static String bytecodeErrorAt(const char *message, size_t instruction) {
		char buf[256];
		snprintf(buf, sizeof(buf), "%s at instruction %u", message, (unsigned int)instruction);
		return buf;
	};

	bool Bytecode::save(CodeUnit &unit, Buffer &output, String &error, bool withSource, CodeUnitMap *units) {
		size_t length = unit.length();
		size_t k;

		// new label of each instruction, removed instructions resolve to the next instruction
		std::vector<uint32_t> newLabel(length + 1, 0);
		uint32_t count = 0;
		for (k = 0; k < length; ++k) {
			if (!unit.instruction.index(k).isRemoved) {
				newLabel[k + 1] = ++count;
			};
		};
		uint32_t next = 0;
		for (k = length; k > 0; --k) {
			if (unit.instruction.index(k - 1).isRemoved) {
				newLabel[k] = next;
				continue;
			};
			next = newLabel[k];
		};
		auto mapLabel = [&](AsmLabel label) -> uint32_t {
			if (label == 0 || label > length) {
				return 0;
			};
			return newLabel[label];
		};

		BytecodeStrings strings;
		Buffer codeBuffer;
		BytecodeWriter code(codeBuffer);

		// source, runs of instructions with the same source and line
		std::vector<uint32_t> runCount;
		std::vector<uint32_t> runSource;
		std::vector<uint32_t> runLine;

		code.u32(count);

		try {
			for (k = 0; k < length; ++k) {
				CodeInstruction &in = unit.instruction.index(k);
				if (in.isRemoved) {
					continue;
				};
				const AsmOpcodeInfo *info = AsmOpcode::getInfo(in.asmType);
				if (!info || !info->procedure) {
					error = bytecodeErrorAt("Unknown opcode", k + 1);
					return false;
				};
				if (in.targetExternal) {
					error = bytecodeErrorAt("External jump can not be saved", k + 1);
					return false;
				};
				Variable *operand = in.operand.value();
				switch (info->operand) {
				case AsmOperand::None:
				case AsmOperand::ProgramCounter:
				case AsmOperand::Function:
				case AsmOperand::ProgramCounterRange:
					break;
				case AsmOperand::Native:
					error = bytecodeErrorAt("Native instruction can not be saved", k + 1);
					return false;
				default:
					if (!operand) {
						error = bytecodeErrorAt("Missing operand", k + 1);
						return false;
					};
					break;
				};

				code.u16((uint16_t)in.asmType);
				switch (info->operand) {
				case AsmOperand::Symbol:
					code.u32(strings.add(Context::getSymbolMirror(((VariableSymbol *)operand)->value)));
					break;
				case AsmOperand::Index:
					code.u32(((VariableSymbol *)operand)->value);
					break;
				case AsmOperand::Boolean:
					code.u8(operand->toBoolean() ? 1 : 0);
					break;
				case AsmOperand::Number:
					code.f64(((VariableNumber *)operand)->value);
					break;
				case AsmOperand::String:
					code.u32(strings.add(((VariableString *)operand)->value));
					break;
				case AsmOperand::IndexLevel:
					code.u32((uint32_t)((VariableArgumentLevel *)operand)->value);
					code.u32((uint32_t)((VariableArgumentLevel *)operand)->level);
					break;
				case AsmOperand::ProgramCounter:
					code.u32(mapLabel(in.target));
					break;
				case AsmOperand::Function:
					code.u32(mapLabel(in.target));
					code.u32(mapLabel(in.targetRange));
					code.u32((uint32_t)in.functionHint);
					break;
				case AsmOperand::ProgramCounterRange:
					code.u32(mapLabel(in.targetRange));
					code.u32(mapLabel(in.target));
					break;
				case AsmOperand::IndexNumber:
					code.u32(((VariableOperator21 *)operand)->symbol);
					code.f64(((VariableOperator21 *)operand)->value);
					break;
				case AsmOperand::IndexIndex:
					code.u32(((VariableOperator22 *)operand)->symbol1);
					code.u32(((VariableOperator22 *)operand)->symbol2);
					break;
				case AsmOperand::IndexNumberGoto:
					code.u32(((VariableOperator31 *)operand)->symbol);
					code.f64(((VariableOperator31 *)operand)->value);
					code.u32(mapLabel(in.target));
					break;
				case AsmOperand::SymbolNumberGoto:
					code.u32(strings.add(Context::getSymbolMirror(((VariableOperator31 *)operand)->symbol)));
					code.f64(((VariableOperator31 *)operand)->value);
					code.u32(mapLabel(in.target));
					break;
				default:
					break;
				};

				if (withSource) {
					uint32_t sourceIndex = in.sourceSymbol ? strings.add(Context::getSymbolMirror(in.sourceSymbol)) + 1 : 0;
					if (!runCount.empty() && runSource.back() == sourceIndex && runLine.back() == in.sourceLineNumber) {
						++runCount.back();
					} else {
						runCount.push_back(1);
						runSource.push_back(sourceIndex);
						runLine.push_back(in.sourceLineNumber);
					};
				};
			};
		} catch (const Error &) {
			error = "Unknown symbol";
			return false;
		};

		Buffer stringsBuffer;
		BytecodeWriter stringsWriter(stringsBuffer);
		stringsWriter.u32((uint32_t)strings.list.length());
		for (k = 0; k < strings.list.length(); ++k) {
			String &value = strings.list.index(k);
			stringsWriter.u32((uint32_t)value.length());
			stringsWriter.bytes(value.value(), value.length());
		};

		Buffer sourceBuffer;
		BytecodeWriter source(sourceBuffer);
		source.u32((uint32_t)runCount.size());
		for (k = 0; k < runCount.size(); ++k) {
			source.u32(runCount[k]);
			source.u32(runSource[k]);
			source.u32(runLine[k]);
		};

		Buffer unitsBuffer;
		BytecodeWriter unitsWriter(unitsBuffer);
		bool hasUnits = units && units->begin() != nullptr;
		if (hasUnits) {
			uint32_t unitCount = 0;
			CodeUnitMap::Node *scan;
			for (scan = units->begin(); scan; scan = scan->successor()) {
				++unitCount;
			};
			unitsWriter.u32(unitCount);
			for (scan = units->begin(); scan; scan = scan->successor()) {
				Buffer image;
				String unitError;
				if (!save(*(scan->value.value()), image, unitError, withSource, nullptr)) {
					error = "Unit ";
					error << scan->key << ": " << unitError;
					return false;
				};
				unitsWriter.u32((uint32_t)scan->key.length());
				unitsWriter.bytes(scan->key.value(), scan->key.length());
				unitsWriter.u32((uint32_t)image.length);
				unitsWriter.bytes(image.buffer, image.length);
			};
		};

		struct Section {
				uint32_t type;
				Buffer *data;
		};
		Section sections[4];
		uint32_t sectionCount = 0;
		sections[sectionCount++] = {BytecodeSection::Strings, &stringsBuffer};
		sections[sectionCount++] = {BytecodeSection::Code, &codeBuffer};
		if (withSource) {
			sections[sectionCount++] = {BytecodeSection::Source, &sourceBuffer};
		};
		if (hasUnits) {
			sections[sectionCount++] = {BytecodeSection::Units, &unitsBuffer};
		};

		size_t totalSize = bytecodeHeaderSize + sectionCount * bytecodeSectionEntrySize;
		for (k = 0; k < sectionCount; ++k) {
			totalSize += sections[k].data->length;
		};
		if (totalSize > 0xFFFFFFFFUL) {
			error = "Bytecode too large";
			return false;
		};

		uint16_t major, minor, patch;
		uint32_t build;
		bytecodeEngineVersion(major, minor, patch, build);

		output.allocate(totalSize);
		BytecodeWriter out(output);
		out.bytes(bytecodeMagic, sizeof(bytecodeMagic));
		out.u16(formatVersion);
		out.u16(major);
		out.u16(minor);
		out.u16(patch);
		out.u32(build);
		out.u32(sectionCount);

		uint32_t offset = (uint32_t)(bytecodeHeaderSize + sectionCount * bytecodeSectionEntrySize);
		for (k = 0; k < sectionCount; ++k) {
			out.u32(sections[k].type);
			out.u32(offset);
			out.u32((uint32_t)sections[k].data->length);
			offset += (uint32_t)sections[k].data->length;
		};
		for (k = 0; k < sectionCount; ++k) {
			out.bytes(sections[k].data->buffer, sections[k].data->length);
		};
		return true;
	};

	static bool bytecodeLoad(const uint8_t *data, size_t size, CodeUnit &unit, String &error, CodeUnitMap *units) {
		char buf[256];
		size_t k;

		if (!Bytecode::isBytecode(data, size) || size < bytecodeHeaderSize) {
			error = "Not a bytecode file";
			return false;
		};

		BytecodeReader header(data, size);
		header.bytes(sizeof(bytecodeMagic));
		uint16_t fileFormatVersion = header.u16();
		uint16_t major = header.u16();
		uint16_t minor = header.u16();
		uint16_t patch = header.u16();
		uint32_t build = header.u32();
		uint32_t sectionCount = header.u32();

		if (fileFormatVersion != Bytecode::formatVersion) {
			snprintf(buf, sizeof(buf), "Unsupported bytecode format %u", (unsigned int)fileFormatVersion);
			error = buf;
			return false;
		};

		uint16_t engineMajor, engineMinor, enginePatch;
		uint32_t engineBuild;
		bytecodeEngineVersion(engineMajor, engineMinor, enginePatch, engineBuild);
		if (major != engineMajor) {
			snprintf(buf, sizeof(buf), "Bytecode is for engine version %u.%u.%u.%u, this engine is %u.%u.%u.%u",
			         (unsigned int)major, (unsigned int)minor, (unsigned int)patch, (unsigned int)build,
			         (unsigned int)engineMajor, (unsigned int)engineMinor, (unsigned int)enginePatch, (unsigned int)engineBuild);
			error = buf;
			return false;
		};

		if (sectionCount > bytecodeSectionCountLimit || (size - bytecodeHeaderSize) / bytecodeSectionEntrySize < sectionCount) {
			error = "Invalid section directory";
			return false;
		};

		const uint8_t *stringsData = nullptr;
		size_t stringsSize = 0;
		const uint8_t *codeData = nullptr;
		size_t codeSize = 0;
		const uint8_t *sourceData = nullptr;
		size_t sourceSize = 0;
		const uint8_t *unitsData = nullptr;
		size_t unitsSize = 0;

		for (k = 0; k < sectionCount; ++k) {
			uint32_t type = header.u32();
			uint32_t offset = header.u32();
			uint32_t sectionSize = header.u32();
			if (!header.ok || offset > size || sectionSize > size - offset) {
				error = "Invalid section directory";
				return false;
			};
			const uint8_t **target = nullptr;
			size_t *targetSize = nullptr;
			switch (type) {
			case BytecodeSection::Strings:
				target = &stringsData;
				targetSize = &stringsSize;
				break;
			case BytecodeSection::Code:
				target = &codeData;
				targetSize = &codeSize;
				break;
			case BytecodeSection::Source:
				target = &sourceData;
				targetSize = &sourceSize;
				break;
			case BytecodeSection::Units:
				target = &unitsData;
				targetSize = &unitsSize;
				break;
			default:
				snprintf(buf, sizeof(buf), "Unsupported bytecode section %u", (unsigned int)type);
				error = buf;
				return false;
			};
			if (*target) {
				error = "Duplicate bytecode section";
				return false;
			};
			*target = data + offset;
			*targetSize = sectionSize;
		};

		if (!stringsData || !codeData) {
			error = "Missing bytecode section";
			return false;
		};
		if (unitsData && !units) {
			error = "Bytecode bundle not expected";
			return false;
		};

		// strings
		BytecodeReader stringsReader(stringsData, stringsSize);
		uint32_t stringCount = stringsReader.u32();
		if (!stringsReader.ok || stringCount > stringsSize / 4) {
			error = "Invalid string table";
			return false;
		};
		std::vector<String> strings(stringCount);
		for (k = 0; k < stringCount; ++k) {
			uint32_t length = stringsReader.u32();
			const uint8_t *value = stringsReader.bytes(length);
			if (!value) {
				error = "Invalid string table";
				return false;
			};
			strings[k] = String((const char *)value, length);
		};
		if (!stringsReader.isEnd()) {
			error = "Invalid string table";
			return false;
		};

		// code
		BytecodeReader code(codeData, codeSize);
		uint32_t count = code.u32();
		if (!code.ok || count > codeSize / 2) {
			error = "Invalid code section";
			return false;
		};

		auto readLabel = [&](AsmLabel &label) -> bool {
			label = code.u32();
			return code.ok && label <= count;
		};
		auto readString = [&](uint32_t &index) -> bool {
			index = code.u32();
			return code.ok && index < stringCount;
		};

		std::vector<uint32_t> functions;

		for (k = 1; k <= count; ++k) {
			AsmLabel label = unit.push();
			CodeInstruction &in = unit.at(label);
			uint16_t opcode = code.u16();
			const AsmOpcodeInfo *info = AsmOpcode::getInfo(opcode);
			if (!code.ok || !info || !info->procedure || info->operand == AsmOperand::Native || info->operand == AsmOperand::ProgramCounterRange) {
				error = bytecodeErrorAt("Invalid opcode", k);
				return false;
			};
			in.asmType = opcode;

			bool valid = true;
			uint32_t index;
			switch (info->operand) {
			case AsmOperand::None:
				break;
			case AsmOperand::Symbol:
				valid = readString(index);
				if (valid) {
					in.operand = VariableSymbol::newVariable(Context::getSymbol(strings[index]));
				};
				break;
			case AsmOperand::Index:
				in.operand = VariableSymbol::newVariable(code.u32());
				break;
			case AsmOperand::Boolean: {
				uint8_t value = code.u8();
				valid = (value <= 1);
				in.operand = VariableBoolean::newVariable(value == 1);
			};
				break;
			case AsmOperand::Number:
				in.operand = VariableNumber::newVariable(code.f64());
				break;
			case AsmOperand::String:
				valid = readString(index);
				if (valid) {
					in.operand = VariableString::newVariable(strings[index]);
				};
				break;
			case AsmOperand::IndexLevel: {
				int value = (int)code.u32();
				int level = (int)code.u32();
				in.operand = VariableArgumentLevel::newVariable(value, level);
			};
				break;
			case AsmOperand::ProgramCounter:
				valid = readLabel(in.target);
				break;
			case AsmOperand::Function:
				valid = readLabel(in.target) && readLabel(in.targetRange);
				in.functionHint = (int)code.u32();
				// body after the instruction, not empty
				if (valid) {
					valid = (in.target > label) && (in.targetRange >= in.target);
				};
				functions.push_back(label);
				break;
			case AsmOperand::IndexNumber: {
				VariableOperator21 *operatorX = (VariableOperator21 *)VariableOperator21::newVariable();
				operatorX->symbol = code.u32();
				operatorX->value = code.f64();
				in.operand = operatorX;
			};
				break;
			case AsmOperand::IndexIndex: {
				VariableOperator22 *operatorX = (VariableOperator22 *)VariableOperator22::newVariable();
				operatorX->symbol1 = code.u32();
				operatorX->symbol2 = code.u32();
				in.operand = operatorX;
			};
				break;
			case AsmOperand::IndexNumberGoto:
			case AsmOperand::SymbolNumberGoto: {
				VariableOperator31 *operatorX = (VariableOperator31 *)VariableOperator31::newVariable();
				if (info->operand == AsmOperand::SymbolNumberGoto) {
					valid = readString(index);
					if (valid) {
						operatorX->symbol = Context::getSymbol(strings[index]);
					};
				} else {
					operatorX->symbol = code.u32();
				};
				operatorX->value = code.f64();
				operatorX->pc = nullptr;
				in.operand = operatorX;
				valid = valid && readLabel(in.target);
			};
				break;
			default:
				valid = false;
				break;
			};

			if (!valid || !code.ok) {
				error = bytecodeErrorAt("Invalid instruction", k);
				return false;
			};
		};
		if (!code.isEnd()) {
			error = "Invalid code section";
			return false;
		};

		// function bodies are extracted at runtime: bodies must nest, and a function
		// instruction must be in the same body as its own body starts
		std::vector<uint32_t> byBegin(functions);
		std::sort(byBegin.begin(), byBegin.end(), [&](uint32_t a, uint32_t b) {
			if (unit.at(a).target != unit.at(b).target) {
				return unit.at(a).target < unit.at(b).target;
			};
			return unit.at(a).targetRange > unit.at(b).targetRange;
		});
		std::vector<uint32_t> parentOfBody(count + 1, 0);
		std::vector<uint32_t> parentOfInstruction(count + 1, 0);
		std::vector<uint32_t> open;
		size_t nextFunction = 0;
		for (AsmLabel position = 1; position <= count; ++position) {
			while (!open.empty() && unit.at(open.back()).targetRange < position) {
				open.pop_back();
			};
			while (nextFunction < byBegin.size() && unit.at(byBegin[nextFunction]).target == position) {
				uint32_t function = byBegin[nextFunction++];
				if (!open.empty() && unit.at(function).targetRange > unit.at(open.back()).targetRange) {
					error = bytecodeErrorAt("Function body overlaps", function);
					return false;
				};
				parentOfBody[function] = open.empty() ? 0 : open.back();
				open.push_back(function);
			};
			if (AsmOpcode::getInfo(unit.at(position).asmType)->operand == AsmOperand::Function) {
				parentOfInstruction[position] = open.empty() ? 0 : open.back();
			};
		};
		for (uint32_t function : functions) {
			if (parentOfBody[function] != parentOfInstruction[function]) {
				error = bytecodeErrorAt("Function body outside of its scope", function);
				return false;
			};
		};

		// source, optional
		if (sourceData) {
			BytecodeReader source(sourceData, sourceSize);
			uint32_t runs = source.u32();
			if (!source.ok || runs > sourceSize / 12) {
				error = "Invalid source section";
				return false;
			};
			AsmLabel label = 1;
			for (k = 0; k < runs; ++k) {
				uint32_t runLength = source.u32();
				uint32_t index = source.u32();
				uint32_t line = source.u32();
				if (!source.ok || index > stringCount || runLength == 0 || runLength > count - (label - 1)) {
					error = "Invalid source section";
					return false;
				};
				Symbol sourceSymbol = index ? Context::getSymbol(strings[index - 1]) : 0;
				for (; runLength > 0; --runLength, ++label) {
					unit.at(label).sourceSymbol = sourceSymbol;
					unit.at(label).sourceLineNumber = line;
				};
			};
			if (!source.isEnd() || label != count + 1) {
				error = "Invalid source section";
				return false;
			};
		};

		// named units of a bundle
		if (unitsData) {
			BytecodeReader unitsReader(unitsData, unitsSize);
			uint32_t unitCount = unitsReader.u32();
			if (!unitsReader.ok || unitCount > unitsSize / 8) {
				error = "Invalid units section";
				return false;
			};
			CodeUnitMap loaded;
			for (k = 0; k < unitCount; ++k) {
				uint32_t nameLength = unitsReader.u32();
				const uint8_t *name = unitsReader.bytes(nameLength);
				uint32_t imageSize = unitsReader.u32();
				const uint8_t *image = unitsReader.bytes(imageSize);
				if (!name || !image) {
					error = "Invalid units section";
					return false;
				};
				String unitName((const char *)name, nameLength);
				TPointer<CodeUnit> namedUnit;
				if (loaded.get(unitName, namedUnit)) {
					error = "Duplicate unit ";
					error << unitName;
					return false;
				};
				namedUnit.newMemory();
				String unitError;
				if (!Bytecode::load(image, imageSize, *(namedUnit.value()), unitError, nullptr)) {
					error = "Unit ";
					error << unitName << ": " << unitError;
					return false;
				};
				loaded.set(unitName, namedUnit);
			};
			if (!unitsReader.isEnd()) {
				error = "Invalid units section";
				return false;
			};
			CodeUnitMap::Node *scan;
			for (scan = loaded.begin(); scan; scan = scan->successor()) {
				units->set(scan->key, scan->value);
			};
		};

		return true;
	};

	bool Bytecode::load(const uint8_t *data, size_t size, CodeUnit &unit, String &error, CodeUnitMap *units) {
		unit.empty();
		if (bytecodeLoad(data, size, unit, error, units)) {
			return true;
		};
		unit.empty();
		return false;
	};

	bool Bytecode::saveFile(CodeUnit &unit, const char *fileName, String &error, bool withSource, CodeUnitMap *units) {
		Buffer output;
		if (!save(unit, output, error, withSource, units)) {
			return false;
		};
		if (!Shell::filePutContents(fileName, output)) {
			error = "Unable to write file";
			return false;
		};
		return true;
	};

	bool Bytecode::loadFile(const char *fileName, CodeUnit &unit, String &error, CodeUnitMap *units) {
		Buffer input;
		if (!Shell::fileGetContents(fileName, input)) {
			error = "Unable to read file";
			return false;
		};
		return load(input.buffer, input.length, unit, error, units);
	};

	bool Bytecode::isBytecodeFile(const char *fileName) {
		File in;
		uint8_t magic[sizeof(bytecodeMagic)];
		if (!in.openRead(fileName)) {
			return false;
		};
		size_t size = in.read(magic, sizeof(magic));
		in.close();
		return isBytecode(magic, size);
	};

};
