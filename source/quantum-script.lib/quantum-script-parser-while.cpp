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

};
