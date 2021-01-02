//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_TOKEN_HPP
#define QUANTUM_SCRIPT_TOKEN_HPP

#ifndef QUANTUM_SCRIPT_INPUT_HPP
#include "quantum-script-input.hpp"
#endif

namespace Quantum {
	namespace Script {


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

		using namespace XYO;

		class Token :
			public Object {
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

				QUANTUM_SCRIPT_EXPORT Token();
				QUANTUM_SCRIPT_EXPORT ~Token();

				QUANTUM_SCRIPT_EXPORT void reset();
				QUANTUM_SCRIPT_EXPORT void init(Token &in);
				QUANTUM_SCRIPT_EXPORT void copy(Token &in);
				QUANTUM_SCRIPT_EXPORT bool is(const char *name);
				QUANTUM_SCRIPT_EXPORT bool isSymbol();
				QUANTUM_SCRIPT_EXPORT bool isString();
				QUANTUM_SCRIPT_EXPORT bool isNumber();
				QUANTUM_SCRIPT_EXPORT bool is1(const char *value);
				QUANTUM_SCRIPT_EXPORT bool is2(const char *first, const char *second);
				QUANTUM_SCRIPT_EXPORT bool checkIs1(const char *value);
				QUANTUM_SCRIPT_EXPORT bool checkIs2(const char *first, const char *second);
				QUANTUM_SCRIPT_EXPORT bool checkIs3(const char *first, const char *second, const char *third);
				QUANTUM_SCRIPT_EXPORT bool checkIs4(const char *first, const char *second, const char *third, const char *four);
				QUANTUM_SCRIPT_EXPORT bool isN(const char *name);
				QUANTUM_SCRIPT_EXPORT bool isSpace();
				QUANTUM_SCRIPT_EXPORT bool isComment();
				QUANTUM_SCRIPT_EXPORT void ignoreSpaceOrComment();
				QUANTUM_SCRIPT_EXPORT bool read();
				QUANTUM_SCRIPT_EXPORT bool isEof();
				QUANTUM_SCRIPT_EXPORT bool is1X(const char *value);
				QUANTUM_SCRIPT_EXPORT bool isSymbolX(const char *name);

		};


	};
};


#endif
