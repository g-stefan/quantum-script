// Quantum Script Native Runtime
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.NativeRuntime/Module.hpp>

namespace XYO::QuantumScript::NativeRuntime {

	Module::Module() {
		info = nullptr;
		executive = nullptr;
		global = nullptr;
		undefined = nullptr;
		valueTrue = nullptr;
		valueFalse = nullptr;
		symbol = nullptr;
		string = nullptr;
		number = nullptr;
		source = nullptr;
		symbolMessage = 0;
		symbolStackTrace = 0;
		symbolError = 0;
		string_ = nullptr;
		number_ = nullptr;
	};

	Module::~Module() {
		delete[] symbol;
		delete[] string;
		delete[] number;
		delete[] source;
		delete[] string_;
		delete[] number_;
	};

	TPointer<Module> Module::newModule(const ModuleInfo &info) {
		TPointer<Module> retV;
		size_t k;

		retV.newMemory();
		retV->info = &info;
		retV->executive = &(ExecutiveX::getExecutive());

		retV->global = (Context::getGlobalObject()).value();
		retV->undefined = (Context::getValueUndefined()).value();
		retV->valueTrue = Context::getValueBoolean(true);
		retV->valueFalse = Context::getValueBoolean(false);

		retV->symbol = new Symbol[info.symbolCount + 1];
		for (k = 0; k < info.symbolCount; ++k) {
			retV->symbol[k] = Context::getSymbol(info.symbol[k]);
		};

		retV->string_ = new TPointer<Variable>[info.stringCount + 1];
		retV->string = new Variable *[info.stringCount + 1];
		for (k = 0; k < info.stringCount; ++k) {
			retV->string_[k] = VariableString::newVariable(String(info.string[k].value, info.string[k].length));
			retV->string[k] = retV->string_[k].value();
		};

		retV->number_ = new TPointer<Variable>[info.numberCount + 1];
		retV->number = new Variable *[info.numberCount + 1];
		for (k = 0; k < info.numberCount; ++k) {
			retV->number_[k] = VariableNumber::newVariable(info.number[k]);
			retV->number[k] = retV->number_[k].value();
		};

		retV->source = new Symbol[info.sourceCount + 1];
		for (k = 0; k < info.sourceCount; ++k) {
			retV->source[k] = Context::getSymbol(info.source[k]);
		};

		retV->symbolMessage = Context::getSymbol("message");
		retV->symbolStackTrace = Context::getSymbol("stackTrace");
		retV->symbolError = Context::getSymbol("Error");

		retV->iteratorValue = VariableArray::newArray();
		retV->iteratorReference = VariableReferenceIndex::newVariable(retV->iteratorValue, 0);

		return retV;
	};

};
