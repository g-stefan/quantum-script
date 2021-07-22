//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-libstdarray.hpp"
#include "quantum-script-variablearray.hpp"
#include "quantum-script-variablestring.hpp"

#include "quantum-script-libstdarray.src"

//#define QUANTUM_SCRIPT_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {

		namespace LibStdArray {

			static TPointer<Variable> arrayJoin(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- array-join\n");
#endif
				if(TIsType<VariableArray>(this_)) {
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
};


