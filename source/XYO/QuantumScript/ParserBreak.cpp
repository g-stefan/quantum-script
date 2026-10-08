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

	bool Parser::statementBreak() {
		if (token.isSymbolX("break")) {
			assembleBreak();
			return true;
		};
		return false;
	};

	// a loop without context (for, while): goto, else break of the contexts
	AsmLabel Parser::assembleBreak() {
		if (parserContext.head->value->isLoop) {
			AsmLabel retV = assembleProgramCounter(ParserAsm::Goto, 0);
			parserContext.head->value->pcBreak.push(retV);
			return retV;
		};
		return assemble(ParserAsm::Break);
	};

};
