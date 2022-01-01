//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_COPYRIGHT_HPP
#define QUANTUM_SCRIPT_COPYRIGHT_HPP

#define QUANTUM_SCRIPT_COPYRIGHT            "Copyright (c) Grigore Stefan"
#define QUANTUM_SCRIPT_PUBLISHER            "Grigore Stefan"
#define QUANTUM_SCRIPT_COMPANY              QUANTUM_SCRIPT_PUBLISHER
#define QUANTUM_SCRIPT_CONTACT              "g_stefan@yahoo.com"
#define QUANTUM_SCRIPT_FULL_COPYRIGHT       QUANTUM_SCRIPT_COPYRIGHT " <" QUANTUM_SCRIPT_CONTACT ">"

#ifndef XYO_RC

#ifndef QUANTUM_SCRIPT__EXPORT_HPP
#include "quantum-script--export.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Copyright {
			QUANTUM_SCRIPT_EXPORT const char *copyright();
			QUANTUM_SCRIPT_EXPORT const char *publisher();
			QUANTUM_SCRIPT_EXPORT const char *company();
			QUANTUM_SCRIPT_EXPORT const char *contact();
			QUANTUM_SCRIPT_EXPORT const char *fullCopyright();
		};
	};
};

#endif
#endif
