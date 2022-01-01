//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "quantum-script-prototype.hpp"
#include "quantum-script-variableobject.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		Prototype::Prototype() {
			parent.pointerLink(this);
			prototype.pointerLink(this);
		};


	};
};


