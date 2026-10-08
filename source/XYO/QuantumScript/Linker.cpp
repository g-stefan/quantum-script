// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <map>
#include <vector>

#include <XYO/QuantumScript/Linker.hpp>
#include <XYO/QuantumScript/VariableVmProgramCounter.hpp>
#include <XYO/QuantumScript/VariableVmFunction.hpp>
#include <XYO/QuantumScript/VariableOperator23.hpp>
#include <XYO/QuantumScript/VariableOperator31.hpp>
#include <XYO/QuantumScript/VariableArgumentLevel.hpp>

namespace XYO::QuantumScript {

	Linker::Linker() {
	};

	ProgramCounter *Linker::getProgramCounter(AsmLabel label) {
		if (label == 0 || label > address.length()) {
			return nullptr;
		};
		return address.index(label - 1);
	};

	void Linker::link(CodeUnit &unit, InstructionList &instructionList) {
		size_t length = unit.length();
		size_t k;

		address.empty();

		// create the instructions
		for (k = 0; k < length; ++k) {
			CodeInstruction &in = unit.instruction.index(k);
			if (in.isRemoved) {
				address.index(k) = nullptr;
				continue;
			};
			instructionList.pushToTail();
			InstructionList::Node *node = instructionList.tail;
			node->value.asmType = in.asmType;
			node->value.procedure = AsmOpcode::getInfo(in.asmType)->procedure;
			node->value.operand = in.operand;
			node->value.sourceSymbol = in.sourceSymbol;
			node->value.sourceLineNumber = in.sourceLineNumber;
			address.index(k) = reinterpret_cast<ProgramCounter *>(node);
		};

		// removed instructions resolve to the next instruction
		ProgramCounter *next = nullptr;
		for (k = length; k > 0; --k) {
			if (unit.instruction.index(k - 1).isRemoved) {
				address.index(k - 1) = next;
				continue;
			};
			next = address.index(k - 1);
		};

		// jump operands
		for (k = 0; k < length; ++k) {
			CodeInstruction &in = unit.instruction.index(k);
			if (in.isRemoved) {
				continue;
			};
			InstructionList::Node *node = reinterpret_cast<InstructionList::Node *>(address.index(k));
			ProgramCounter *target = in.targetExternal ? in.targetExternal : getProgramCounter(in.target);

			switch (AsmOpcode::getInfo(in.asmType)->operand) {
			case AsmOperand::ProgramCounter:
				node->value.operand = VariableVmProgramCounter::newVariable(target);
				break;
			case AsmOperand::Function: {
				VariableVmFunction *function = (VariableVmFunction *)VariableVmFunction::newVariable(target);
				function->valueEnd = getProgramCounter(in.targetRange);
				function->functionHint = in.functionHint;
				node->value.operand = function;
			};
				break;
			case AsmOperand::ProgramCounterRange: {
				VariableOperator23 *range = (VariableOperator23 *)VariableOperator23::newVariable();
				range->linkBegin = getProgramCounter(in.targetRange);
				range->pc = target;
				node->value.operand = range;
			};
				break;
			case AsmOperand::IndexNumberGoto:
			case AsmOperand::SymbolNumberGoto: {
				VariableOperator31 *operatorX = (VariableOperator31 *)VariableOperator31::newVariable();
				operatorX->symbol = ((VariableOperator31 *)in.operand.value())->symbol;
				operatorX->value = ((VariableOperator31 *)in.operand.value())->value;
				operatorX->pc = target;
				node->value.operand = operatorX;
			};
				break;
			default:
				break;
			};
		};
	};

	// Unlink: live instructions of a script function back to a unit
	class LinkerUnlink {
		public:
			CodeUnit &unit;
			String &error;

			// label of each unlinked instruction
			std::map<const void *, AsmLabel> label;

			class Fixup {
				public:
					AsmLabel at;
					bool isRange; // targetRange, else target
					ProgramCounter *target;
			};
			std::vector<Fixup> fixup;

			inline LinkerUnlink(CodeUnit &unit_, String &error_) : unit(unit_), error(error_){};

			inline void addFixup(AsmLabel at, bool isRange, ProgramCounter *target) {
				Fixup value;
				value.at = at;
				value.isRange = isRange;
				value.target = target;
				fixup.push_back(value);
			};

