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

		bool Parser::statementBreak() {
			if (token.isSymbolX("break")) {
				if (parserContext.head->value->isLoop) {
					parserContext.head->value->pcBreak.push(assembleProgramCounter(ParserAsm::Goto, nullptr));
					return true;
				};
				assemble(ParserAsm::Break);
				return true;
			};
			return false;
		};

	};
};
