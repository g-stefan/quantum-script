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

	bool Parser::statementForSub(int hasSymbol) {
		if (hasSymbol) {
			expression(1);
			if (error) {
				return false;
			};
			assemble(ParserAsm::Pop1);
		};
		while (!token.isEof()) {
			token.reset();
			if (token.checkIs1(";")) {
				return true;
			} else if (token.is1(",")) {
				continue;
			} else if (token.isSymbol()) {
				expression(1);
				if (error) {
					break;
				};
				assemble(ParserAsm::Pop1);
				continue;
			};
			break;
		};
		error = ParserError::Compile;
		return false;
	};

	bool Parser::statementFor() {
		bool hasSymbol;
		if (token.isSymbolX("for")) {
			if (token.is1("(")) {
				int isVar;
				isVar = 0;
				if (token.isSymbolX("var")) {
					isVar = 1;
				};
				token.reset();
				hasSymbol = token.isSymbol();

				// for in

				if (hasSymbol) {
					if (token.isSymbolX("in")) {
						AsmLabel linkForIn;
						AsmLabel linkForInIf;
						AsmLabel linkForInSkip;
						AsmLabel linkForInInc;
						AsmLabel linkForInBreak;
						AsmLabel linkForInEnd;
						if (isVar) {
							int index, level;
							bool found = false;
							if (isFunctionArgument(token.value, index, level)) {
								if (level == 0) {
									error = ParserError::Compile;
									return false;
								};
							};
							if (isFunctionLocalVariable(token.value, index, level)) {
								if (level == 0) { // already defined
									found = true;
								};
							};
							if (found) {
								char buffer1[32];
								sprintf(buffer1, "%d", index);
								(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
								assemble1(ParserAsm::LocalVariablesPushObjectReference, buffer1);
							} else {

								if (functionLocalVariables.head) {
									char buf[32];
									(functionLocalVariables.head)->value.set(token.value, (functionLocalVariableLevel.head)->value);
									sprintf(buf, "%d", (functionLocalVariableLevel.head)->value);
									++(functionLocalVariableLevel.head)->value;
									assemble1(ParserAsm::LocalVariablesPushObjectReference, buf);
									(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
								} else {
									assemble1(ParserAsm::PushObjectReference, token.value);
								};
							};
						} else {
							token.lValue = true;
							token.type = TokenType::FirstSymbol;
							if (expressionProcessToken(&token) == 0) {
								return false;
							};
						};

						assemble(ParserAsm::ContextSetReference);

						token.reset();
						if (expression(0)) {
							if (token.is1(")")) {
								assemble(ParserAsm::OperatorSetReferenceIndexKey);
								linkForInIf = assembleProgramCounter(ParserAsm::IfFalseGoto, 0);
								linkForInSkip = assembleProgramCounter(ParserAsm::Goto, 0);
								linkForInInc = assemble(ParserAsm::OperatorNextReferenceIndex);
								assembleProgramCounter(ParserAsm::Goto, linkForInIf);

								linkProgramCounter(linkForInSkip,
								                   assemble(ParserAsm::Mark));

								if (token.checkIs1("{")) {

									parserEnterContext();
									assemble(ParserAsm::EnterContext);

									linkForInBreak = assembleProgramCounter(ParserAsm::ContextSetBreak, 0);
									assembleProgramCounter(ParserAsm::ContextSetContinue, linkForInInc);

									if (isBlockStatement()) {

										parserLeaveContext();
										assemble(ParserAsm::LeaveContext);
										assembleProgramCounter(ParserAsm::Goto, linkForInInc);

										linkForInEnd = assemble(ParserAsm::Mark);

										linkProgramCounter(linkForInBreak, linkForInEnd);
										linkProgramCounter(linkForInIf, linkForInEnd);

										return true;
									};
									error = ParserError::Compile;
									return false;
								};

								if (statementOrExpression()) {
									assembleProgramCounter(ParserAsm::Goto, linkForInInc);
									linkForInEnd = assemble(ParserAsm::Mark);
									linkProgramCounter(linkForInIf, linkForInEnd);
									return true;
								};
							};
						};

						error = ParserError::Compile;
						return false;
					};
					if (token.isSymbolX("of")) {
						AsmLabel linkForIn;
						AsmLabel linkForInIf;
						AsmLabel linkForInSkip;
						AsmLabel linkForInInc;
						AsmLabel linkForInBreak;
						AsmLabel linkForInEnd;
						if (isVar) {

							int index, level;
							bool found = false;
							if (isFunctionArgument(token.value, index, level)) {
								if (level == 0) {
									error = ParserError::Compile;
									return false;
								};
							};
							if (isFunctionLocalVariable(token.value, index, level)) {
								if (level == 0) { // already defined
									found = true;
								};
							};
							if (found) {
								char buffer1[32];
								sprintf(buffer1, "%d", index);
								(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
								assemble1(ParserAsm::LocalVariablesPushObjectReference, buffer1);
							} else {

								if (functionLocalVariables.head) {
									char buf[32];
									(functionLocalVariables.head)->value.set(token.value, (functionLocalVariableLevel.head)->value);
									sprintf(buf, "%d", (functionLocalVariableLevel.head)->value);
									++(functionLocalVariableLevel.head)->value;
									assemble1(ParserAsm::LocalVariablesPushObjectReference, buf);
									(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
								} else {
									assemble1(ParserAsm::PushObjectReference, token.value);
								};
							};

						} else {
							token.lValue = true;
							token.type = TokenType::FirstSymbol;
							if (expressionProcessToken(&token) == 0) {
								return false;
							};
						};

						assemble(ParserAsm::ContextSetReference);

						token.reset();
						if (expression(0)) {
							if (token.is1(")")) {
								assemble(ParserAsm::OperatorSetReferenceIndexValue);
								linkForInIf = assembleProgramCounter(ParserAsm::IfFalseGoto, 0);
								linkForInSkip = assembleProgramCounter(ParserAsm::Goto, 0);
								linkForInInc = assemble(ParserAsm::OperatorNextReferenceIndex);
								assembleProgramCounter(ParserAsm::Goto, linkForInIf);

								linkProgramCounter(linkForInSkip,
								                   assemble(ParserAsm::Mark));

								if (token.checkIs1("{")) {

									parserEnterContext();
									assemble(ParserAsm::EnterContext);

									linkForInBreak = assembleProgramCounter(ParserAsm::ContextSetBreak, 0);
									assembleProgramCounter(ParserAsm::ContextSetContinue, linkForInInc);

									if (isBlockStatement()) {

										parserLeaveContext();
										assemble(ParserAsm::LeaveContext);
										assembleProgramCounter(ParserAsm::Goto, linkForInInc);

										linkForInEnd = assemble(ParserAsm::Mark);

										linkProgramCounter(linkForInBreak, linkForInEnd);
										linkProgramCounter(linkForInIf, linkForInEnd);

										return true;
									};
									error = ParserError::Compile;
									return false;
								};

								if (statementOrExpression()) {
									assembleProgramCounter(ParserAsm::Goto, linkForInInc);
									linkForInEnd = assemble(ParserAsm::Mark);
									linkProgramCounter(linkForInIf, linkForInEnd);
									return true;
								};
							};
						};

						error = ParserError::Compile;
						return false;
					};
				};

				AsmLabel linkFor;
				AsmLabel linkForIf;
				AsmLabel linkForSkip;
				AsmLabel linkForInc;
				AsmLabel linkForBreak;
				AsmLabel linkForEnd;

				if (isVar) {
					if (statementVarSub(hasSymbol)) {
					} else {
						error = ParserError::Compile;
						return false;
					};
				} else {
					if (statementForSub(hasSymbol)) {
					} else {
						error = ParserError::Compile;
						return false;
					};
				};

				if (token.is1(";")) {
					linkFor = assemble(ParserAsm::Mark);
					token.reset();

					if (expression(0)) {
					} else {
						assemble1(ParserAsm::PushBoolean, "true");
					};

					if (token.is1(";")) {

						linkForIf = assembleProgramCounter(ParserAsm::IfFalseGoto, 0);
						linkForSkip = assembleProgramCounter(ParserAsm::Goto, 0);
						linkForInc = assemble(ParserAsm::Mark);

						do {
							if (expression(0)) {
								assemble(ParserAsm::Pop1);
							} else {
								break;
							};
						} while (token.is1(","));

						if (token.is1(")")) {
							assembleProgramCounter(ParserAsm::Goto, linkFor);
							linkProgramCounter(linkForSkip,
							                   assemble(ParserAsm::Mark));

							if (token.checkIs1("{")) {

								parserEnterContext();
								parserContext.head->value->isLoop = true;
								parserContext.head->value->pcContinue = linkForInc;

								if (isBlockStatement()) {

									assembleProgramCounter(ParserAsm::Goto, linkForInc);

									linkForEnd = assemble(ParserAsm::Mark);
									linkProgramCounter(linkForIf, linkForEnd);

									while (!parserContext.head->value->pcBreak.isEmpty()) {
										parserContext.head->value->pcBreak.pop(linkForBreak);
										linkProgramCounter(linkForBreak, linkForEnd);
									};
									parserLeaveContext();

									return true;
								};
								error = ParserError::Compile;
								return false;
							};

							if (statementOrExpression()) {
								assembleProgramCounter(ParserAsm::Goto, linkForInc);
								linkForEnd = assemble(ParserAsm::Mark);
								linkProgramCounter(linkForIf, linkForEnd);
								return true;
							};
						};
					};
				};
			};
			error = ParserError::Compile;
		};
		return false;
	};

};
