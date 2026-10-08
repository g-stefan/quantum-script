// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/Token.hpp>

namespace XYO::QuantumScript {

	static bool hexDigitValue(char value, uint32_t &out) {
		if (value >= '0' && value <= '9') {
			out = value - '0';
			return true;
		};
		if (value >= 'A' && value <= 'F') {
			out = value - 'A' + 10;
			return true;
		};
		if (value >= 'a' && value <= 'f') {
			out = value - 'a' + 10;
			return true;
		};
		return false;
	};

	// read exactly 4 hexadecimal digits
	static bool readHex4(Input *input, uint32_t &out) {
		uint32_t digit;
		int k;
		out = 0;
		for (k = 0; k < 4; ++k) {
			if (!input->read()) {
				return false;
			};
			if (!hexDigitValue(input->inputChar, digit)) {
				return false;
			};
			out = (out << 4) | digit;
		};
		return true;
	};

	static void appendUTF8(String &value, uint32_t code) {
		char buffer[4];
		if (code < 0x80) {
			buffer[0] = (char)code;
			value.concatenate(buffer, 1);
			return;
		};
		if (code < 0x800) {
			buffer[0] = (char)(0xC0 | (code >> 6));
			buffer[1] = (char)(0x80 | (code & 0x3F));
			value.concatenate(buffer, 2);
			return;
		};
		if (code < 0x10000) {
			buffer[0] = (char)(0xE0 | (code >> 12));
			buffer[1] = (char)(0x80 | ((code >> 6) & 0x3F));
			buffer[2] = (char)(0x80 | (code & 0x3F));
			value.concatenate(buffer, 3);
			return;
		};
		buffer[0] = (char)(0xF0 | (code >> 18));
		buffer[1] = (char)(0x80 | ((code >> 12) & 0x3F));
		buffer[2] = (char)(0x80 | ((code >> 6) & 0x3F));
		buffer[3] = (char)(0x80 | (code & 0x3F));
		value.concatenate(buffer, 4);
	};

	Token::Token() {
		reset();
	};

	Token::~Token(){};

	void Token::reset() {
		value = "";
		type = TokenType::None;
		precedence = 0;
		associativity = 0;
		lValue = 0;
		asmType = 0;
		lValueDelete = 0;
	};

	void Token::init(Token &in) {
		input = in.input;
		sourceLineNumber = in.sourceLineNumber;
		reset();
	};

	void Token::copy(Token &in) {
		value = in.value;
		type = in.type;
		precedence = in.precedence;
		associativity = in.associativity;
		lValue = in.lValue;
		asmType = in.asmType;
		sourceLineNumber = in.sourceLineNumber;
		lValueDelete = in.lValueDelete;
	};

	bool Token::is(const char *name) {
		return (value == name);
	};

	bool Token::isSpace() {
		if (
		    (input->inputChar == '\t') || (input->inputChar == '\r') || (input->inputChar == '\n') || (input->inputChar == ' ')) {
			while (input->read()) {

				if (
				    (input->inputChar == '\t') || (input->inputChar == '\r') || (input->inputChar == '\n') || (input->inputChar == ' ')) {
				} else {
					return true;
				};
			};
			return true;
		};
		// UTF-8 BOM 0xEF,0xBB,0xBF ignore ...
		if (isN("\xEF\xBB\xBF")) {
			return isSpace();
		};
		return false;
	};

	bool Token::isComment() {
		if (input->inputChar == '/') {
			if (input->read()) {
				if (input->inputChar == '/') {
					if (input->read()) {
						if (
						    (input->inputChar == '\r') || (input->inputChar == '\n')

						) {
							return isSpace();
						};
						while (input->read()) {
							if (
							    (input->inputChar == '\r') || (input->inputChar == '\n')

							) {
								return isSpace();
							};
						};
					};
					return true;
				} else if (input->inputChar == '*') {
					while (input->read()) {
						if (input->inputChar == '*') {
							if (input->read()) {
								if (input->inputChar == '/') {
									input->read();
									return true;
								};
							};
						};
					};
					return true;
				} else {
					input->push();
					input->set("/");
				};
			};
		};
		return false;
	};

