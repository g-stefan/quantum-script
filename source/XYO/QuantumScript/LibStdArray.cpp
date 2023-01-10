// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript/LibStdArray.hpp>
#include <XYO/QuantumScript/VariableArray.hpp>
#include <XYO/QuantumScript/VariableString.hpp>

#include <XYO/QuantumScript/LibStdArray.Source.cpp>

// #define XYO_QUANTUMSCRIPT_DEBUG_RUNTIME

namespace XYO::QuantumScript {

	namespace LibStdArray {

		static TPointer<Variable> arrayJoin(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- array-join\n");
#endif
			if (TIsType<VariableArray>(this_)) {
				return VariableString::newVariable((static_cast<VariableArray *>(this_))->join((arguments->index(0))->toString()));
			};
			return Context::getValueUndefined();
		};

		void initExecutive(Executive *executive, void *extensionId) {

			executive->setFunction2("Array.prototype.join(text)", arrayJoin);

			executive->compileStringX(libStdArraySource);
		};

	};
};
