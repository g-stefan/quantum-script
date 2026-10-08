// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/Parser.hpp>
#include <XYO/QuantumScript/ParserAsm.hpp>

namespace XYO::QuantumScript {

	bool Parser::statementContinue() {
		if (token.isSymbolX("continue")) {
			assembleContinue();
			return true;
		};
		return false;
	};

	// a loop without context (for, while): goto, else continue of the contexts
	AsmLabel Parser::assembleContinue() {
		if (parserContext.head->value->isLoop) {
			return assembleProgramCounter(ParserAsm::Goto, parserContext.head->value->pcContinue);
		};
		return assemble(ParserAsm::Continue);
	};

};
