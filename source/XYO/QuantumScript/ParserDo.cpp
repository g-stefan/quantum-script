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

	bool Parser::statementDo() {
		if (token.isSymbolX("do")) {
			AsmLabel linkDo;
			AsmLabel linkDoEnd;
			AsmLabel linkDoBreak;
			AsmLabel linkDoContinue;

			linkDo = assemble(ParserAsm::Mark);

			if (token.checkIs1("{")) {

				parserEnterContext();
				assemble(ParserAsm::EnterContext);
				linkDoBreak = assembleProgramCounter(ParserAsm::ContextSetBreak, 0);
				linkDoContinue = assembleProgramCounter(ParserAsm::ContextSetContinue, 0);

				if (isBlockStatement()) {

					parserLeaveContext();
					assemble(ParserAsm::LeaveContext);
					// continue: the condition
					linkProgramCounter(linkDoContinue, assemble(ParserAsm::Mark));

					if (token.isSymbolX("while")) {

						if (expressionParentheses()) {

							assembleProgramCounter(ParserAsm::IfTrueGoto, linkDo);
							linkProgramCounter(
							    linkDoBreak,
							    assemble(ParserAsm::Mark));

							return true;
						};
					};
				};
			};

			error = ParserError::Compile;
			return false;
		};
		return false;
	};

};
