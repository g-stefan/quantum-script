// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/Parser.hpp>
#include <XYO/QuantumScript/ParserAsm.hpp>

namespace XYO::QuantumScript {

	bool Parser::statementTry() {
		if (token.isSymbolX("try")) {

			if (token.checkIs1("{")) {
				ProgramCounter *linkCatch;
				ProgramCounter *linkFinally;
				ProgramCounter *linkTryBreak;
				ProgramCounter *linkTryContinue;
				ProgramCounter *linkTryReturn;
				ProgramCounter *linkGoFinally;
				ProgramCounter *linkTryThrow;
				ProgramCounter *linkCallPC;
				ProgramCounter *linkPC;

				parserEnterContext();
				assemble(ParserAsm::EnterContext);

				assemble(ParserAsm::ContextSetStack);
				linkCatch = assembleProgramCounter(ParserAsm::ContextSetCatch, nullptr);
				linkFinally = assembleProgramCounter(ParserAsm::ContextSetFinally, nullptr);
				linkTryBreak = assembleProgramCounter(ParserAsm::ContextSetTryBreak, nullptr);
				linkTryContinue = assembleProgramCounter(ParserAsm::ContextSetTryContinue, nullptr);
				linkTryReturn = assembleProgramCounter(ParserAsm::ContextSetTryReturn, nullptr);
				linkTryThrow = assembleProgramCounter(ParserAsm::ContextSetTryThrow, nullptr);

				if (isBlockStatement()) {

					parserLeaveContext();
					assemble(ParserAsm::LeaveContext);
					linkGoFinally = assembleProgramCounter(ParserAsm::Goto, nullptr);

					linkProgramCounter(linkTryBreak, assemble(ParserAsm::Break));
					linkProgramCounter(linkTryContinue, assemble(ParserAsm::Continue));
					linkProgramCounter(linkTryReturn, assemble(ParserAsm::Return));
					linkProgramCounter(linkTryThrow, assemble(ParserAsm::Throw));

					if (token.isSymbolX("catch")) {
						if (token.is1("(")) {
							token.reset();
							if (token.isSymbol()) {
								if (token.is1(")")) {
									linkProgramCounter(linkCatch,
									                   assemble(ParserAsm::Catch));
									assemble1(ParserAsm::PushObjectReference, token.value);
									assemble(ParserAsm::AssignReverse);

									if (token.checkIs1("{")) {

										parserEnterContext();
										assemble(ParserAsm::EnterContext);

										if (isBlockStatement()) {

											parserLeaveContext();
											assemble(ParserAsm::LeaveContext);

											if (token.isSymbolX("finally")) {
												if (token.checkIs1("{")) {

													parserEnterContext();

													linkPC = assemble(ParserAsm::EnterContext);
													linkProgramCounter(linkGoFinally, linkPC);
													linkCallPC = assembleProgramCounter(ParserAsm::ContextSetPC, nullptr);

													linkPC = assemble(ParserAsm::Nop);
													linkProgramCounter(linkFinally, linkPC);

													if (isBlockStatement()) {
														parserLeaveContext();

														assemble(ParserAsm::Return);
														linkPC = assemble(ParserAsm::Nop);
														linkProgramCounter(linkCallPC, linkPC);

														return true;
													};
												};
											} else {

												linkPC = assemble(ParserAsm::EnterContext);
												linkProgramCounter(linkGoFinally, linkPC);
												linkCallPC = assembleProgramCounter(ParserAsm::ContextSetPC, nullptr);

												linkPC = assemble(ParserAsm::Nop);
												linkProgramCounter(linkFinally, linkPC);

												assemble(ParserAsm::Return);
												linkPC = assemble(ParserAsm::Nop);
												linkProgramCounter(linkCallPC, linkPC);
												return true;
											};
										};
									};
								};
							};
						};
						error = ParserError::Compile;
						return false;
					};
					if (token.isSymbolX("finally")) {

						if (token.checkIs1("{")) {

							parserEnterContext();

							linkPC = assemble(ParserAsm::EnterContext);
							linkProgramCounter(linkGoFinally, linkPC);
							linkCallPC = assembleProgramCounter(ParserAsm::ContextSetPC, nullptr);

							linkPC = assemble(ParserAsm::Nop);
							linkProgramCounter(linkFinally, linkPC);

							if (isBlockStatement()) {

								parserLeaveContext();

								assemble(ParserAsm::Return);
								linkPC = assemble(ParserAsm::Nop);
								linkProgramCounter(linkCallPC, linkPC);

								return true;
							};
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
