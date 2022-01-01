//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
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


		bool Parser::statementContinue() {
			if (token.isSymbolX("continue")) {
				if(parserContext.head->value->isLoop) {
					assembleProgramCounter(ParserAsm::Goto, parserContext.head->value->pcContinue);
					return true;
				};
				assemble(ParserAsm::Continue);
				return true;
			};
			return false;
		};

	};
};

