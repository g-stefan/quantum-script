//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VERSION_HPP
#define QUANTUM_SCRIPT_VERSION_HPP

#define QUANTUM_SCRIPT_VERSION_ABCD 2,7,0,36
#define QUANTUM_SCRIPT_VERSION_STR "2.7.0"
#define QUANTUM_SCRIPT_VERSION_STR_BUILD "36"
#define QUANTUM_SCRIPT_VERSION_STR_DATETIME "2022-04-04 01:44:48"

#ifndef XYO_RC

#	ifndef QUANTUM_SCRIPT__EXPORT_HPP
#		include "quantum-script--export.hpp"
#	endif

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
