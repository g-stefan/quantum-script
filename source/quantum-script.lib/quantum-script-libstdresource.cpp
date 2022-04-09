//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-libstdresource.hpp"
#include "quantum-script-variableresource.hpp"

//#define QUANTUM_SCRIPT_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {

		namespace LibStdResource {

			static TPointer<Variable> drop(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
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
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
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
};
