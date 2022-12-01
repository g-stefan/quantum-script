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

	bool Parser::statementSwitch() {
		if (token.isSymbolX("switch")) {
			ProgramCounter *linkSwitchBreak;

			if (expressionParentheses()) {

				if (token.is1("{")) {

					parserEnterContext();
					assemble(ParserAsm::EnterContext);
					linkSwitchBreak = assembleProgramCounter(ParserAsm::ContextSetBreak, nullptr);
					assemble(ParserAsm::ContextSetRegisterValue);

					if (statementSwitchCase()) {
						if (token.is1("}")) {

							parserLeaveContext();
							assemble(ParserAsm::LeaveContext);
							linkProgramCounter(linkSwitchBreak,
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

	bool Parser::statementSwitchCase() {
		if (token.isSymbolX("case")) {
			ProgramCounter *linkCaseIf;
			ProgramCounter *linkCaseSkip;

			if (expression(0)) {

				if (token.is1(":")) {

					assemble(ParserAsm::ContextPushRegisterValue);
					assemble(ParserAsm::OperatorEqual);
					linkCaseIf = assembleProgramCounter(ParserAsm::IfFalseGoto, nullptr);

					while (!token.isEof()) {
						if (token.isSymbolX("case")) {
							linkCaseSkip = assembleProgramCounter(ParserAsm::Goto, nullptr);
							linkProgramCounter(linkCaseIf,
							                   assemble(ParserAsm::Mark));
							if (expression(0)) {
								if (token.is1(":")) {
									assemble(ParserAsm::ContextPushRegisterValue);
									assemble(ParserAsm::OperatorEqual);
									linkCaseIf = assembleProgramCounter(ParserAsm::IfFalseGoto, nullptr);

									linkProgramCounter(linkCaseSkip,
									                   assemble(ParserAsm::Mark));

									continue;
								};
							};
						};
						if (token.isSymbolX("default")) {
							if (token.is1(":")) {
								if (linkCaseIf != nullptr) {
									linkProgramCounter(linkCaseIf,
									                   assemble(ParserAsm::Mark));
								};
								linkCaseIf = nullptr;
								continue;
							};
							error = ParserError::Compile;
							return false;
						};
						if (token.checkIs1("{")) {
							if (isBlockStatement()) {
								continue;
							};
							error = ParserError::Compile;
							return false;
						};
						if (token.checkIs1("}")) {
							if (linkCaseIf != nullptr) {
								linkProgramCounter(linkCaseIf,
								                   assemble(ParserAsm::Mark));
							};
							return true;
						};
						if (statementOrExpression()) {
							continue;
						};
						break;
					};
				};
			};

			error = ParserError::Compile;
			return false;
		};
		return false;
	};

};