	bool Token::isSymbol() {
		if ((input->inputChar == '_') || ((input->inputChar >= 'A') && (input->inputChar <= 'Z')) || ((input->inputChar >= 'a') && (input->inputChar <= 'z'))) {
			type = TokenType::Symbol;
			value << input->inputChar;
			while (input->read()) {
				if (
				    (input->inputChar == '_') || ((input->inputChar >= 'A') && (input->inputChar <= 'Z')) || ((input->inputChar >= 'a') && (input->inputChar <= 'z')) || ((input->inputChar >= '0') && (input->inputChar <= '9'))

				) {
					value << input->inputChar;
				} else {
					ignoreSpaceOrComment();
					return true;
				};
			};
			return true;
		};
		return false;
	};

	bool Token::isString() {
		if (input->inputChar == '"' || input->inputChar == '\'') {
			char strMark;
			strMark = input->inputChar;
			type = TokenType::String;
			while (input->read()) {
				if (input->inputChar != strMark) {
					if (input->inputChar == '\\') {
						if (input->read()) {
							if (input->inputChar == '\\') {
								value << "\\";
								continue;
							};
							if (input->inputChar == '/') {
								value << "/";
								continue;
							};
							if (input->inputChar == '"') {
								value << "\"";
								continue;
							};
							if (input->inputChar == '\'') {
								value << "'";
								continue;
							};
							if (input->inputChar == 'b') {
								value << "\x08";
								continue;
							};
							if (input->inputChar == 't') {
								value << "\x09";
								continue;
							};
							if (input->inputChar == 'n') {
								value << "\x0A";
								continue;
							};
							if (input->inputChar == 'v') {
								value << "\x0B";
								continue;
							};
							if (input->inputChar == 'f') {
								value << "\x0C";
								continue;
							};
							if (input->inputChar == 'r') {
								value << "\x0D";
								continue;
							};
							if (input->inputChar == 'u') {
								uint32_t code4;
								if (!readHex4(input, code4)) {
									return false;
								};
								// UTF-16 surrogate pair
								if (code4 >= 0xD800 && code4 <= 0xDBFF) {
									if (!input->read()) {
										return false;
									};
									if (input->inputChar != '\\') {
										appendUTF8(value, code4);
										input->push();
										continue;
									};
									if (!input->read()) {
										return false;
									};
									if (input->inputChar != 'u') {
										appendUTF8(value, code4);
										// process the escape again
										input->push();
										input->inputChar = '\\';
										input->push();
										continue;
									};
									uint32_t codeLow;
									if (!readHex4(input, codeLow)) {
										return false;
									};
									if (codeLow >= 0xDC00 && codeLow <= 0xDFFF) {
										appendUTF8(value, 0x10000 + ((code4 - 0xD800) << 10) + (codeLow - 0xDC00));
										continue;
									};
									appendUTF8(value, code4);
									appendUTF8(value, codeLow);
									continue;
								};
								appendUTF8(value, code4);
								continue;
							};
							if (input->inputChar == 'x') {
								char code[3];
								code[2] = 0;
								int codeHex;
								if (!input->read()) {
									return false;
								};
								code[0] = input->inputChar;
								if (!input->read()) {
									return false;
								};
								code[1] = input->inputChar;
								if (sscanf(code, "%02X", &codeHex) != 1) {
									return false;
								};
								code[0] = (uint8_t)codeHex;
								code[1] = 0;
								value.concatenate(code, 1);
								continue;
							};
							if (input->inputChar >= '0' && input->inputChar <= '7') {
								// 1 to 3 octal digits, max \377
								char code[1];
								int codeOctal = input->inputChar - '0';
								int k;
								for (k = 1; k < 3; ++k) {
									if (!input->read()) {
										return false;
									};
									if ((input->inputChar >= '0') && (input->inputChar <= '7') && ((codeOctal * 8 + (input->inputChar - '0')) <= 0377)) {
										codeOctal = codeOctal * 8 + (input->inputChar - '0');
										continue;
									};
									// not part of the escape, process it again
									input->push();
									break;
								};
								code[0] = (char)(uint8_t)codeOctal;
								value.concatenate(code, 1);
								continue;
							};
							return false;
						} else {
							return false;
						};
					};

					value << input->inputChar;

				} else {
					input->read();
					ignoreSpaceOrComment();
					return true;
				};
			};
		};
		return false;
	};

