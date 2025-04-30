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

	bool Parser::statementWhile() {
		if (token.isSymbolX("while")) {
			ProgramCounter *linkWhileBegin;
			ProgramCounter *linkWhile;
			ProgramCounter *linkWhileIf;
			ProgramCounter *linkWhileEnd;
			ProgramCounter *linkWhileBreak;
			ProgramCounter *linkWhileContinue;
			ProgramCounter *linkWhileEndBreak;

			parserEnterContext();

			linkWhile = assemble(ParserAsm::Mark);

			parserContext.head->value->isLoop = true;
			parserContext.head->value->pcContinue = linkWhile;

			if (expressionParentheses()) {

				linkWhileIf = assembleProgramCounter(ParserAsm::IfFalseGoto, nullptr);

				if (token.checkIs1("{")) {

					if (isBlockStatement()) {

						assembleProgramCounter(ParserAsm::Goto, linkWhile);

						linkWhileEnd = assemble(ParserAsm::Mark);

						linkProgramCounter(linkWhileIf, linkWhileEnd);

						while (!parserContext.head->value->pcBreak.isEmpty()) {
							parserContext.head->value->pcBreak.pop(linkWhileBreak);
							linkProgramCounter(linkWhileBreak, linkWhileEnd);
						};
						parserLeaveContext();

						return true;
					};
					error = ParserError::Compile;
					return false;
				};

				if (statementOrExpression()) {
					assembleProgramCounter(ParserAsm::Goto, linkWhile);

					linkWhileEnd = assemble(ParserAsm::Mark);
					linkProgramCounter(linkWhileIf, linkWhileEnd);

					while (!parserContext.head->value->pcBreak.isEmpty()) {
						parserContext.head->value->pcBreak.pop(linkWhileBreak);
						linkProgramCounter(linkWhileBreak, linkWhileEnd);
					};
					parserLeaveContext();

					return true;
				};
			};

			error = ParserError::Compile;
			return false;
		};
		return false;
	};

};
