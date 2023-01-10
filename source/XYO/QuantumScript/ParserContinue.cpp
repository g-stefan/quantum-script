// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/Parser.hpp>
#include <XYO/QuantumScript/ParserAsm.hpp>

namespace XYO::QuantumScript {

	bool Parser::statementContinue() {
		if (token.isSymbolX("continue")) {
			if (parserContext.head->value->isLoop) {
				assembleProgramCounter(ParserAsm::Goto, parserContext.head->value->pcContinue);
				return true;
			};
			assemble(ParserAsm::Continue);
			return true;
		};
		return false;
	};

};
