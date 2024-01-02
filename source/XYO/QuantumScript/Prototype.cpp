// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <XYO/QuantumScript/Prototype.hpp>
#include <XYO/QuantumScript/VariableObject.hpp>

namespace XYO::QuantumScript {

	Prototype::Prototype() {
		parent.pointerLink(this);
		prototype.pointerLink(this);
	};

};
