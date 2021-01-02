//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-iterator.hpp"
#include "quantum-script-context.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		bool Iterator::next(TPointerX<Variable> &out) {
			out = Context::getValueUndefined();
			return false;
		};

	};
};


