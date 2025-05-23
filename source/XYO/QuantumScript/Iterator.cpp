// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/Iterator.hpp>
#include <XYO/QuantumScript/Context.hpp>

namespace XYO::QuantumScript {

	bool Iterator::next(Variable *out) {
		out->referenceSet(Context::getValueUndefined());
		return false;
	};

};
