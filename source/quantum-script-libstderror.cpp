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

#include "quantum-script-libstderror.hpp"

#include "quantum-script-libstderror.src"

//#define QUANTUM_SCRIPT_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {

		namespace LibStdError {

			void initExecutive(Executive *executive, void *extensionId) {
				executive->compileStringX(libStdErrorSource);
			};


		};
	};
};


