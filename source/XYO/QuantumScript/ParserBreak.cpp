// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/Parser.hpp>
#include <XYO/QuantumScript/ParserAsm.hpp>

namespace XYO::QuantumScript {

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
