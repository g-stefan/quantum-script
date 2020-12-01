//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include "quantum-script-version.hpp"

namespace Quantum {
	namespace Script {
		namespace Version {

			static const char *version_ = "1.2.0";
			static const char *build_ = "6";
			static const char *versionWithBuild_ = "1.2.0.6";
			static const char *datetime_ = "2020-12-01 04:52:40";

			const char *version() {
				return version_;
			};
			const char *build() {
				return build_;
			};
			const char *versionWithBuild() {
				return versionWithBuild_;
			};
			const char *datetime() {
				return datetime_;
			};

		};
	};
};
