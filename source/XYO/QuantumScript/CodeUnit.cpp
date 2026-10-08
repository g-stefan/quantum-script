// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript/CodeUnit.hpp>

namespace XYO::QuantumScript {

	CodeUnit::CodeUnit() {
	};

	AsmLabel CodeUnit::previous(AsmLabel label) {
		for (--label; label > 0; --label) {
			if (!at(label).isRemoved) {
				return label;
			};
		};
		return 0;
	};

	AsmLabel CodeUnit::push() {
		AsmLabel label = (AsmLabel)(instruction.length() + 1);
		at(label).reset();
		return label;
	};

	void CodeUnit::pop() {
		size_t length_ = instruction.length();
		while (length_ > 0) {
			--length_;
			CodeInstruction &last = instruction.index(length_);
			bool isRemoved = last.isRemoved;
			last.reset();
			instruction.setLength(length_);
			if (!isRemoved) {
				return;
			};
		};
	};

	void CodeUnit::truncate(AsmLabel label) {
		if (label == 0) {
			return;
		};
		size_t newLength = label - 1;
		for (size_t k = newLength; k < instruction.length(); ++k) {
			instruction.index(k).reset();
		};
		if (newLength < instruction.length()) {
			instruction.setLength(newLength);
		};
	};

	void CodeUnit::empty() {
		instruction.empty();
	};

	AsmLabel CodeUnit::append(CodeUnit &unit) {
		AsmLabel offset = lastLabel();
		size_t k;
		for (k = 0; k < unit.length(); ++k) {
			CodeInstruction &in = unit.instruction.index(k);
			CodeInstruction &out = at(push());
			out.asmType = in.asmType;
			out.operand = in.operand;
			out.target = in.target ? in.target + offset : 0;
			out.targetRange = in.targetRange ? in.targetRange + offset : 0;
			out.targetExternal = in.targetExternal;
			out.functionHint = in.functionHint;
			out.sourceSymbol = in.sourceSymbol;
			out.sourceLineNumber = in.sourceLineNumber;
			out.isRemoved = in.isRemoved;
		};
		return unit.length() ? offset + 1 : 0;
	};

};
