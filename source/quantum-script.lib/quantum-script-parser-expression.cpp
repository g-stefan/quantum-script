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

		bool Parser::expression(int hasSymbol) {
			Token localToken;
			localToken.init(token);
			if (expressionTerm(&localToken, hasSymbol, 0)) {
				localToken.reset();
				if (expressionIsBinaryOperator(&localToken)) {
					if (expressionStep(&localToken)) {
						return true;
					} else {
						error = ParserError::Compile;
						return false;
					};
				};
				return true;
			};
			return false;
		};

		bool Parser::expressionStep(Token *backOperator, Token *levelOperator) {
			int isOk;
			Token localToken, levelToken;
			localToken.init(token);
			isOk = 0;

			if(backOperator->asmType == ParserAsm::OperatorConditional) {
				ProgramCounter *linkIf;
				linkIf = assembleProgramCounter(ParserAsm::IfFalseGoto, nullptr);
				if(expression(0)) {
					token.reset();
					if (token.is1(":")) {
						ProgramCounter *linkElse;
						linkElse = assembleProgramCounter(ParserAsm::Goto, nullptr);
						linkProgramCounter(linkIf, assemble(ParserAsm::Mark));
						if(expression(0)) {
							// Nop, don't use Mark (can be replaced by optimizer, runtime error)
							linkProgramCounter(linkElse, assemble(ParserAsm::Nop));
							return true;
						};
					};
				};
				error = ParserError::Compile;
				return false;
			};

			if (backOperator->asmType == ParserAsm::OperatorAssign ||
				backOperator->asmType == ParserAsm::OperatorAssignPlus ||
				backOperator->asmType == ParserAsm::OperatorAssignMinus ||
				backOperator->asmType == ParserAsm::OperatorAssignMul ||
				backOperator->asmType == ParserAsm::OperatorAssignDiv ||
				backOperator->asmType == ParserAsm::OperatorAssignMod ||
				backOperator->asmType == ParserAsm::OperatorAssignBitwiseOr ||
				backOperator->asmType == ParserAsm::OperatorAssignBitwiseAnd
			) {

				if (statementFunction()) {
					isOk = 1;
				} else if (token.isSymbolX("new")) {
					assemble(ParserAsm::PushNewObject);
					assemble(ParserAsm::Duplicate);

					if (expressionTerm(&localToken, 0, 1)) {
						assemble(ParserAsm::AssignNewObject);
						isOk = 1;

					} else {
						error = ParserError::Compile;
						return false;
					};

				};

			};

			if (!isOk) {
				if (expressionTerm(&localToken, 0, 0)) {
					isOk = 1;
				};
			};

			if (isOk) {
				localToken.reset();
				if (expressionIsBinaryOperator(&localToken)) {
					if (localToken.precedence < backOperator->precedence) {
						assemble(backOperator->asmType);
						if(levelOperator) {
							levelOperator->copy(localToken);
							return true;
						};
						levelToken.reset();
						if(expressionStep(&localToken, &levelToken)) {
							if(levelToken.asmType > 0) {
								localToken.copy(levelToken);
								return expressionStep(&localToken);
							};
							return true;
						};
						return false;
					} else if (localToken.precedence == backOperator->precedence) {
						if (localToken.associativity == TokenAssociativity::RightToLeft) {
							expressionStep(&localToken, levelOperator);
							assemble(backOperator->asmType);
							return true;
						};
						assemble(backOperator->asmType);
						return expressionStep(&localToken, levelOperator);
					};
					expressionStep(&localToken, levelOperator);
				};
				assemble(backOperator->asmType);
				return true;
			};

			error = ParserError::Compile;
			return false;
		};

		bool Parser::expressionParentheses() {
			if (token.is1("(")) {
				token.reset();
				if (expression(0)) {
					if (token.is1(")")) {
						return true;
					};
				};
				error = ParserError::Compile;
				return false;
			};
			return false;
		};

		bool Parser::expressionTerm(Token *backToken, int hasSymbol, int isNew) {
			int isTypeOf;

			isTypeOf = 0;
			if (hasSymbol) {
				if (token.isSymbolX("typeof")) {
					isTypeOf = 1;
				};
			} else {
				if (token.isSymbolX("typeof")) {
					isTypeOf = 1;
				};
			};

			if (isTypeOf) {
				if (token.is1("(")) {
					token.reset();
					if (expression(0)) {
						if (token.is1(")")) {
							assemble(ParserAsm::OperatorTypeOf);
							return true;
						};
					};
				};
				error = ParserError::Compile;
				return false;
			};


			if (statementFunction()) {
				return true;
			} else if (token.isSymbolX("new")) {
				Token localToken;
				localToken.init(token);

				assemble(ParserAsm::PushNewObject);
				assemble(ParserAsm::Duplicate);

				if (expressionTerm(&localToken, 0, 1)) {
					assemble(ParserAsm::AssignNewObject);
					return true;
				};
				error = ParserError::Compile;
				return false;
			} else if (token.isSymbolX("delete")) {
				Token localToken;
				localToken.init(token);
				localToken.reset();
				if (localToken.checkIs1(".")) {
					(functionHint.head)->value |= ParserFunctionHint::This;
					assemble(ParserAsm::ContextPushThis);
					localToken.reset();
					localToken.lValue = 1;
					localToken.lValueDelete = 1;
					if(expressionTermSelector(&localToken, 0, 0, isNew)) {
						if (error) {
							return false;
						};
						return true;
					};
					error = ParserError::Compile;
					return false;
				};
				if (localToken.isSymbol()) {
					localToken.lValue = 1;
					localToken.lValueDelete = 1;
					localToken.type = TokenType::FirstSymbol;
					if (expressionTermSelector(&localToken, 1, 0, isNew)) {
						if (error) {
							return false;
						};
						return true;
					};
					return true;
				};
				error = ParserError::Compile;
				return false;
			} else if (expressionTermSymbol(backToken, hasSymbol, isNew)) {
				if(hasSymbol == 0) {
					expressionTermSelector(&token, 0, 0, isNew);
				};
				return true;
			} else if (expressionParentheses()) {
				expressionTermSelector(&token, 0, 0, isNew);
				return true;
			} else if (expressionTermPrefix(backToken)) {
				return true;
			} else if (expressionTermObject()) {
				expressionTermSelector(&token, 0, 0, isNew);
				return true;
			} else if (expressionTermArray()) {
				expressionTermSelector(&token, 0, 0, isNew);
				return true;
			};
			return false;
		};

		bool Parser::expressionTermSymbol(Token *backToken, int hasSymbol, int isNew) {
			Token localToken;


			localToken.init(token);


			if (hasSymbol) {
				localToken.copy(token);
			} else {
				hasSymbol = localToken.isSymbol();
			};

			if (hasSymbol) {
				localToken.type = TokenType::FirstSymbol;
				expressionTermSelector(&localToken, 1, 0, isNew);
				return true;
			} else if (localToken.isNumber()) {
				if (localToken.type == TokenType::NumberInteger) {
					assemble1(ParserAsm::PushNumber, localToken.value);
				} else if (localToken.type == TokenType::NumberReal) {
					assemble1(ParserAsm::PushNumber, localToken.value);
				} else if (localToken.type == TokenType::NumberHexadecimal) {
					char buf[60];
					unsigned int tmp_ = 0;
					if(sscanf((char *) & (localToken.value.value())[2], "%x", &tmp_)!=1) {
						tmp_ = 0;
					};
					sprintf(buf, "%u", tmp_);
					assemble1(ParserAsm::PushNumber, buf);
				} else {
					error = ParserError::Compile;
					return false;
				};
				return true;
			} else if (localToken.isString()) {
				assemble1(ParserAsm::PushString, localToken.value);
				return true;
			};

			return false;
		};

		ProgramCounter *Parser::expressionProcessToken(Token *token_) {
			int index;
			int level;
			char buffer1[22];
			char buffer2[22];
			tokenLastAsm = ParserAsm::Nop;
			tokenLastValue = "";
			tokenLastIndex = 0;
			tokenLastLevel = 0;

			index = 0;
			if (token_->lValue) {
				if(token_->lValueDelete) {
					if (token_->type == TokenType::Index) {
						tokenLastAsm = ParserAsm::OperatorReferenceDeleteIndex;
						tokenLastValue = "";
						return assemble(ParserAsm::OperatorReferenceDeleteIndex);
					};
					if (token_->type == TokenType::Symbol) {
						tokenLastAsm = ParserAsm::OperatorReferenceDeleteReference;
						tokenLastValue = token_->value;
						return assemble1(ParserAsm::OperatorReferenceDeleteReference, token_->value);
					};
					error = ParserError::Compile;
					return nullptr;
				};

				if (token_->type == TokenType::Call) {
					error = ParserError::Compile;
					return nullptr;
				} else if (token_->type == TokenType::Index) {
					tokenLastAsm = ParserAsm::OperatorReferenceReference;
					tokenLastValue = "";
					return assemble(ParserAsm::OperatorReferenceReference);
				} else if (token_->type == TokenType::Symbol) {
					if (token_->is("undefined")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("null")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("this")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("self")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("true")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("false")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("NaN")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("Infinity")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("arguments")) {
						(functionHint.head)->value |= ParserFunctionHint::Arguments;
					};
					if (token_->is("global")) {
						error = ParserError::Compile;
						return nullptr;
					};

					tokenLastAsm = ParserAsm::ReferenceObjectReference;
					tokenLastValue = token_->value;
					return assemble1(ParserAsm::ReferenceObjectReference, token_->value);
				} else if (token_->type == TokenType::FirstSymbol) {
					if (token_->is("undefined")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("null")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("this")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("self")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("true")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("false")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("NaN")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("Infinity")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("arguments")) {
						(functionHint.head)->value |= ParserFunctionHint::Arguments;
					};
					if (token_->is("global")) {
						error = ParserError::Compile;
						return nullptr;
					};

					if (isFunctionArgument(token_->value, index, level)) {
						if (level == 0) {
							sprintf(buffer1, "%d", index);
							(functionHint.head)->value |= ParserFunctionHint::Arguments;
							tokenLastAsm = ParserAsm::ArgumentsPushObjectReference;
							tokenLastValue = token_->value;
							tokenLastIndex = index;
							return assemble1(ParserAsm::ArgumentsPushObjectReference, buffer1);
						} else {
							sprintf(buffer1, "%d", index);
							sprintf(buffer2, "%d", level);
							(functionHint.head)->value |= ParserFunctionHint::ArgumentsLevel;
							setArgumentsLevelHint(level);
							tokenLastAsm = ParserAsm::ArgumentsLevelPushObjectReference;
							tokenLastValue = token_->value;
							tokenLastIndex = index;
							tokenLastLevel = level;
							return assembleX(ParserAsm::ArgumentsLevelPushObjectReference, buffer1, buffer2);
						};
					} else {

						if (isFunctionLocalVariable(token_->value, index, level)) {
							if (level == 0) {
								sprintf(buffer1, "%d", index);
								(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
								tokenLastAsm = ParserAsm::LocalVariablesPushObjectReference;
								tokenLastValue = token_->value;
								tokenLastIndex = index;
								return assemble1(ParserAsm::LocalVariablesPushObjectReference, buffer1);
							} else {
								sprintf(buffer1, "%d", index);
								sprintf(buffer2, "%d", level);
								(functionHint.head)->value |= ParserFunctionHint::LocalVariablesLevel;
								setVariablesLevelHint(level);
								tokenLastAsm = ParserAsm::LocalVariablesLevelPushObjectReference;
								tokenLastValue = token_->value;
								tokenLastIndex = index;
								tokenLastLevel = level;
								return assembleX(ParserAsm::LocalVariablesLevelPushObjectReference, buffer1, buffer2);
							};
						} else {
							tokenLastAsm = ParserAsm::PushObjectReference;
							tokenLastValue = token_->value;
							return assemble1(ParserAsm::PushObjectReference, token_->value);
						};

					};
				} else {
					error = ParserError::Compile;
					return nullptr;
				};

			} else {

				if (token_->type == TokenType::Call) {
				} else if (token_->type == TokenType::Index) {
					tokenLastAsm = ParserAsm::OperatorReference;
					tokenLastValue = "";
					return assemble(ParserAsm::OperatorReference);
				} else if (token_->type == TokenType::Symbol) {
					if (token_->is("undefined")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("null")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("this")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("self")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("true")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("false")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("NaN")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("Infinity")) {
						error = ParserError::Compile;
						return nullptr;
					};
					if (token_->is("arguments")) {
						(functionHint.head)->value |= ParserFunctionHint::Arguments;
					};
					if (token_->is("global")) {
						error = ParserError::Compile;
						return nullptr;
					};

					tokenLastValue = token_->value;
					return assemble1(ParserAsm::Reference, token_->value);
				} else if (token_->type == TokenType::FirstSymbol) {
					if (token_->is("undefined")) {
						tokenLastValue = "";
						tokenLastAsm = ParserAsm::PushUndefined;
						return assemble(ParserAsm::PushUndefined);
					};
					if (token_->is("null")) {
						tokenLastValue = "";
						tokenLastAsm = ParserAsm::PushNull;
						return assemble(ParserAsm::PushNull);
					};
					if (token_->is("this")) {
						(functionHint.head)->value |= ParserFunctionHint::This;
						tokenLastAsm = ParserAsm::ContextPushThis;
						tokenLastValue = "";
						return assemble(ParserAsm::ContextPushThis);
					};
					if (token_->is("self")) {
						(functionHint.head)->value |= ParserFunctionHint::Self;
						tokenLastAsm = ParserAsm::ContextPushSelf;
						tokenLastValue = "";
						return assemble(ParserAsm::ContextPushSelf);
					};
					if (token_->is("true")) {
						tokenLastValue = "";
						tokenLastAsm = ParserAsm::PushBoolean;
						return assemble1(ParserAsm::PushBoolean, "true");
					};
					if (token_->is("false")) {
						tokenLastValue = "";
						tokenLastAsm = ParserAsm::PushBoolean;
						return assemble1(ParserAsm::PushBoolean, "false");
					};
					if (token_->is("NaN")) {
						tokenLastValue = "";
						tokenLastAsm = ParserAsm::PushNaN;
						return assemble(ParserAsm::PushNaN);
					};
					if (token_->is("Infinity")) {
						tokenLastValue = "";
						tokenLastAsm = ParserAsm::PushInfinity;
						return assemble(ParserAsm::PushInfinity);
					};
					if (token_->is("arguments")) {
						(functionHint.head)->value |= ParserFunctionHint::Arguments;
					};
					if (token_->is("global")) {
						tokenLastAsm = ParserAsm::PushGlobal;
						tokenLastValue = "";
						return assemble(ParserAsm::PushGlobal);
					};

					if (isFunctionArgument(token_->value, index, level)) {
						if (level == 0) {
							sprintf(buffer1, "%d", index);
							(functionHint.head)->value |= ParserFunctionHint::Arguments;
							tokenLastAsm = ParserAsm::ArgumentsPushSymbol;
							tokenLastValue = token_->value;
							tokenLastIndex = index;
							return assemble1(ParserAsm::ArgumentsPushSymbol, buffer1);
						} else {
							sprintf(buffer1, "%d", index);
							sprintf(buffer2, "%d", level);
							(functionHint.head)->value |= ParserFunctionHint::ArgumentsLevel;
							setArgumentsLevelHint(level);
							tokenLastAsm = ParserAsm::ArgumentsLevelPushSymbol;
							tokenLastValue = token_->value;
							tokenLastIndex = index;
							tokenLastLevel = level;
							return assembleX(ParserAsm::ArgumentsLevelPushSymbol, buffer1, buffer2);
						};

					} else {
						if (isFunctionLocalVariable(token_->value, index, level)) {
							if (level == 0) {
								sprintf(buffer1, "%d", index);
								(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
								tokenLastAsm = ParserAsm::LocalVariablesPushSymbol;
								tokenLastValue = token_->value;
								tokenLastIndex = index;
								return assemble1(ParserAsm::LocalVariablesPushSymbol, buffer1);
							} else {
								sprintf(buffer1, "%d", index);
								sprintf(buffer2, "%d", level);
								(functionHint.head)->value |= ParserFunctionHint::LocalVariablesLevel;
								setVariablesLevelHint(level);
								tokenLastAsm = ParserAsm::LocalVariablesLevelPushSymbol;
								tokenLastValue = token_->value;
								tokenLastIndex = index;
								tokenLastLevel = level;
								return assembleX(ParserAsm::LocalVariablesLevelPushSymbol, buffer1, buffer2);
							};
						} else {
							if (token_->is("arguments")) {
								tokenLastAsm = ParserAsm::PushArguments;
								tokenLastValue = token_->value;
								return assemble(ParserAsm::PushArguments);
							} else {
								tokenLastAsm = ParserAsm::PushSymbol;
								tokenLastValue = token_->value;
								return assemble1(ParserAsm::PushSymbol, token_->value);
							};
						};
					};
				} else {
					error = ParserError::Compile;
					return nullptr;
				};
			};
			return nullptr;
		};

		bool Parser::expressionTermPrefix(Token *backToken) {
			Token localToken;

			localToken.init(token);
			if (localToken.is2("+", "+")) {

				if (localToken.checkIs1(".")) {
					(functionHint.head)->value |= ParserFunctionHint::This;
					assemble(ParserAsm::ContextPushThis);
					localToken.reset();
					localToken.lValue = 1;
					if(expressionTermSelector(&localToken, 0, 0, 0)) {
						if (error) {
							return false;
						};
						assemble(ParserAsm::OperatorPlusPlusLeft);
						return true;
					};
					error = ParserError::Compile;
					return false;
				};


				localToken.reset();
				if (localToken.isSymbol()) {
					localToken.lValue = 1;
					localToken.type = TokenType::FirstSymbol;
					if (expressionTermSelector(&localToken, 1, 0, 0)) {
						if (error) {
							return false;
						};
						assemble(ParserAsm::OperatorPlusPlusLeft);
						return true;
					};
					assemble(ParserAsm::OperatorPlusPlusLeft);
					return true;
				};
				error = ParserError::Compile;
				return false;
			};
			if (localToken.is1("+")) {

				if (localToken.checkIs1(".")) {
					(functionHint.head)->value |= ParserFunctionHint::This;
					assemble(ParserAsm::ContextPushThis);
					localToken.reset();
					if(expressionTermSelector(&localToken, 0, 0, 0)) {
						if (error) {
							return false;
						};
						return true;
					};
					error = ParserError::Compile;
					return false;
				};

				localToken.reset();
				if (expressionTermSymbol(&localToken, 0, 0)) {
					assemble(ParserAsm::OperatorUnaryPlus);
					return true;
				};
				if(expressionParentheses()) {
					assemble(ParserAsm::OperatorUnaryPlus);
					return true;
				};
				error = ParserError::Compile;
				return false;
			};
			if (localToken.is2("-", "-")) {

				if (localToken.checkIs1(".")) {
					(functionHint.head)->value |= ParserFunctionHint::This;
					assemble(ParserAsm::ContextPushThis);
					localToken.reset();
					localToken.lValue = 1;
					if(expressionTermSelector(&localToken, 0, 0, 0)) {
						if (error) {
							return false;
						};
						assemble(ParserAsm::OperatorMinusMinusLeft);
						return true;
					};
					error = ParserError::Compile;
					return false;
				};


				localToken.reset();
				if (localToken.isSymbol()) {
					localToken.lValue = 1;
					localToken.type = TokenType::FirstSymbol;

					if (expressionTermSelector(&localToken, 1, 0, 0)) {
						if (error) {
							return false;
						};
						assemble(ParserAsm::OperatorMinusMinusLeft);
						return true;
					};

					assemble(ParserAsm::OperatorMinusMinusLeft);
					return true;
				};
				error = ParserError::Compile;
				return false;
			};
			if (localToken.is1("-")) {
				assemble1(ParserAsm::PushNumber, "0");

				if (localToken.checkIs1(".")) {
					(functionHint.head)->value |= ParserFunctionHint::This;
					assemble(ParserAsm::ContextPushThis);
					localToken.reset();
					if(expressionTermSelector(&localToken, 0, 0, 0)) {
						if (error) {
							return false;
						};
						assemble(ParserAsm::OperatorMinus);
						return true;
					};
					error = ParserError::Compile;
					return false;
				};


				if (expressionTermSymbol(&localToken, 0, 0)) {
					assemble(ParserAsm::OperatorMinus);
					return true;
				};
				if(expressionParentheses()) {
					assemble(ParserAsm::OperatorMinus);
					return true;
				};
				error = ParserError::Compile;
				return false;
			};
			if (localToken.is1("!")) {

				if (localToken.checkIs1(".")) {
					(functionHint.head)->value |= ParserFunctionHint::This;
					assemble(ParserAsm::ContextPushThis);
					localToken.reset();
					if(expressionTermSelector(&localToken, 0, 0, 0)) {
						if (error) {
							return false;
						};
						assemble(ParserAsm::OperatorNot);
						return true;
					};
					error = ParserError::Compile;
					return false;
				};

				if (expressionTermSymbol(&localToken, 0, 0)) {
					assemble(ParserAsm::OperatorNot);
					return true;
				};
				if(expressionParentheses()) {
					assemble(ParserAsm::OperatorNot);
					return true;
				};
				error = ParserError::Compile;
				return false;
			};
			if (localToken.is1("~")) {

				if (localToken.checkIs1(".")) {
					(functionHint.head)->value |= ParserFunctionHint::This;
					assemble(ParserAsm::ContextPushThis);
					localToken.reset();
					if(expressionTermSelector(&localToken, 0, 0, 0)) {
						if (error) {
							return false;
						};
						assemble(ParserAsm::OperatorBitwiseNot);
						return true;
					};
					error = ParserError::Compile;
					return false;
				};

				if (expressionTermSymbol(&localToken, 0, 0)) {
					assemble(ParserAsm::OperatorBitwiseNot);
					return true;
				};
				if(expressionParentheses()) {
					assemble(ParserAsm::OperatorBitwiseNot);
					return true;
				};
				error = ParserError::Compile;
				return false;
			};
			if (localToken.checkIs1(".")) {
				(functionHint.head)->value |= ParserFunctionHint::This;
				assemble(ParserAsm::ContextPushThis);
				localToken.reset();
				if(expressionTermSelector(&localToken, 0, 0, 0)) {
					if (error) {
						return false;
					};
					return true;
				};
				error = ParserError::Compile;
				return false;
			};
			return false;
		};

		bool Parser::expressionTermCheckLValue() {
			if (token.checkIs2("+", "+")) {
				return true;
			} else if (token.checkIs2("-", "-")) {
				return true;
			} else if (token.checkIs2("=", "=")) {
				return false;
			};
			if (token.checkIs2("+", "=")) {
				return true;
			};
			if (token.checkIs2("-", "=")) {
				return true;
			};
			if (token.checkIs2("*", "=")) {
				return true;
			};
			if (token.checkIs2("/", "=")) {
				return true;
			};
			if (token.checkIs2("%", "=")) {
				return true;
			};
			if (token.checkIs2("|", "=")) {
				return true;
			};
			if (token.checkIs2("&", "=")) {
				return true;
			};
			if (token.checkIs3("<", "<", "=")) {
				return true;
			};
			if (token.checkIs3(">", ">", "=")) {
				return true;
			};
			if (token.checkIs4(">", ">", ">", "=")) {
				return true;
			};
			if (token.checkIs1("=")) {
				return true;
			};
			return false;
		};

		bool Parser::expressionTermProcessLValue(Token *backToken, int processBackToken) {
			if (processBackToken) {
				if (backToken->lValue == 0) {
					backToken->lValue = expressionTermCheckLValue();
				};
				expressionProcessToken(backToken);
				backToken->reset();
				if (expressionIsUnaryOperatorRight(backToken)) {
					assemble(backToken->asmType);
				};
			};
			return false;
		};

		bool Parser::expressionTermSelector(Token *backToken, int processBackToken, int level, int isNew) {
			Token localToken;
			ProgramCounter *tokenPcForCall = nullptr;
			int tokenAsmForCall = ParserAsm::Nop;
			int tokenIndexForCall = 0;
			int tokenLevelForCall = 0;
			String tokenValueForCall;


			localToken.init(token);
			if (localToken.is1(".")) {
				int requestLValue;
				int requestLValueDelete;
				requestLValue = 0;
				requestLValueDelete = 0;
				if (backToken->lValue) {
					requestLValue = 1;
					backToken->lValue = 0;
					requestLValueDelete = backToken->lValueDelete;
					backToken->lValueDelete = 0;
				};
				if (processBackToken) {
					tokenPcForCall = expressionProcessToken(backToken);
					tokenAsmForCall = tokenLastAsm;
					tokenIndexForCall = tokenLastIndex;
					tokenLevelForCall = tokenLastLevel;
					tokenValueForCall = tokenLastValue;

					// transmute to call ...
				};

				while (!localToken.isEof()) {
					localToken.reset();
					if (localToken.isSymbol()) {
						if (requestLValue) {
							localToken.lValue = 1;
							localToken.lValueDelete = requestLValueDelete;
							requestLValue = 0;
							requestLValueDelete = 0;
						};
						if (localToken.is1(".")) {
							if (token.lValue == 1) {
								localToken.lValue = 0;
								requestLValue = 1;
								requestLValueDelete = localToken.lValueDelete;
								localToken.lValueDelete = 0;
							};
							expressionProcessToken(&localToken);
							continue;
						};

						if (localToken.checkIs1("(")) {
							if (localToken.is("call")) {
								int step;
								step = 0;
								if (localToken.is1("(")) {
									if (localToken.checkIs1(")")) {
										error = ParserError::Compile;
										return false;
									};
									while (!localToken.isEof()) {
										localToken.reset();
										if (expression(0)) {
											if (step == 0) {
												step = 1;
												assemble(ParserAsm::XPushNewArray);
											} else {
												assemble(ParserAsm::XArrayPushWithTransfer);
											};
											continue;
										};
										if (localToken.is1(",")) {
											continue;
										};
										if (localToken.is1(")")) {
											assemble(ParserAsm::XCallThisModeCall);
											return true;
										};
									};
								};
								error = ParserError::Compile;
								return false;
							};
							if (localToken.is("apply")) {
								int step;
								step = 0;
								if (localToken.is1("(")) {
									if (localToken.checkIs1(")")) {
										error = ParserError::Compile;
										return false;
									};
									while (!localToken.isEof()) {
										localToken.reset();
										if (expression(0)) {
											if (step == 0) {
												++step;
												assemble(ParserAsm::XPushNewArray);
											} else {
												++step;
												assemble(ParserAsm::XArrayPushWithTransfer);
											};
											continue;
										};
										if (localToken.is1(",")) {
											continue;
										};
										if (localToken.is1(")")) {
											if(step == 1) {
												assemble(ParserAsm::PushNewArray);
												assemble(ParserAsm::XArrayPushWithTransfer);
											};
											assemble(ParserAsm::XCallThisModeApply);
											return true;
										};
									};
								};
								error = ParserError::Compile;
								return false;
							};

						};

						if (expressionTermSelector(&localToken, 1, 1, isNew)) {
							return true;
						};
						return true;
					};
					break;
				};
				error = ParserError::Compile;
				return false;
			} else if (localToken.is1("(")) {
				int requestLValue;
				int requestLValueDelete;
				int hasThis;
				requestLValue = 0;

				hasThis = 0;
				if (processBackToken) {
					if (backToken->lValue) {
						requestLValue = 1;
						backToken->lValue = 0;
						requestLValueDelete = backToken->lValueDelete;
						backToken->lValueDelete = 0;
					};

					if (
						(backToken->type == TokenType::FirstSymbol)
						|| (backToken->type == TokenType::Symbol)
					) {
						if (level) {
							hasThis = 1;
						} else {
							tokenPcForCall = expressionProcessToken(backToken);
							tokenAsmForCall = tokenLastAsm;
							tokenIndexForCall = tokenLastIndex;
							tokenLevelForCall = tokenLastLevel;
							tokenValueForCall = tokenLastValue;

						};
					} else {
						tokenPcForCall = expressionProcessToken(backToken);
						tokenAsmForCall = tokenLastAsm;
						tokenIndexForCall = tokenLastIndex;
						tokenLevelForCall = tokenLastLevel;
						tokenValueForCall = tokenLastValue;
					};
				};

				if (isNew) {
					if (hasThis) {
						assemble1(ParserAsm::Reference, backToken->value);
					};
					assemble(ParserAsm::OperatorAssignXPrototype);
				};

				assemble(ParserAsm::XPushNewArray);

				while (!localToken.isEof()) {

					if (localToken.is1(")")) {
						if (hasThis) {
							if (isNew) {
								assemble(ParserAsm::XCallThis);
							} else {
								assemble1(ParserAsm::XCallWithThisReference, backToken->value);
							};
						} else {
							if (isNew) {
								assemble(ParserAsm::XCallThis);
							} else {
								if(tokenAsmForCall == ParserAsm::PushSymbol) {
									assembler->removeProgramCounter(tokenPcForCall);
									assemble1(ParserAsm::XCallSymbol, tokenValueForCall);
									tokenAsmForCall = ParserAsm::Nop;
								} else {
									assemble(ParserAsm::XCall);
								};
							};
						};
						if (localToken.is1("(")) {
							hasThis = 0;
							assemble(ParserAsm::XPushNewArray);
							continue;
						};


						localToken.reset();
						if (requestLValue) {
							localToken.lValue = 1;
							localToken.lValueDelete = requestLValueDelete;
						};

						localToken.type = TokenType::Call;

						if (expressionTermSelector(&localToken, 1, 0, 0)) {
							return true;
						};

						return true;
					};

					localToken.reset();
					if (expression(0)) {
						if (localToken.is1(",")) {
							assemble(ParserAsm::XArrayPushWithTransfer);
							continue;
						} else if (localToken.is1(")")) {
							assemble(ParserAsm::XArrayPushWithTransfer);
							if (hasThis) {
								if (isNew) {
									assemble(ParserAsm::XCallThis);
								} else {
									assemble1(ParserAsm::XCallWithThisReference, backToken->value);
								};
							} else {
								if (isNew) {
									assemble(ParserAsm::XCallThis);
								} else {
									if(tokenAsmForCall == ParserAsm::PushSymbol) {
										assembler->removeProgramCounter(tokenPcForCall);
										assemble1(ParserAsm::XCallSymbol, tokenValueForCall);
										tokenAsmForCall = ParserAsm::Nop;
									} else {
										assemble(ParserAsm::XCall);
									};
								};
							};

							if (token.is1("(")) {
								hasThis = 0;
								assemble(ParserAsm::XPushNewArray);
								continue;
							};
							token.reset();
							if (requestLValue) {
								token.lValue = 1;
								token.lValueDelete = requestLValueDelete;
							};

							token.type = TokenType::Call;

							if (expressionTermSelector(&token, 1, 0, 0)) {
								return true;
							};

							return true;

						};

					};
					break;
				};
				error = ParserError::Compile;
				return 0;
			} else if (token.is1("[")) {
				int requestLValue;
				int requestLValueDelete;
				requestLValue = 0;
				if (processBackToken) {
					if (backToken->lValue) {
						requestLValue = 1;
						backToken->lValue = 0;
						requestLValueDelete = backToken->lValueDelete;
						backToken->lValueDelete = 0;
					};
					expressionProcessToken(backToken);
				};

				while (!token.isEof()) {
					token.reset();
					if (expression(0)) {

						if (token.is1("]")) {

							if (token.is1("[")) {
								assemble(ParserAsm::OperatorReference);
								continue;
							};
							token.reset();
							if (requestLValue) {
								token.lValue = 1;
								token.lValueDelete = requestLValueDelete;
							};
							token.type = TokenType::Index;

							if (expressionTermSelector(&token, 1, 1, 0)) {
								return true;
							};

							return true;

						};

					};
					break;
				};
				error = ParserError::Compile;
				return false;
			} else {
				expressionTermProcessLValue(backToken, processBackToken);
			};
			return false;
		};

		bool Parser::expressionIsBinaryOperator(Token *token_) {
			if (token_->is1X("=")) {
				if (token_->is1("=")) {
					if (token_->is1("=")) {
						token_->asmType = ParserAsm::OperatorEqualStrict;
						token_->associativity = TokenAssociativity::LeftToRight;
						token_->precedence = 9;
						return true;
					};
					token_->asmType = ParserAsm::OperatorEqual;
					token_->associativity = TokenAssociativity::LeftToRight;
					token_->precedence = 9;
					return true;
				};
				token_->asmType = ParserAsm::OperatorAssign;
				token_->associativity = TokenAssociativity::RightToLeft;
				token_->precedence = 1;
				token_->ignoreSpaceOrComment();
				return true;
			};
			if (token_->is1X("+")) {
				if (token_->is1("=")) {
					token_->asmType = ParserAsm::OperatorAssignPlus;
					token_->associativity = TokenAssociativity::RightToLeft;
					token_->precedence = 1;
					return true;
				};
				token_->asmType = ParserAsm::OperatorPlus;
				token_->associativity = TokenAssociativity::LeftToRight;
				token_->precedence = 12;
				token_->ignoreSpaceOrComment();
				return true;
			};
			if (token_->is1X("-")) {
				if (token_->is1("=")) {
					token_->asmType = ParserAsm::OperatorAssignMinus;
					token_->associativity = TokenAssociativity::RightToLeft;
					token_->precedence = 1;
					return true;
				};
				token_->asmType = ParserAsm::OperatorMinus;
				token_->associativity = TokenAssociativity::LeftToRight;
				token_->precedence = 12;
				token_->ignoreSpaceOrComment();
				return true;
			};
			if (token_->is1X("*")) {
				if (token_->is1("=")) {
					token_->asmType = ParserAsm::OperatorAssignMul;
					token_->associativity = TokenAssociativity::RightToLeft;
					token_->precedence = 1;
					return true;
				};
				token_->asmType = ParserAsm::OperatorMul;
				token_->associativity = TokenAssociativity::LeftToRight;
				token_->precedence = 13;
				token_->ignoreSpaceOrComment();
				return true;
			};
			if (token_->is1X("/")) {
				if (token_->is1("=")) {
					token_->asmType = ParserAsm::OperatorAssignDiv;
					token_->associativity = TokenAssociativity::RightToLeft;
					token_->precedence = 1;
					return true;
				};
				token_->asmType = ParserAsm::OperatorDiv;
				token_->associativity = TokenAssociativity::LeftToRight;
				token_->precedence = 13;
				token_->ignoreSpaceOrComment();
				return true;
			};
			if (token_->is1X("%")) {
				if (token_->is1("=")) {
					token_->asmType = ParserAsm::OperatorAssignMod;
					token_->associativity = TokenAssociativity::RightToLeft;
					token_->precedence = 1;
					return true;
				};
				token_->asmType = ParserAsm::OperatorMod;
				token_->associativity = TokenAssociativity::LeftToRight;
				token_->precedence = 13;
				token_->ignoreSpaceOrComment();
				return true;
			};
			if (token_->is2("!", "=")) {
				if (token_->is1("=")) {
					token_->asmType = ParserAsm::OperatorNotEqualStrict;
					token_->associativity = TokenAssociativity::LeftToRight;
					token_->precedence = 9;
					return true;
				};
				token_->asmType = ParserAsm::OperatorNotEqual;
				token_->associativity = TokenAssociativity::LeftToRight;
				token_->precedence = 9;
				return true;
			};
			if (token_->is1X("<")) {
				if (token_->is1("=")) {
					token_->asmType = ParserAsm::OperatorLessOrEqual;
					token_->associativity = TokenAssociativity::LeftToRight;
					token_->precedence = 10;
					return true;
				};
				if (token_->is1("<")) {
					if (token_->is1("=")) {
						token_->asmType = ParserAsm::OperatorAssignBitwiseLeftShift;
						token_->associativity = TokenAssociativity::RightToLeft;
						token_->precedence = 1;
						return true;
					};
					token_->asmType = ParserAsm::OperatorBitwiseLeftShift;
					token_->associativity = TokenAssociativity::LeftToRight;
					token_->precedence = 11;
					return true;
				};
				token_->asmType = ParserAsm::OperatorLess;
				token_->associativity = TokenAssociativity::LeftToRight;
				token_->precedence = 10;
				token_->ignoreSpaceOrComment();
				return true;
			};
			if (token_->is1X(">")) {
				if (token_->is1("=")) {
					token_->asmType = ParserAsm::OperatorGreaterOrEqual;
					token_->associativity = TokenAssociativity::LeftToRight;
					token_->precedence = 10;
					return true;
				};
				if (token_->is1(">")) {
					if (token_->is1("=")) {
						token_->asmType = ParserAsm::OperatorAssignBitwiseRightShift;
						token_->associativity = TokenAssociativity::RightToLeft;
						token_->precedence = 1;
						return true;
					};
					if (token_->is1(">")) {
						if (token_->is1("=")) {
							token_->asmType = ParserAsm::OperatorAssignBitwiseRightShiftZero;
							token_->associativity = TokenAssociativity::RightToLeft;
							token_->precedence = 1;
							return true;
						};
						token_->asmType = ParserAsm::OperatorBitwiseRightShiftZero;
						token_->associativity = TokenAssociativity::LeftToRight;
						token_->precedence = 11;
						return true;
					};
					token_->asmType = ParserAsm::OperatorBitwiseRightShift;
					token_->associativity = TokenAssociativity::LeftToRight;
					token_->precedence = 11;
					return true;
				};

				token_->asmType = ParserAsm::OperatorGreater;
				token_->associativity = TokenAssociativity::LeftToRight;
				token_->precedence = 10;
				token_->ignoreSpaceOrComment();
				return true;
			};
			if (token_->is1X("|")) {
				if (token_->is1("=")) {
					token_->asmType = ParserAsm::OperatorAssignBitwiseOr;
					token_->associativity = TokenAssociativity::RightToLeft;
					token_->precedence = 1;
					return true;
				};
				if (token_->is1("|")) {
					token_->asmType = ParserAsm::OperatorLogicalOr;
					token_->associativity = TokenAssociativity::LeftToRight;
					token_->precedence = 4;
					return true;
				};
				token_->asmType = ParserAsm::OperatorBitwiseOr;
				token_->associativity = TokenAssociativity::LeftToRight;
				token_->precedence = 6;
				token_->ignoreSpaceOrComment();
				return true;
			};
			if (token_->is1X("^")) {
				if (token_->is1("=")) {
					token_->asmType = ParserAsm::OperatorAssignBitwiseXor;
					token_->associativity = TokenAssociativity::RightToLeft;
					token_->precedence = 1;
					return true;
				};
				token_->asmType = ParserAsm::OperatorBitwiseXor;
				token_->associativity = TokenAssociativity::LeftToRight;
				token_->precedence = 7;
				token_->ignoreSpaceOrComment();
				return true;
			};
			if (token_->is1X("&")) {
				if (token_->is1("=")) {
					token_->asmType = ParserAsm::OperatorAssignBitwiseAnd;
					token_->associativity = TokenAssociativity::RightToLeft;
					token_->precedence = 1;
					return true;
				};
				if (token_->is1("&")) {
					token_->asmType = ParserAsm::OperatorLogicalAnd;
					token_->associativity = TokenAssociativity::LeftToRight;
					token_->precedence = 5;
					return true;
				};
				token_->asmType = ParserAsm::OperatorBitwiseAnd;
				token_->associativity = TokenAssociativity::LeftToRight;
				token_->precedence = 8;
				token_->ignoreSpaceOrComment();
				return true;
			};
			if (token_->is1X("?")) {
				token_->asmType = ParserAsm::OperatorConditional;
				token_->associativity = TokenAssociativity::RightToLeft;
				token_->precedence = 3;
				token_->ignoreSpaceOrComment();
				return true;
			};
			if (token_->isSymbol()) {
				if (token_->is("instanceof")) {
					token_->asmType = ParserAsm::OperatorInstanceOf;
					token_->associativity = TokenAssociativity::LeftToRight;
					token_->precedence = 10;
					return true;
				};
				if (token_->is("in")) {
					token_->asmType = ParserAsm::OperatorIn;
					token_->associativity = TokenAssociativity::LeftToRight;
					token_->precedence = 10;
					return true;
				};
				error = ParserError::Compile;
				return false;
			};
			return false;
		};

		bool Parser::expressionIsUnaryOperatorRight(Token *token_) {
			if (token_->is2("+", "+")) {
				token_->asmType = ParserAsm::OperatorPlusPlusRight;
				token_->associativity = TokenAssociativity::LeftToRight;
				token_->precedence = 15;
				token_->ignoreSpaceOrComment();
				return true;
			} else if (token_->is2("-", "-")) {
				token_->asmType = ParserAsm::OperatorMinusMinusRight;
				token_->associativity = TokenAssociativity::LeftToRight;
				token_->precedence = 15;
				token_->ignoreSpaceOrComment();
				return true;
			};
			return false;
		};

		bool Parser::expressionTermObject() {
			if (token.is1("{")) {

				assemble(ParserAsm::PushNewObject);

				while (!token.isEof()) {
					token.reset();
					if (token.is1("}")) {
						return true;
					};
					if (token.is1(",")) {
						continue;
					};
					if (token.isSymbol()) {
						assemble(ParserAsm::Duplicate);
						assemble1(ParserAsm::ReferenceObjectReference, token.value);
						if (token.is1(":")) {
							token.reset();
							if (expression(0)) {
								assemble(ParserAsm::Assign);
								continue;
							};
						};
						break;
					};
					if (token.isString()) {
						assemble(ParserAsm::Duplicate);
						assemble1(ParserAsm::ReferenceObjectReference, token.value);
						if (token.is1(":")) {
							token.reset();
							if (expression(0)) {
								assemble(ParserAsm::Assign);
								continue;
							};
						};
						break;
					};
					if (token.isNumber()) {
						assemble(ParserAsm::Duplicate);
						assemble1(ParserAsm::ReferenceObjectReference, token.value);
						if (token.is1(":")) {
							token.reset();
							if (expression(0)) {
								assemble(ParserAsm::Assign);
								continue;
							};
						};
						break;
					};
					break;
				};
				error = ParserError::Compile;
			};
			return false;
		};

		bool Parser::expressionTermArray() {
			if (token.is1("[")) {

				assemble(ParserAsm::PushNewArray);


				while (!token.isEof()) {
					token.reset();
					if (token.is1("]")) {
						return true;
					} else if (token.is1(",")) {
						continue;
					} else if (expression(0)) {
						assemble(ParserAsm::ArrayPush);
						continue;
					};
					break;
				};
				error = ParserError::Compile;
			};
			return false;
		};

	};
};


