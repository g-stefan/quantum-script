// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_LIBSTD_HPP
#define XYO_QUANTUMSCRIPT_LIBSTD_HPP

#ifndef XYO_QUANTUMSCRIPT_EXECUTIVE_HPP
#	include <XYO/QuantumScript/Executive.hpp>
#endif

namespace XYO::QuantumScript {

	namespace LibStd {

		XYO_QUANTUMSCRIPT_EXPORT void initExecutive(Executive *executive, void *extensionId);

	};
};

#endif