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

	bool Parser::statementIf() {
		if (token.isSymbolX("if")) {
			if (expressionParentheses()) {
				AsmLabel linkIf;

				linkIf = assembleProgramCounter(ParserAsm::IfFalseGoto, 0);

				if (token.checkIs1("{")) {

					if (isBlockStatement()) {

						if (token.isSymbolX("else")) {
							AsmLabel linkElse;

							if (token.checkIs1("{")) {

								linkElse = assembleProgramCounter(ParserAsm::Goto, 0);
								linkProgramCounter(linkIf,
								                   assemble(ParserAsm::Mark));

								if (isBlockStatement()) {

									linkProgramCounter(linkElse,
									                   assemble(ParserAsm::Mark));

									return true;
								};

								error = ParserError::Compile;
								return false;
							};

							linkElse = assembleProgramCounter(ParserAsm::Goto, 0);
							linkProgramCounter(linkIf,
							                   assemble(ParserAsm::Mark));

							if (statementOrExpression()) {
								linkProgramCounter(linkElse,
								                   assemble(ParserAsm::Mark));
								return true;
							};

							error = ParserError::Compile;
							return false;
						};

						linkProgramCounter(linkIf,
						                   assemble(ParserAsm::Mark));
						return true;
					};
					error = ParserError::Compile;
					return false;
				};

				if (statementOrExpression()) {
					linkProgramCounter(linkIf,
					                   assemble(ParserAsm::Mark));
					return true;
				};
			};
			error = ParserError::Compile;
		};
		return false;
	};

};
