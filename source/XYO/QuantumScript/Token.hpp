// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_TOKEN_HPP
#define XYO_QUANTUMSCRIPT_TOKEN_HPP

#ifndef XYO_QUANTUMSCRIPT_INPUT_HPP
#	include <XYO/QuantumScript/Input.hpp>
#endif

namespace XYO::QuantumScript {

	class TokenType {
		public:
			enum {
				None,
				Symbol,
				String,
				NumberInteger,
				NumberHexadecimal,
				NumberReal,
				Call,
				Index,
				FirstSymbol
			};
	};

	class TokenAssociativity {
		public:
			enum {
				LeftToRight,
				RightToLeft
			};
	};

	class Token : public Object {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(Token);

		public:
			TPointer<Input> input;
			String value;

			int type;
			int precedence;
			int associativity;
			int lValue;
			int asmType;
			int lValueDelete;

			uint32_t *sourceLineNumber;

			XYO_QUANTUMSCRIPT_EXPORT Token();
			XYO_QUANTUMSCRIPT_EXPORT ~Token();

			XYO_QUANTUMSCRIPT_EXPORT void reset();
			XYO_QUANTUMSCRIPT_EXPORT void init(Token &in);
			XYO_QUANTUMSCRIPT_EXPORT void copy(Token &in);
			XYO_QUANTUMSCRIPT_EXPORT bool is(const char *name);
			XYO_QUANTUMSCRIPT_EXPORT bool isSymbol();
			XYO_QUANTUMSCRIPT_EXPORT bool isString();
			XYO_QUANTUMSCRIPT_EXPORT bool isNumber();
			XYO_QUANTUMSCRIPT_EXPORT bool is1(const char *value);
			XYO_QUANTUMSCRIPT_EXPORT bool is2(const char *first, const char *second);
			XYO_QUANTUMSCRIPT_EXPORT bool checkIs1(const char *value);
			XYO_QUANTUMSCRIPT_EXPORT bool checkIs2(const char *first, const char *second);
			XYO_QUANTUMSCRIPT_EXPORT bool checkIs3(const char *first, const char *second, const char *third);
			XYO_QUANTUMSCRIPT_EXPORT bool checkIs4(const char *first, const char *second, const char *third, const char *four);
			XYO_QUANTUMSCRIPT_EXPORT bool isN(const char *name);
			XYO_QUANTUMSCRIPT_EXPORT bool isSpace();
			XYO_QUANTUMSCRIPT_EXPORT bool isComment();
			XYO_QUANTUMSCRIPT_EXPORT void ignoreSpaceOrComment();
			XYO_QUANTUMSCRIPT_EXPORT bool read();
			XYO_QUANTUMSCRIPT_EXPORT bool isEof();
			XYO_QUANTUMSCRIPT_EXPORT bool is1X(const char *value);
			XYO_QUANTUMSCRIPT_EXPORT bool isSymbolX(const char *name);
	};

};

#endif
