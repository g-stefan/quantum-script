//
// Quantum Script Extension Console
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_CONSOLE_HPP
#define QUANTUM_SCRIPT_EXTENSION_CONSOLE_HPP

#ifndef QUANTUM_SCRIPT_HPP
#	include "quantum-script.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_CONSOLE__EXPORT_HPP
#	include "quantum-script-extension-console--export.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_CONSOLE_COPYRIGHT_HPP
#	include "quantum-script-extension-console-copyright.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_CONSOLE_LICENSE_HPP
#	include "quantum-script-extension-console-license.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_CONSOLE_VERSION_HPP
#	include "quantum-script-extension-console-version.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Console {

				using namespace Quantum::Script;

				QUANTUM_SCRIPT_EXTENSION_CONSOLE_EXPORT void initExecutive(Executive *executive, void *extensionId);
				QUANTUM_SCRIPT_EXTENSION_CONSOLE_EXPORT void registerInternalExtension(Executive *executive);

			};
		};
	};
};

#endif
