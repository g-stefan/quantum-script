//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
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


		bool Parser::statementDo() {
			if (token.isSymbolX("do")) {
				ProgramCounter *linkDo;
				ProgramCounter *linkDoEnd;
				ProgramCounter *linkDoBreak;

				linkDo = assemble(ParserAsm::Mark);

				if (token.checkIs1("{")) {

					parserEnterContext();
					assemble(ParserAsm::EnterContext);
					linkDoBreak = assembleProgramCounter(ParserAsm::ContextSetBreak, nullptr);
					assembleProgramCounter(ParserAsm::ContextSetContinue, linkDo);

					if (isBlockStatement()) {

						parserLeaveContext();
						assemble(ParserAsm::LeaveContext);

						if (token.isSymbolX("while")) {

							if (expressionParentheses()) {

								assembleProgramCounter(ParserAsm::IfTrueGoto, linkDo);
								linkProgramCounter(
									linkDoBreak,
									assemble(ParserAsm::Mark)
								);

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
};