			// depth: 0 is the function, 1 a function inside it, ...
			bool emitRange(InstructionList::Node *begin, InstructionList::Node *end, size_t depth) {
				InstructionList::Node *node = begin;
				// ends of the bodies of nested functions that are still in the list
				std::vector<InstructionList::Node *> inlineEnd;
				for (;;) {
					if (!node) {
						error = "Invalid function code";
						return false;
					};
					size_t depthHere = depth + inlineEnd.size();
					AsmLabel at = emitInstruction(node);
					if (!at) {
						return false;
					};
					CodeInstruction &in = unit.at(at);
					Variable *operand = node->value.operand.value();
					switch (AsmOpcode::getInfo(in.asmType)->operand) {
					case AsmOperand::Function: {
						VariableVmFunction *inner = (VariableVmFunction *)operand;
						if (!inner->originalValue || !inner->valueEnd) {
							error = "Invalid function code";
							return false;
						};
						in.functionHint = inner->functionHint;
						addFixup(at, false, inner->originalValue);
						addFixup(at, true, inner->valueEnd);
						if (!inner->instructionList) {
							// body follows in the list
							inlineEnd.push_back(reinterpret_cast<InstructionList::Node *>(inner->valueEnd));
							break;
						};
						// body extracted at runtime, emit it after the goto that skips it
						InstructionList::Node *skip = node->next;
						if (node == end || !skip || skip->value.asmType != ParserAsm::Goto) {
							error = "Invalid function code";
							return false;
						};
						if (!emitInstruction(skip)) {
							return false;
						};
						if (!emitRange(reinterpret_cast<InstructionList::Node *>(inner->originalValue),
						               reinterpret_cast<InstructionList::Node *>(inner->valueEnd), depthHere + 1)) {
							return false;
						};
						node = skip;
					};
						break;
					case AsmOperand::IndexLevel:
						if ((size_t)(((VariableArgumentLevel *)operand)->level) > depthHere) {
							error = "Function uses variables of an enclosing function";
							return false;
						};
						break;
					default:
						break;
					};
					while (!inlineEnd.empty() && inlineEnd.back() == node) {
						inlineEnd.pop_back();
					};
					if (node == end) {
						break;
					};
					node = node->next;
				};
				return true;
			};

			// copy one instruction, jump targets are added as fixups
			AsmLabel emitInstruction(InstructionList::Node *node) {
				const AsmOpcodeInfo *info = AsmOpcode::getInfo(node->value.asmType);
				if (!info || !info->procedure) {
					error = "Invalid function code";
					return 0;
				};
				Variable *operand = node->value.operand.value();
				AsmLabel at = unit.push();
				label[node] = at;
				CodeInstruction &in = unit.at(at);
				in.asmType = node->value.asmType;
				in.sourceSymbol = node->value.sourceSymbol;
				in.sourceLineNumber = node->value.sourceLineNumber;
				switch (info->operand) {
				case AsmOperand::None:
				case AsmOperand::Function:
					break;
				case AsmOperand::Native:
					error = "Function uses native code";
					return 0;
				case AsmOperand::ProgramCounterRange:
					error = "Invalid function code";
					return 0;
				case AsmOperand::ProgramCounter:
					addFixup(at, false, ((VariableVmProgramCounter *)operand)->value);
					break;
				case AsmOperand::IndexNumberGoto:
				case AsmOperand::SymbolNumberGoto: {
					VariableOperator31 *operatorX = (VariableOperator31 *)VariableOperator31::newVariable();
					operatorX->symbol = ((VariableOperator31 *)operand)->symbol;
					operatorX->value = ((VariableOperator31 *)operand)->value;
					operatorX->pc = nullptr;
					in.operand = operatorX;
					addFixup(at, false, ((VariableOperator31 *)operand)->pc);
				};
					break;
				default:
					in.operand = operand;
					break;
				};
				return at;
			};

			bool resolve() {
				for (Fixup &value : fixup) {
					if (!value.target) {
						continue;
					};
					auto it = label.find(value.target);
					if (it == label.end()) {
						error = "Function jumps outside of its code";
						return false;
					};
					if (value.isRange) {
						unit.at(value.at).targetRange = it->second;
					} else {
						unit.at(value.at).target = it->second;
					};
				};
				return true;
			};
	};

	bool Linker::unlinkFunction(Variable *function, CodeUnit &unit, String &error) {
		unit.empty();
		if (!TIsType<VariableVmFunction>(function)) {
			error = "Not a script function";
			return false;
		};
		VariableVmFunction *vmFunction = (VariableVmFunction *)function;
		// value can change for a coroutine, originalValue is the begin
		if (!vmFunction->originalValue || !vmFunction->valueEnd) {
			error = "Function without code";
			return false;
		};
		LinkerUnlink unlink(unit, error);
		if (unlink.emitRange(reinterpret_cast<InstructionList::Node *>(vmFunction->originalValue),
		                     reinterpret_cast<InstructionList::Node *>(vmFunction->valueEnd), 0) &&
		    unlink.resolve()) {
			return true;
		};
		unit.empty();
		return false;
	};

};
