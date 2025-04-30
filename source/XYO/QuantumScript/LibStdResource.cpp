// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/LibStdResource.hpp>
#include <XYO/QuantumScript/VariableResource.hpp>

// #define XYO_QUANTUMSCRIPT_DEBUG_RUNTIME

namespace XYO::QuantumScript {

	namespace LibStdResource {

		static TPointer<Variable> drop(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- resource-drop\n");
#endif

			if (!TIsType<VariableResource>(this_)) {
				throw(Error("invalid parameter"));
			};

			((VariableResource *)(this_))->drop();
			return Context::getValueUndefined();
			;
		};

		static TPointer<Variable> close(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- resource-close\n");
#endif

			if (!TIsType<VariableResource>(this_)) {
				throw(Error("invalid parameter"));
			};

			((VariableResource *)(this_))->close();
			return Context::getValueUndefined();
			;
		};

		void initExecutive(Executive *executive, void *extensionId) {
			executive->setFunction2("Resource.prototype.close()", close);
			executive->setFunction2("Resource.prototype.drop()", drop);
		};

	};
};
