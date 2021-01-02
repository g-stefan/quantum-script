//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VERSION_HPP
#define QUANTUM_SCRIPT_VERSION_HPP

#define QUANTUM_SCRIPT_VERSION_ABCD                1,3,0,6
#define QUANTUM_SCRIPT_VERSION_STR                 "1.3.0"
#define QUANTUM_SCRIPT_VERSION_STR_BUILD           "6"
#define QUANTUM_SCRIPT_VERSION_STR_DATETIME        "2020-12-01 04:52:40"

#ifndef XYO_RC

#ifndef QUANTUM_SCRIPT__EXPORT_HPP
#include "quantum-script--export.hpp"
#endif


namespace Quantum {
	namespace Script {
		namespace Version {
			QUANTUM_SCRIPT_EXPORT const char *version();
			QUANTUM_SCRIPT_EXPORT const char *build();
			QUANTUM_SCRIPT_EXPORT const char *versionWithBuild();
			QUANTUM_SCRIPT_EXPORT const char *datetime();
		};
	};
};


#endif
#endif

