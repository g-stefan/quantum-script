//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_LIBSTDOBJECT_HPP
#define QUANTUM_SCRIPT_LIBSTDOBJECT_HPP

#ifndef QUANTUM_SCRIPT_EXECUTIVE_HPP
#include "quantum-script-executive.hpp"
#endif

namespace Quantum {
	namespace Script {

		namespace LibStdObject {

			QUANTUM_SCRIPT_EXPORT void initExecutive(Executive *executive, void *extensionId);

		};
	};
};


#endif
