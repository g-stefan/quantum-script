//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
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


		bool Parser::statementVar() {
			if (token.isSymbolX("var")) {
				while (!token.isEof()) {
					token.reset();
					if (token.checkIs1(";")) {
						return true;
					} else if (token.is1(",")) {
						continue;
					} else if (token.isSymbol()) {
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
							if(functionLocalVariables.head) {
								(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
							};
						} else {
							if(functionLocalVariables.head) {
								char buf[32];
								(functionLocalVariables.head)->value.set(token.value, (functionLocalVariableLevel.head)->value);
								sprintf(buf, "%d", (functionLocalVariableLevel.head)->value);
								++(functionLocalVariableLevel.head)->value;
								(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
							};
						};
						if (expression(1)) {
							assemble(ParserAsm::Pop1);
							continue;
						} else {
							return false;
						};
					};
					break;
				};
			};
			return false;
		};

		bool Parser::statementVarSub(int hasSymbol) {
			int index, level;
			bool found;
			if (hasSymbol) {
				found = false;
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
					if(functionLocalVariables.head) {
						(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
					};
				} else {
					if(functionLocalVariables.head) {
						char buf[32];
						(functionLocalVariables.head)->value.set(token.value, (functionLocalVariableLevel.head)->value);
						sprintf(buf, "%d", (functionLocalVariableLevel.head)->value);
						++(functionLocalVariableLevel.head)->value;
						(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
					};
				};
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

					found = false;
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
						if(functionLocalVariables.head) {
							(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
						};
					} else {

						if(functionLocalVariables.head) {
							char buf[32];
							(functionLocalVariables.head)->value.set(token.value, (functionLocalVariableLevel.head)->value);
							sprintf(buf, "%d", (functionLocalVariableLevel.head)->value);
							++(functionLocalVariableLevel.head)->value;
							(functionHint.head)->value |= ParserFunctionHint::LocalVariables;
						};

					};

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

	};
};


