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

#define QUANTUM_SCRIPT_VERSION_ABCD                1,6,0,12
#define QUANTUM_SCRIPT_VERSION_STR                 "1.6.0"
#define QUANTUM_SCRIPT_VERSION_STR_BUILD           "12"
#define QUANTUM_SCRIPT_VERSION_STR_DATETIME        "2021-07-06 15:11:44"

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

