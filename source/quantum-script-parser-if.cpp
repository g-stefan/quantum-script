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


		bool Parser::statementIf() {
			if (token.isSymbolX("if")) {
				if (expressionParentheses()) {
					ProgramCounter *linkIf;

					linkIf = assembleProgramCounter(ParserAsm::IfFalseGoto, nullptr);

					if (token.checkIs1("{")) {

						if (isBlockStatement()) {

							if (token.isSymbolX("else")) {
								ProgramCounter *linkElse;

								if (token.checkIs1("{")) {

									linkElse = assembleProgramCounter(ParserAsm::Goto, nullptr);
									linkProgramCounter(linkIf,
										assemble(ParserAsm::Mark)
									);

									if (isBlockStatement()) {

										linkProgramCounter(linkElse,
											assemble(ParserAsm::Mark)
										);

										return true;
									};

									error = ParserError::Compile;
									return false;

								};

								linkElse = assembleProgramCounter(ParserAsm::Goto, nullptr);
								linkProgramCounter(linkIf,
									assemble(ParserAsm::Mark)
								);

								if (statementOrExpression()) {
									linkProgramCounter(linkElse,
										assemble(ParserAsm::Mark)
									);
									return true;
								};

								error = ParserError::Compile;
								return false;
							};

							linkProgramCounter(linkIf,
								assemble(ParserAsm::Mark)
							);
							return true;

						};
						error = ParserError::Compile;
						return false;
					};

					if (statementOrExpression()) {
						linkProgramCounter(linkIf,
							assemble(ParserAsm::Mark)
						);
						return true;
					};
				};
				error = ParserError::Compile;
			};
			return false;
		};

	};
};


