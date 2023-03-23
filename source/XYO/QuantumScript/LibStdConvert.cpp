// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/LibStdConvert.hpp>

#include <XYO/QuantumScript/VariableBoolean.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>
#include <XYO/QuantumScript/VariableString.hpp>

// #define XYO_QUANTUMSCRIPT_DEBUG_RUNTIME

namespace XYO::QuantumScript {

	namespace LibStdConvert {

		static TPointer<Variable> toBoolean(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- convert-to-boolean\n");
#endif
			return VariableBoolean::newVariable((arguments->index(0))->toBoolean());
		};

		static TPointer<Variable> toNumber(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- convert-to-number\n");
#endif
			return VariableNumber::newVariable((arguments->index(0))->toNumber());
		};

		static TPointer<Variable> toString(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- convert-to-string\n");
#endif
			return VariableString::newVariable((arguments->index(0))->toString());
		};

		static TPointer<Variable> toStringSource(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- convert-to-string-source\n");
#endif
			return VariableString::newVariable(StringX::encodeC((arguments->index(0))->toString()));
		};

		static TPointer<Variable> numberFormat(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- convert-number-format\n");
#endif
			Number x = (arguments->index(0))->toNumber();
			size_t fSize = (arguments->index(1))->toIndex();

			char buf[128];
			char req[128];

			sprintf(req, "%%." XYO_FORMAT_SIZET "lf", fSize);
			sprintf(buf, req, x);

			return VariableString::newVariable(buf);
		};

		void initExecutive(Executive *executive, void *extensionId) {
			executive->compileStringX("var Convert={};");

			executive->setFunction2("Convert.toBoolean(x)", toBoolean);
			executive->setFunction2("Convert.toNumber(x)", toNumber);
			executive->setFunction2("Convert.toString(x)", toString);
			executive->setFunction2("Convert.toStringSource(x)", toStringSource);
			executive->setFunction2("Convert.numberFormat(x,fSize)", numberFormat);
		};

	};
};
