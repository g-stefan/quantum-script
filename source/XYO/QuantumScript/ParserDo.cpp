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
