// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/Parser.hpp>
#include <XYO/QuantumScript/ParserAsm.hpp>

namespace XYO::QuantumScript {

	Parser::Parser() {
		tokenLastAsm = ParserAsm::Nop;
		tokenLastValue = "";
		tokenLastIndex = 0;
		tokenLastLevel = 0;
		error = ParserError::None;
		//
		// safeguard, you can have "this" without function body
		//
		int kx_ = ParserFunctionHint::None;
		functionHint.push(kx_);

		//
		// safeguard, continue without for/while
		//
		parserEnterContext();
	};

	bool Parser::isFunctionArgument(String variable, int &index, int &level) {
		TStack<TRedBlackTree<String, int>>::Node *arguments(functionArguments.head);
		level = 0;
		for (; arguments; arguments = arguments->next, ++level) {
			if (arguments->value.get(variable, index)) {
				return true;
			};
		};
		return false;
	};

	bool Parser::isFunctionLocalVariable(String variable, int &index, int &level) {
		TStack<TRedBlackTree<String, int>>::Node *variables(functionLocalVariables.head);
		level = 0;
		for (; variables; variables = variables->next, ++level) {
			if (variables->value.get(variable, index)) {
				return true;
			};
		};
		return false;
	};

	bool Parser::init(Input *input_, Asm *assembler_, uint32_t sourceSymbol_) {
		error = ParserError::None;
		assembler = assembler_;
		token.input = input_;
		token.sourceLineNumber = &sourceLineNumber;

		sourceSymbol = sourceSymbol_;
		return true;
	};

	bool Parser::statementOrExpression() {
		if (token.is1(";")) {
			return true;
		};
		if (statement()) {
			return true;
		} else if (expression(0)) {
			assemble(ParserAsm::Pop1);
			if (token.is1(";")) {
				return true;
			};
			error = ParserError::Compile;
			return false;
		};
		return false;
	};

	int Parser::parse() {
		error = ParserError::None;
		if (token.read()) {
			if (statementOrExpression()) {
				if (error) {
					return error;
				};

				while (!token.isEof()) {
					if (statementOrExpression()) {
						if (error) {
							break;
						};
						continue;
					} else {
						error = ParserError::Compile;
					};
					break;
				};
			} else {
				error = ParserError::Compile;
			};
		};
		return error;
	};

	ProgramCounter *Parser::assembleProgramCounter(int type, ProgramCounter *value) {
		ProgramCounter *retV;
		retV = assembler->assembleProgramCounter(type, value, sourceSymbol, sourceLineNumber);
		if (retV == nullptr) {
			error = ParserError::Compile;
		};
		return retV;
	};

	ProgramCounter *Parser::assemble(int type) {
		ProgramCounter *retV;
		retV = assembler->assemble(type, "", sourceSymbol, sourceLineNumber);
		if (retV == nullptr) {
			error = ParserError::Compile;
		};
		return retV;
	};

	ProgramCounter *Parser::assemble1(int type, const char *value) {
		ProgramCounter *retV;
		retV = assembler->assemble(type, value, sourceSymbol, sourceLineNumber);
		if (retV == nullptr) {
			error = ParserError::Compile;
		};
		return retV;
	};

	ProgramCounter *Parser::assembleX(int type, const char *value, const char *valueX) {
		ProgramCounter *retV;
		retV = assembler->assembleX(type, value, valueX, sourceSymbol, sourceLineNumber);
		if (retV == nullptr) {
			error = ParserError::Compile;
		};
		return retV;
	};

	void Parser::linkProgramCounter(ProgramCounter *old_, ProgramCounter *new_) {
		assembler->linkProgramCounter(old_, new_);
	};

	void Parser::linkProgramCounterEnd(ProgramCounter *old_, ProgramCounter *new_) {
		assembler->linkProgramCounterEnd(old_, new_);
	};

	bool Parser::isBlockStatement() {
		if (token.is1("{")) {
			while (!token.isEof()) {
				if (token.is1("}")) {
					return true;
				} else if (statementOrExpression()) {
					if (error) {
						return false;
					};
					continue;
				};
				error = ParserError::Compile;
				return false;
			};
			error = ParserError::Compile;
			return false;
		};
		return false;
	};

	bool Parser::statement() {
		if (statementVar()) {
			return true;
		} else if (statementIf()) {
			return true;
		} else if (statementFor()) {
			return true;
		} else if (statementFunction()) {
			return true;
		} else if (statementReturn()) {
			return true;
		} else if (statementBreak()) {
			return true;
		} else if (statementContinue()) {
			return true;
		} else if (statementWhile()) {
			return true;
		} else if (statementTry()) {
			return true;
		} else if (statementThrow()) {
			return true;
		} else if (statementSwitch()) {
			return true;
		} else if (statementDo()) {
			return true;
		} else if (statementYield()) {
			return true;
		};
		return false;
	};

	void Parser::beginCapture() {
		token.input->beginCapture();
	};

	void Parser::beginCapture(const String prefix) {
		token.input->beginCapture(prefix);
	};

	String Parser::endCapture() {
		return token.input->endCapture();
	};

	void Parser::initMemory() {
		TStack<TRedBlackTree<String, int>>::initMemory();
		TStack<int>::initMemory();
	};

};
