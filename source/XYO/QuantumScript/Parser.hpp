// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_PARSER_HPP
#define XYO_QUANTUMSCRIPT_PARSER_HPP

#ifndef XYO_QUANTUMSCRIPT_DEPENDENCY_HPP
#	include <XYO/QuantumScript/Dependency.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_INPUT_HPP
#	include <XYO/QuantumScript/Input.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_TOKEN_HPP
#	include <XYO/QuantumScript/Token.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_ASM_HPP
#	include <XYO/QuantumScript/Asm.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_PARSERFUNCTIONHINT_HPP
#	include <XYO/QuantumScript/ParserFunctionHint.hpp>
#endif

namespace XYO::QuantumScript {

#define XYO_QUANTUMSCRIPT_PARSER_TOKEN_STACK_SIZE 8

	class ParserError {
		public:
			enum {
				None,
				Compile
			};
	};

	class ParserContext : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(ParserContext);

		public:
			bool isLoop;
			TStack<ProgramCounter *> pcBreak;
			ProgramCounter *pcContinue;

			inline ParserContext() {
				isLoop = false;
				pcContinue = nullptr;
			};
	};

	class Parser : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Parser);

		protected:
			TStack<TRedBlackTree<String, int>> functionArguments;
			TStack<TRedBlackTree<String, int>> functionLocalVariables;
			TStack<int> functionLocalVariableLevel;
			TStack<int> functionHint;
			TStack<int> functionArgumentsLevelHint;
			TStack<int> functionVariablesLevelHint;

			TPointer<Asm> assembler;
			Token token;
			uint32_t sourceSymbol;
			uint32_t sourceLineNumber;

			int tokenLastAsm;
			String tokenLastValue;
			int tokenLastIndex;
			int tokenLastLevel;

			TStack<TPointer<ParserContext>> parserContext;

			inline void parserEnterContext() {
				TPointer<ParserContext> context;
				context.newMemory();
				parserContext.push(context);
			};

			inline void parserLeaveContext() {
				parserContext.pop();
			};

		public:
			int error;

			XYO_QUANTUMSCRIPT_EXPORT Parser();

			XYO_QUANTUMSCRIPT_EXPORT bool init(Input *input_, Asm *assembler_, uint32_t sourceSymbol_);

			XYO_QUANTUMSCRIPT_EXPORT bool statementOrExpression();
			XYO_QUANTUMSCRIPT_EXPORT int parse();
			XYO_QUANTUMSCRIPT_EXPORT ProgramCounter *assembleProgramCounter(int type, ProgramCounter *value);
			XYO_QUANTUMSCRIPT_EXPORT ProgramCounter *assemble(int type);
			XYO_QUANTUMSCRIPT_EXPORT ProgramCounter *assemble1(int type, const char *value);
			XYO_QUANTUMSCRIPT_EXPORT ProgramCounter *assembleX(int type, const char *value, const char *valueX);
			XYO_QUANTUMSCRIPT_EXPORT void linkProgramCounter(ProgramCounter *old_, ProgramCounter *new_);
			XYO_QUANTUMSCRIPT_EXPORT void linkProgramCounterEnd(ProgramCounter *old_, ProgramCounter *new_);
			XYO_QUANTUMSCRIPT_EXPORT bool isBlockStatement();
			XYO_QUANTUMSCRIPT_EXPORT bool statement();

			XYO_QUANTUMSCRIPT_EXPORT bool expression(int hasSymbol);
			XYO_QUANTUMSCRIPT_EXPORT bool expressionStep(Token *backOperator, Token *levelOperator = nullptr);
			XYO_QUANTUMSCRIPT_EXPORT bool expressionParentheses();
			XYO_QUANTUMSCRIPT_EXPORT bool expressionTerm(Token *backToken, int hasSymbol, int isNew);
			XYO_QUANTUMSCRIPT_EXPORT ProgramCounter *expressionProcessToken(Token *token_);
			XYO_QUANTUMSCRIPT_EXPORT bool expressionTermPrefix(Token *backToken);
			XYO_QUANTUMSCRIPT_EXPORT bool expressionTermCheckLValue();
			XYO_QUANTUMSCRIPT_EXPORT bool expressionTermProcessLValue(Token *backToken, int processBackToken);
			XYO_QUANTUMSCRIPT_EXPORT bool expressionTermSelector(Token *backToken, int processBackToken, int level, int isNew);
			XYO_QUANTUMSCRIPT_EXPORT bool expressionTermSymbol(Token *backToken, int hasSymbol, int isNew);
			XYO_QUANTUMSCRIPT_EXPORT bool expressionIsBinaryOperator(Token *token);
			XYO_QUANTUMSCRIPT_EXPORT bool expressionTermObject();
			XYO_QUANTUMSCRIPT_EXPORT bool expressionTermArray();
			XYO_QUANTUMSCRIPT_EXPORT bool expressionIsUnaryOperatorRight(Token *token_);

			XYO_QUANTUMSCRIPT_EXPORT bool statementForSub(int hasSymbol);
			XYO_QUANTUMSCRIPT_EXPORT bool statementFunctionArgument(int level);
			XYO_QUANTUMSCRIPT_EXPORT bool statementSwitchCase();
			XYO_QUANTUMSCRIPT_EXPORT bool statementVarSub(int hasSymbol);

			XYO_QUANTUMSCRIPT_EXPORT bool statementVar();
			XYO_QUANTUMSCRIPT_EXPORT bool statementIf();
			XYO_QUANTUMSCRIPT_EXPORT bool statementFor();
			XYO_QUANTUMSCRIPT_EXPORT bool statementFunction();
			XYO_QUANTUMSCRIPT_EXPORT bool statementReturn();
			XYO_QUANTUMSCRIPT_EXPORT bool statementBreak();
			XYO_QUANTUMSCRIPT_EXPORT bool statementContinue();
			XYO_QUANTUMSCRIPT_EXPORT bool statementWhile();
			XYO_QUANTUMSCRIPT_EXPORT bool statementTry();
			XYO_QUANTUMSCRIPT_EXPORT bool statementThrow();
			XYO_QUANTUMSCRIPT_EXPORT bool statementSwitch();
			XYO_QUANTUMSCRIPT_EXPORT bool statementDo();
			XYO_QUANTUMSCRIPT_EXPORT bool statementYield();

			XYO_QUANTUMSCRIPT_EXPORT bool isFunctionArgument(String variable, int &index, int &level);
			XYO_QUANTUMSCRIPT_EXPORT bool isFunctionLocalVariable(String variable, int &index, int &level);

			XYO_QUANTUMSCRIPT_EXPORT void setArgumentsLevelHint(int level);
			XYO_QUANTUMSCRIPT_EXPORT void functionUpdateArgumentsLevelHint();
			XYO_QUANTUMSCRIPT_EXPORT void setVariablesLevelHint(int level);
			XYO_QUANTUMSCRIPT_EXPORT void functionUpdateVariablesLevelHint();

			XYO_QUANTUMSCRIPT_EXPORT void beginCapture();
			XYO_QUANTUMSCRIPT_EXPORT void beginCapture(const String prefix);
			XYO_QUANTUMSCRIPT_EXPORT String endCapture();

			static void initMemory();
	};

};

#endif
