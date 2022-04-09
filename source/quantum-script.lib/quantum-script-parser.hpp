//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_PARSER_HPP
#define QUANTUM_SCRIPT_PARSER_HPP

#ifndef QUANTUM_SCRIPT__DEPENDENCY_HPP
#	include "quantum-script--dependency.hpp"
#endif

#ifndef QUANTUM_SCRIPT_INPUT_HPP
#	include "quantum-script-input.hpp"
#endif

#ifndef QUANTUM_SCRIPT_TOKEN_HPP
#	include "quantum-script-token.hpp"
#endif

#ifndef QUANTUM_SCRIPT_ASM_HPP
#	include "quantum-script-asm.hpp"
#endif

#ifndef QUANTUM_SCRIPT_PARSERFUNCTIONHINT_HPP
#	include "quantum-script-parserfunctionhint.hpp"
#endif

namespace Quantum {
	namespace Script {

#define QUANTUM_SCRIPT_PARSER_TOKEN_STACK_SIZE 8

		class ParserError {
			public:
				enum {
					None,
					Compile
				};
		};

		using namespace XYO;

		class ParserContext : public Object {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(ParserContext);

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
				XYO_DISALLOW_COPY_ASSIGN_MOVE(Parser);

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

				QUANTUM_SCRIPT_EXPORT Parser();

				QUANTUM_SCRIPT_EXPORT bool init(Input *input_, Asm *assembler_, uint32_t sourceSymbol_);

				QUANTUM_SCRIPT_EXPORT bool statementOrExpression();
				QUANTUM_SCRIPT_EXPORT int parse();
				QUANTUM_SCRIPT_EXPORT ProgramCounter *assembleProgramCounter(int type, ProgramCounter *value);
				QUANTUM_SCRIPT_EXPORT ProgramCounter *assemble(int type);
				QUANTUM_SCRIPT_EXPORT ProgramCounter *assemble1(int type, const char *value);
				QUANTUM_SCRIPT_EXPORT ProgramCounter *assembleX(int type, const char *value, const char *valueX);
				QUANTUM_SCRIPT_EXPORT void linkProgramCounter(ProgramCounter *old_, ProgramCounter *new_);
				QUANTUM_SCRIPT_EXPORT void linkProgramCounterEnd(ProgramCounter *old_, ProgramCounter *new_);
				QUANTUM_SCRIPT_EXPORT bool isBlockStatement();
				QUANTUM_SCRIPT_EXPORT bool statement();

				QUANTUM_SCRIPT_EXPORT bool expression(int hasSymbol);
				QUANTUM_SCRIPT_EXPORT bool expressionStep(Token *backOperator, Token *levelOperator = nullptr);
				QUANTUM_SCRIPT_EXPORT bool expressionParentheses();
				QUANTUM_SCRIPT_EXPORT bool expressionTerm(Token *backToken, int hasSymbol, int isNew);
				QUANTUM_SCRIPT_EXPORT ProgramCounter *expressionProcessToken(Token *token_);
				QUANTUM_SCRIPT_EXPORT bool expressionTermPrefix(Token *backToken);
				QUANTUM_SCRIPT_EXPORT bool expressionTermCheckLValue();
				QUANTUM_SCRIPT_EXPORT bool expressionTermProcessLValue(Token *backToken, int processBackToken);
				QUANTUM_SCRIPT_EXPORT bool expressionTermSelector(Token *backToken, int processBackToken, int level, int isNew);
				QUANTUM_SCRIPT_EXPORT bool expressionTermSymbol(Token *backToken, int hasSymbol, int isNew);
				QUANTUM_SCRIPT_EXPORT bool expressionIsBinaryOperator(Token *token);
				QUANTUM_SCRIPT_EXPORT bool expressionTermObject();
				QUANTUM_SCRIPT_EXPORT bool expressionTermArray();
				QUANTUM_SCRIPT_EXPORT bool expressionIsUnaryOperatorRight(Token *token_);

				QUANTUM_SCRIPT_EXPORT bool statementForSub(int hasSymbol);
				QUANTUM_SCRIPT_EXPORT bool statementFunctionArgument(int level);
				QUANTUM_SCRIPT_EXPORT bool statementSwitchCase();
				QUANTUM_SCRIPT_EXPORT bool statementVarSub(int hasSymbol);

				QUANTUM_SCRIPT_EXPORT bool statementVar();
				QUANTUM_SCRIPT_EXPORT bool statementIf();
				QUANTUM_SCRIPT_EXPORT bool statementFor();
				QUANTUM_SCRIPT_EXPORT bool statementFunction();
				QUANTUM_SCRIPT_EXPORT bool statementReturn();
				QUANTUM_SCRIPT_EXPORT bool statementBreak();
				QUANTUM_SCRIPT_EXPORT bool statementContinue();
				QUANTUM_SCRIPT_EXPORT bool statementWhile();
				QUANTUM_SCRIPT_EXPORT bool statementTry();
				QUANTUM_SCRIPT_EXPORT bool statementThrow();
				QUANTUM_SCRIPT_EXPORT bool statementSwitch();
				QUANTUM_SCRIPT_EXPORT bool statementDo();
				QUANTUM_SCRIPT_EXPORT bool statementYield();

				QUANTUM_SCRIPT_EXPORT bool isFunctionArgument(String variable, int &index, int &level);
				QUANTUM_SCRIPT_EXPORT bool isFunctionLocalVariable(String variable, int &index, int &level);

				QUANTUM_SCRIPT_EXPORT void setArgumentsLevelHint(int level);
				QUANTUM_SCRIPT_EXPORT void functionUpdateArgumentsLevelHint();
				QUANTUM_SCRIPT_EXPORT void setVariablesLevelHint(int level);
				QUANTUM_SCRIPT_EXPORT void functionUpdateVariablesLevelHint();

				QUANTUM_SCRIPT_EXPORT void beginCapture();
				QUANTUM_SCRIPT_EXPORT void beginCapture(const String prefix);
				QUANTUM_SCRIPT_EXPORT String endCapture();

				static void initMemory();
		};

	};
};

#endif
