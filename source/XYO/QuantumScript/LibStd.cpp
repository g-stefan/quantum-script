// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/LibStd.hpp>
#include <XYO/QuantumScript/LibStdScript.hpp>
#include <XYO/QuantumScript/LibStdError.hpp>
#include <XYO/QuantumScript/LibStdFunction.hpp>
#include <XYO/QuantumScript/LibStdObject.hpp>
#include <XYO/QuantumScript/LibStdConvert.hpp>
#include <XYO/QuantumScript/LibStdString.hpp>
#include <XYO/QuantumScript/LibStdArray.hpp>

namespace XYO::QuantumScript {

	namespace LibStd {

		void initExecutive(Executive *executive, void *extensionId) {
#ifndef XYO_QUANTUMSCRIPT_DISABLE_INIT_EXECUTIVE
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
