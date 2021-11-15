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

#define QUANTUM_SCRIPT_VERSION_ABCD                2,1,0,44
#define QUANTUM_SCRIPT_VERSION_STR                 "2.1.0"
#define QUANTUM_SCRIPT_VERSION_STR_BUILD           "44"
#define QUANTUM_SCRIPT_VERSION_STR_DATETIME        "2021-11-15 16:51:48"

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

