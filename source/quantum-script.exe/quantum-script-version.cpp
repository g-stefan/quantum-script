//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include "quantum-script-version.hpp"

namespace Quantum {
	namespace Script {
		namespace Version {

			static const char *version_ = "2.0.0";
			static const char *build_ = "16";
			static const char *versionWithBuild_ = "2.0.0.16";
			static const char *datetime_ = "2021-07-23 13:43:40";

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
