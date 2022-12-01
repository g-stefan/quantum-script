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

	bool Parser::statementYield() {
		if (token.isSymbolX("yield")) {

			(functionHint.head)->value |= ParserFunctionHint::Yield;

			if (token.checkIs1(";")) {
				assemble(ParserAsm::PushUndefined);
				assemble(ParserAsm::Yield);
				return true;
			};
			if (expression(0)) {
				assemble(ParserAsm::Yield);
				return true;
			};
			error = ParserError::Compile;
			return false;
		};
		return false;
	};

};
