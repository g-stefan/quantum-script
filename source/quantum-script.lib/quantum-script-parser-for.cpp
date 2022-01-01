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
							ProgramCounter *linkForIn;
							ProgramCounter *linkForInIf;
							ProgramCounter *linkForInSkip;
							ProgramCounter *linkForInInc;
							ProgramCounter *linkForInBreak;
							ProgramCounter *linkForInEnd;
							if (isVar) {
								int index, level;
								bool found = false;
								if(isFunctionArgument(token.value, index, level)) {
									if(level == 0) {
										error = ParserError::Compile;
										return false;
									};
								};
								if(isFunctionLocalVariable(token.value, index, level)) {
									if(level == 0) { // already defined
										found = true;
									};
								};
								if(found) {
									char buffer1[32];
									sprintf(buffer1, "%d", index);
									(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
									assemble1(ParserAsm::LocalVariablesPushObjectReference, buffer1);
								} else {


									if(functionLocalVariables.head) {
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
								if(expressionProcessToken(&token) == nullptr) {
									return false;
								};
							};

							assemble(ParserAsm::ContextSetReference);

							token.reset();
							if (expression(0)) {
								if (token.is1(")")) {
									assemble(ParserAsm::OperatorSetReferenceIndexKey);
									linkForInIf = assembleProgramCounter(ParserAsm::IfFalseGoto, nullptr);
									linkForInSkip = assembleProgramCounter(ParserAsm::Goto, nullptr);
									linkForInInc = assemble(ParserAsm::OperatorNextReferenceIndex);
									assembleProgramCounter(ParserAsm::Goto, linkForInIf);

									linkProgramCounter(linkForInSkip,
										assemble(ParserAsm::Mark)
									);

									if (token.checkIs1("{")) {

										parserEnterContext();
										assemble(ParserAsm::EnterContext);

										linkForInBreak = assembleProgramCounter(ParserAsm::ContextSetBreak, nullptr);
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
							ProgramCounter *linkForIn;
							ProgramCounter *linkForInIf;
							ProgramCounter *linkForInSkip;
							ProgramCounter *linkForInInc;
							ProgramCounter *linkForInBreak;
							ProgramCounter *linkForInEnd;
							if (isVar) {

								int index, level;
								bool found = false;
								if(isFunctionArgument(token.value, index, level)) {
									if(level == 0) {
										error = ParserError::Compile;
										return false;
									};
								};
								if(isFunctionLocalVariable(token.value, index, level)) {
									if(level == 0) { // already defined
										found = true;
									};
								};
								if(found) {
									char buffer1[32];
									sprintf(buffer1, "%d", index);
									(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
									assemble1(ParserAsm::LocalVariablesPushObjectReference, buffer1);
								} else {

									if(functionLocalVariables.head) {
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
								if(expressionProcessToken(&token) == nullptr) {
									return false;
								};
							};

							assemble(ParserAsm::ContextSetReference);

							token.reset();
							if (expression(0)) {
								if (token.is1(")")) {
									assemble(ParserAsm::OperatorSetReferenceIndexValue);
									linkForInIf = assembleProgramCounter(ParserAsm::IfFalseGoto, nullptr);
									linkForInSkip = assembleProgramCounter(ParserAsm::Goto, nullptr);
									linkForInInc = assemble(ParserAsm::OperatorNextReferenceIndex);
									assembleProgramCounter(ParserAsm::Goto, linkForInIf);

									linkProgramCounter(linkForInSkip,
										assemble(ParserAsm::Mark)
									);

									if (token.checkIs1("{")) {

										parserEnterContext();
										assemble(ParserAsm::EnterContext);

										linkForInBreak = assembleProgramCounter(ParserAsm::ContextSetBreak, nullptr);
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

					ProgramCounter *linkFor;
					ProgramCounter *linkForIf;
					ProgramCounter *linkForSkip;
					ProgramCounter *linkForInc;
					ProgramCounter *linkForBreak;
					ProgramCounter *linkForEnd;

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

							linkForIf = assembleProgramCounter(ParserAsm::IfFalseGoto, nullptr);
							linkForSkip = assembleProgramCounter(ParserAsm::Goto, nullptr);
							linkForInc = assemble(ParserAsm::Mark);

							do {
								if(expression(0)) {
									assemble(ParserAsm::Pop1);
								} else {
									break;
								};
							} while(token.is1(","));

							if (token.is1(")")) {
								assembleProgramCounter(ParserAsm::Goto, linkFor);
								linkProgramCounter(linkForSkip,
									assemble(ParserAsm::Mark)
								);


								if (token.checkIs1("{")) {

									parserEnterContext();
									parserContext.head->value->isLoop = true;
									parserContext.head->value->pcContinue = linkForInc;

									if (isBlockStatement()) {

										assembleProgramCounter(ParserAsm::Goto, linkForInc);

										linkForEnd = assemble(ParserAsm::Mark);
										linkProgramCounter(linkForIf, linkForEnd);

										while(!parserContext.head->value->pcBreak.isEmpty()) {
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
};


