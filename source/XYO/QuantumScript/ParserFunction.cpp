// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/Parser.hpp>
#include <XYO/QuantumScript/ParserAsm.hpp>

namespace XYO::QuantumScript {

	bool Parser::statementFunctionArgument(int level) {
		token.reset();
		if (token.isSymbol()) {
			if (token.is("arguments")) {
				error = ParserError::Compile;
				return false;
			};
			if (token.is1(",")) {
				(functionArguments.head)->value.set(token.value, level);
				if (statementFunctionArgument(level + 1)) {
					return true;
				} else {
					return false;
				};
			};
			if (token.is1(")")) {
				(functionArguments.head)->value.set(token.value, level);
				return true;
			};
		};
		error = ParserError::Compile;
		return false;
	};

	bool Parser::statementFunction() {
		if (token.isSymbolX("function")) {
			ProgramCounter *linkFunctionBegin;
			ProgramCounter *linkFunctionEnd;
			ProgramCounter *linkFunctionDone;

			ProgramCounter *sourceFunctionBegin;
			int isAnonymous;

			uint32_t fnBeginSourceSymbol;
			uint32_t fnBeginSourceLineNumber;

			isAnonymous = 0;
			if (token.checkIs1("(")) {
				isAnonymous = 1;
			} else {
				token.reset();
				if (token.isSymbol()) {

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
				} else {
					error = ParserError::Compile;
					return false;
				};
			};

			linkFunctionBegin = assembleProgramCounter(ParserAsm::XPushFunction, nullptr);

			linkFunctionEnd = assembleProgramCounter(ParserAsm::Goto, nullptr);

			sourceFunctionBegin = assemble(ParserAsm::Nop);
			linkProgramCounter(linkFunctionBegin, sourceFunctionBegin);

			fnBeginSourceSymbol = sourceSymbol;
			fnBeginSourceLineNumber = sourceLineNumber;

			beginCapture("function(");

			if (token.is1("(")) {
				functionArguments.push();
				(functionArguments.head)->value.empty();

				if (token.is1(")")) {
				} else if (statementFunctionArgument(0)) {
				} else {
					error = ParserError::Compile;
					return false;
				};

				functionLocalVariables.push();
				(functionLocalVariables.head)->value.empty();
				functionLocalVariableLevel.push();
				(functionLocalVariableLevel.head)->value = 0;

				int k1_ = ParserFunctionHint::None;
				functionHint.push(k1_);
				k1_ = 0;
				functionArgumentsLevelHint.push(k1_);
				functionVariablesLevelHint.push(k1_);

				parserEnterContext();

				if (isBlockStatement()) {

					parserLeaveContext();

					char buffer1[32];
					char buffer2[32];

					assemble(ParserAsm::PushUndefined);
					assemble(ParserAsm::Return);

					functionArguments.pop();
					functionLocalVariables.pop();
					functionLocalVariableLevel.pop();

					if (isAnonymous) {

						linkFunctionDone = assemble(ParserAsm::Nop);

						linkProgramCounter(linkFunctionEnd, assemble(ParserAsm::Mark));
						linkProgramCounterEnd(linkFunctionBegin, linkFunctionDone);

					} else {

						linkFunctionDone = assemble(ParserAsm::Nop);
						linkProgramCounter(linkFunctionEnd, assemble(ParserAsm::Assign));
						linkProgramCounterEnd(linkFunctionBegin, linkFunctionDone);
					};

					String source = endCapture();
					String fnSource = source.substring(0, source.length() - 1);
					uint32_t fnSourceSymbol = Context::getSymbol(fnSource);

					assembler->linkProgramCounterSource(linkFunctionBegin, fnSourceSymbol, 0);

					assembler->linkProgramCounterSource(sourceFunctionBegin, fnBeginSourceSymbol, fnBeginSourceLineNumber);
					assembler->linkProgramCounterSource(linkFunctionDone, sourceSymbol, sourceLineNumber);

					sprintf(buffer1, "%p", linkFunctionBegin);
					sprintf(buffer2, "%d", (functionHint.head)->value);
					assembleX(ParserAsm::FunctionHint, buffer1, buffer2);

					functionHint.pop();

					functionUpdateArgumentsLevelHint();
					functionArgumentsLevelHint.pop();

					functionUpdateVariablesLevelHint();
					functionVariablesLevelHint.pop();

					return true;
				};

				parserLeaveContext();
			};

			endCapture();
			error = ParserError::Compile;
			return false;
		};
		return false;
	};

	void Parser::setArgumentsLevelHint(int level) {
		if (level > (functionArgumentsLevelHint.head)->value) {
			(functionArgumentsLevelHint.head)->value = level;
		};
	};

	void Parser::functionUpdateArgumentsLevelHint() {
		TStack<int>::Node *hint;
		int level = (functionArgumentsLevelHint.head)->value;
		if (level) {
			for (hint = functionHint.head; hint; hint = hint->next) {
				--level;
				if (level == 0) {
					hint->value |= ParserFunctionHint::Arguments;
					break;
				} else {
					hint->value |= ParserFunctionHint::Arguments;
					hint->value |= ParserFunctionHint::ArgumentsLevel;
				};
			};
		};
	};

	void Parser::setVariablesLevelHint(int level) {
		if (level > (functionVariablesLevelHint.head)->value) {
			(functionVariablesLevelHint.head)->value = level;
		};
	};

	void Parser::functionUpdateVariablesLevelHint() {
		TStack<int>::Node *hint;
		int level = (functionVariablesLevelHint.head)->value;
		if (level) {
			for (hint = functionHint.head; hint; hint = hint->next) {
				--level;
				if (level == 0) {
					hint->value |= ParserFunctionHint::LocalVariables;
					break;
				} else {
					hint->value |= ParserFunctionHint::LocalVariables;
					hint->value |= ParserFunctionHint::LocalVariablesLevel;
				};
			};
		};
	};

};