	bool Token::isNumber() {
		if (input->inputChar == '0') {
			read();
			if (input->inputChar == 'x') {
				// hexadecimal
				value << "0x";
				type = TokenType::NumberHexadecimal;
				while (input->read()) {
					if (

					    (input->inputChar >= '0') && (input->inputChar <= '9') ||
					    (input->inputChar >= 'A') && (input->inputChar <= 'Z') ||
					    (input->inputChar >= 'a') && (input->inputChar <= 'z')

					) {
						value << input->inputChar;
						continue;
					};
					break;
				};
				return true;
			};
			input->push();
			input->inputChar = '0';
		};

		// decimal

		if ((input->inputChar >= '0') && (input->inputChar <= '9')) {
			value << input->inputChar;
			type = TokenType::NumberInteger;
			while (input->read()) {
				if (

				    (input->inputChar >= '0') && (input->inputChar <= '9')

				) {
					value << input->inputChar;
					continue;
				};
				break;
			};

			if (input->inputChar == '.') {

				value << input->inputChar;
				type = TokenType::NumberReal;

				while (input->read()) {
					if (
					    (input->inputChar >= '0') && (input->inputChar <= '9')) {

						value << input->inputChar;
						continue;

					} else {
						ignoreSpaceOrComment();
						return true;
					};
				};
			};
			ignoreSpaceOrComment();
			return true;
		};
		return false;
	};

	bool Token::is1(const char *value) {
		if (input->is1(value)) {
			ignoreSpaceOrComment();
			return true;
		};
		return false;
	};

	bool Token::is1X(const char *value) {
		return input->is1(value);
	};

	bool Token::is2(const char *first, const char *second) {
		if (input->is1(first)) {
			if (input->is1(second)) {
				ignoreSpaceOrComment();
				return true;
			};
			input->push();
			input->set(first);
		};
		return false;
	};

	bool Token::isN(const char *name) {
		if (input->isN(name)) {
			ignoreSpaceOrComment();
			return true;
		};
		return false;
	}

	bool Token::checkIs1(const char *value) {
		return input->is(value);
	};

	bool Token::checkIs2(const char *first, const char *second) {
		if (input->is1(first)) {
			if (input->is(second)) {
				input->push();
				input->set(first);
				return true;
			};
			input->push();
			input->set(first);
		};
		return false;
	};

	bool Token::checkIs3(const char *first, const char *second, const char *third) {
		if (input->is1(first)) {
			if (checkIs2(second, third)) {
				input->push();
				input->set(first);
				return true;
			};
			input->push();
			input->set(first);
		};
		return false;
	};

	bool Token::checkIs4(const char *first, const char *second, const char *third, const char *four) {
		if (input->is1(first)) {
			if (checkIs3(second, third, four)) {
				input->push();
				input->set(first);
				return true;
			};
			input->push();
			input->set(first);
		};
		return false;
	};

	void Token::ignoreSpaceOrComment() {
		*sourceLineNumber = input->lineNumber;
		while (isSpace() || isComment()) {
		};
	};

	bool Token::read() {
		if (input->read()) {
			ignoreSpaceOrComment();
			return !input->isEof();
		};
		return false;
	};

	bool Token::isEof() {
		return input->isEof();
	};

	bool Token::isSymbolX(const char *name) {
		if ((input->inputChar == '_') || ((input->inputChar >= 'A') && (input->inputChar <= 'Z')) || ((input->inputChar >= 'a') && (input->inputChar <= 'z'))) {
			int k = 0;
			if (name[k] == input->inputChar) {
				++k;
				while (input->read()) {
					if (
					    (input->inputChar == '_') || ((input->inputChar >= 'A') && (input->inputChar <= 'Z')) || ((input->inputChar >= 'a') && (input->inputChar <= 'z')) || ((input->inputChar >= '0') && (input->inputChar <= '9'))

					) {
						if (name[k] == input->inputChar) {
							++k;
							continue;
						} else {
							break;
						};
					} else {
						if (name[k] == 0) {
							ignoreSpaceOrComment();
							return true;
						};
						break;
					};
				};

				for (; k > 0; --k) {
					input->push();
					input->inputChar = name[k - 1];
				};
			};
		};
		return false;
	};

};
