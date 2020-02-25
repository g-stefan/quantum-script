//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-parser.hpp"
#include "quantum-script-parserasm.hpp"

namespace Quantum {
	namespace Script {


		bool Parser::statementThrow() {
			if (token.isSymbolX("throw")) {

				if (expression(0)) {
					assemble(ParserAsm::Throw);
					return true;
				};

				error = ParserError::Compile;
				return false;
			};
			return false;
		};

	};
};

