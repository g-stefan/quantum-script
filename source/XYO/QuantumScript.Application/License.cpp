// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Application/License.hpp>

namespace XYO::QuantumScript::Application::License {

	const char *license() {
		return XYO::QuantumScript::License::license();
	};

	const char *shortLicense() {
		return XYO::QuantumScript::License::shortLicense();
	};

};
