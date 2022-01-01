//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-libstd.hpp"
#include "quantum-script-libstdscript.hpp"
#include "quantum-script-libstderror.hpp"
#include "quantum-script-libstdfunction.hpp"
#include "quantum-script-libstdobject.hpp"
#include "quantum-script-libstdconvert.hpp"
#include "quantum-script-libstdstring.hpp"
#include "quantum-script-libstdarray.hpp"

namespace Quantum {
	namespace Script {

		namespace LibStd {

			void initExecutive(Executive *executive, void *extensionId) {

#ifndef QUANTUM_SCRIPT_DISABLE_INIT_EXECUTIVE
				executive->initExtension(LibStdScript::initExecutive);
				executive->initExtension(LibStdError::initExecutive);
				executive->initExtension(LibStdFunction::initExecutive);
				executive->initExtension(LibStdObject::initExecutive);
				executive->initExtension(LibStdConvert::initExecutive);
				executive->initExtension(LibStdString::initExecutive);
				executive->initExtension(LibStdArray::initExecutive);
#endif

			};

		};
	};
};

