// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_APPLICATION_COPYRIGHT_HPP
#define XYO_QUANTUMSCRIPT_APPLICATION_COPYRIGHT_HPP

#ifndef XYO_QUANTUMSCRIPT_HPP
#	include <XYO/QuantumScript.hpp>
#endif

namespace XYO::QuantumScript::Application::Copyright {
	const char *copyright();
	const char *publisher();
	const char *company();
	const char *contact();
	const char *fullCopyright();
};

#endif