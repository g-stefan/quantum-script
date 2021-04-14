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

			static const char *version_ = "1.5.0";
			static const char *build_ = "9";
			static const char *versionWithBuild_ = "1.5.0.9";
			static const char *datetime_ = "2021-03-15 12:30:20";

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
