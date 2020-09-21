//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VERSION_HPP
#define QUANTUM_SCRIPT_VERSION_HPP

#define QUANTUM_SCRIPT_VERSION_ABCD                1,2,0,4
#define QUANTUM_SCRIPT_VERSION_STR                 "1.2.0"
#define QUANTUM_SCRIPT_VERSION_STR_BUILD           "4"
#define QUANTUM_SCRIPT_VERSION_STR_DATETIME        "2020-09-21 17:38:57"

#ifndef XYO_RC

namespace Quantum {
	namespace Script {
		namespace Version {
			const char *version();
			const char *build();
			const char *versionWithBuild();
			const char *datetime();
		};
	};
};

#endif
#endif

