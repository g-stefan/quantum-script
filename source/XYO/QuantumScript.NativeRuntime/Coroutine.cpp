// Quantum Script Native Runtime
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.NativeRuntime/Coroutine.hpp>

namespace XYO::QuantumScript::NativeRuntime {

	CoroutineFrame::CoroutineFrame() {
		variable.pointerLink(this);
		iterator = nullptr;
		value = nullptr;
		iteratorCount = 0;
		valueCount = 0;
	};

	CoroutineFrame::~CoroutineFrame() {
		delete[] iterator;
		delete[] value;
	};

	void CoroutineFrame::setSize(size_t iteratorCount_, size_t valueCount_) {
		delete[] iterator;
		delete[] value;
		iteratorCount = iteratorCount_;
		valueCount = valueCount_;
		iterator = new TPointer<Iterator>[iteratorCount + 1];
		value = new int[valueCount + 1];
		reset();
	};

	void CoroutineFrame::reset() {
		size_t k;
		variable = VariableArray::newArray();
		for (k = 0; k < iteratorCount; ++k) {
			iterator[k].deleteMemory();
		};
		for (k = 0; k < valueCount; ++k) {
			value[k] = 0;
		};
	};

	TPointer<Variable> newFunctionWithYield(Module *module, FunctionParent *parent, VariableArray *localVariables, VariableArray *arguments, FunctionProcedureWithYield procedure, size_t iteratorCount, size_t valueCount) {
		TPointer<CoroutineFrame> frame;
		frame.newMemory();
		frame->module = module;
		frame->setSize(iteratorCount, valueCount);
		return VariableFunctionWithYield::newVariable(parent, localVariables, arguments, procedure, frame, module);
	};

};
