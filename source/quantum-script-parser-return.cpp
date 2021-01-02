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

#include "quantum-script-parser.hpp"
#include "quantum-script-parserasm.hpp"

namespace Quantum {
	namespace Script {


		bool Parser::statementReturn() {
			if (token.isSymbolX("return")) {

				if (token.checkIs1(";")) {
					assemble(ParserAsm::PushUndefined);
					assemble(ParserAsm::Return);
					return true;
				};
				if (expression(0)) {
					assemble(ParserAsm::Return);
					return true;
				};
				error = ParserError::Compile;
				return false;
			};
			return false;
		};

	};
};

