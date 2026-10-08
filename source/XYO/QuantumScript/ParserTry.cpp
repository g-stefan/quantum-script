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

	//
	// try { A } catch (e) { B } finally { C }
	//
	//     enter-context; context-set-stack; context-set-catch Catch; context-set-try-finally Finally
	//     context-set-try-break / continue / return / throw TryBreak / TryContinue / TryReturn / TryThrow
	//     A
	//     leave-context; goto FinallyEntry
	// TryBreak: break (goto in a loop without context), TryContinue: continue, TryReturn: return, TryThrow: throw
	// Catch: catch; e = exception
	//     enter-context; context-set-stack; context-set-try-finally Finally
	//     context-set-try-break / continue / return / throw, as the try
	//     B
	//     leave-context
	// FinallyEntry: enter-context; context-set-break TryBreak; context-set-continue TryContinue
	//     context-set-finally-next End
	// Finally: C
	//     finally-end
	// End:
	//
	// break / continue / return / throw out of the try or the catch leave its context,
	// enter the finally context with finally-next (pending value on the stack) at TryBreak / ...;
	// the finally is not a call, return / break / continue / throw in it leave it
	//
	bool Parser::statementTry() {
		if (token.isSymbolX("try")) {

			if (token.checkIs1("{")) {
				AsmLabel linkCatch;
				AsmLabel linkFinally;
				AsmLabel linkCatchFinally = 0;
				AsmLabel linkTryBreak;
				AsmLabel linkTryContinue;
				AsmLabel linkTryReturn;
				AsmLabel linkTryThrow;
				AsmLabel linkGoFinally;
				AsmLabel linkFinallyNext;
				AsmLabel linkPC;
				AsmLabel tryBreak;
				AsmLabel tryContinue;
				AsmLabel tryReturn;
				AsmLabel tryThrow;
				bool hasCatch = false;
				bool hasFinally = false;

				parserEnterContext();
				assemble(ParserAsm::EnterContext);

				assemble(ParserAsm::ContextSetStack);
				linkCatch = assembleProgramCounter(ParserAsm::ContextSetCatch, 0);
				linkFinally = assembleProgramCounter(ParserAsm::ContextSetTryFinally, 0);
				linkTryBreak = assembleProgramCounter(ParserAsm::ContextSetTryBreak, 0);
				linkTryContinue = assembleProgramCounter(ParserAsm::ContextSetTryContinue, 0);
				linkTryReturn = assembleProgramCounter(ParserAsm::ContextSetTryReturn, 0);
				linkTryThrow = assembleProgramCounter(ParserAsm::ContextSetTryThrow, 0);

				if (!isBlockStatement()) {
					error = ParserError::Compile;
					return false;
				};

				parserLeaveContext();
				assemble(ParserAsm::LeaveContext);
				linkGoFinally = assembleProgramCounter(ParserAsm::Goto, 0);

				// after the finally
				tryBreak = assembleBreak();
				tryContinue = assembleContinue();
				tryReturn = assemble(ParserAsm::Return);
				tryThrow = assemble(ParserAsm::Throw);
				linkProgramCounter(linkTryBreak, tryBreak);
				linkProgramCounter(linkTryContinue, tryContinue);
				linkProgramCounter(linkTryReturn, tryReturn);
				linkProgramCounter(linkTryThrow, tryThrow);

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

									// the catch, in a context with the finally of the try
									parserEnterContext();
									assemble(ParserAsm::EnterContext);
									assemble(ParserAsm::ContextSetStack);
									linkCatchFinally = assembleProgramCounter(ParserAsm::ContextSetTryFinally, 0);
									assembleProgramCounter(ParserAsm::ContextSetTryBreak, tryBreak);
									assembleProgramCounter(ParserAsm::ContextSetTryContinue, tryContinue);
									assembleProgramCounter(ParserAsm::ContextSetTryReturn, tryReturn);
									assembleProgramCounter(ParserAsm::ContextSetTryThrow, tryThrow);

									if (isBlockStatement()) {
										parserLeaveContext();
										assemble(ParserAsm::LeaveContext);
										hasCatch = true;
									};
								};
							};
						};
					};
					if (!hasCatch) {
						error = ParserError::Compile;
						return false;
					};
				};

				if (token.isSymbolX("finally")) {
					if (!token.checkIs1("{")) {
						error = ParserError::Compile;
						return false;
					};
					hasFinally = true;
				};

				if (!(hasCatch || hasFinally)) {
					error = ParserError::Compile;
					return false;
				};

				// the finally, empty if not present
				if (hasFinally) {
					parserEnterContext();
				};

				linkPC = assemble(ParserAsm::EnterContext);
				linkProgramCounter(linkGoFinally, linkPC);
				assembleProgramCounter(ParserAsm::ContextSetBreak, tryBreak);
				assembleProgramCounter(ParserAsm::ContextSetContinue, tryContinue);
				linkFinallyNext = assembleProgramCounter(ParserAsm::ContextSetFinallyNext, 0);

				linkPC = assemble(ParserAsm::Nop);
				linkProgramCounter(linkFinally, linkPC);
				if (linkCatchFinally) {
					linkProgramCounter(linkCatchFinally, linkPC);
				};

				if (hasFinally) {
					if (!isBlockStatement()) {
						error = ParserError::Compile;
						return false;
					};
					parserLeaveContext();
				};

				assemble(ParserAsm::FinallyEnd);
				linkPC = assemble(ParserAsm::Nop);
				linkProgramCounter(linkFinallyNext, linkPC);

				return true;
			};

			error = ParserError::Compile;
			return false;
		};
		return false;
	};

};
