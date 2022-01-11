//
// Quantum Script Extension Console
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include "quantum-script-extension-console-version.hpp"

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Console {
				namespace Version {

					static const char *version_ = "2.4.0";
					static const char *build_ = "31";
					static const char *versionWithBuild_ = "2.4.0.31";
					static const char *datetime_ = "2022-01-09 01:13:05";

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
	};
};



