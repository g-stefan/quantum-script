//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-token.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		Token::Token() {
			reset();
		};

		Token::~Token() {
		};

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
				(input->inputChar == '\t')
				|| (input->inputChar == '\r')
				|| (input->inputChar == '\n')
				|| (input->inputChar == ' ')
			) {
				while (input->read()) {

					if (
						(input->inputChar == '\t')
						|| (input->inputChar == '\r')
						|| (input->inputChar == '\n')
						|| (input->inputChar == ' ')
					) {
					} else {
						return true;
					};
				};
				return true;
			};
			// UTF-8 BOM 0xEF,0xBB,0xBF ignore ...
			if(isN("\xEF\xBB\xBF")) {
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
								(input->inputChar == '\r')
								|| (input->inputChar == '\n')

							) {
								return isSpace();
							};
							while (input->read()) {
								if (
									(input->inputChar == '\r')
									|| (input->inputChar == '\n')

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
			if ((input->inputChar == '_')
				|| ((input->inputChar >= 'A') && (input->inputChar <= 'Z'))
				|| ((input->inputChar >= 'a') && (input->inputChar <= 'z'))
			) {
				type = TokenType::Symbol;
				value << input->inputChar;
				while (input->read()) {
					if (
						(input->inputChar == '_')
						|| ((input->inputChar >= 'A') && (input->inputChar <= 'Z'))
						|| ((input->inputChar >= 'a') && (input->inputChar <= 'z'))
						|| ((input->inputChar >= '0') && (input->inputChar <= '9'))

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
									char code[5];
									code[4] = 0;
									unsigned int code4;
									if(!input->read()) {
										return false;
									};
									code[0] = input->inputChar;
									if(!input->read()) {
										return false;
									};
									code[1] = input->inputChar;
									if(!input->read()) {
										return false;
									};
									code[2] = input->inputChar;
									if(!input->read()) {
										return false;
									};
									code[3] = input->inputChar;
									if(sscanf(code, "%04X", &code4) != 1) {
										return false;
									};
									code[0] = (code4 >> 8) & 0xFF;
									code[1] = (code4) & 0xFF;
									code[2] = 0;
									value.concatenate(code, 2);
									continue;
								};
								if (input->inputChar == 'x') {
									char code[3];
									code[2] = 0;
									int codeHex;
									if(!input->read()) {
										return false;
									};
									code[0] = input->inputChar;
									if(!input->read()) {
										return false;
									};
									code[1] = input->inputChar;
									if(sscanf(code, "%02X", &codeHex) != 1) {
										return false;
									};
									code[0] = (uint8_t)codeHex;
									code[1] = 0;
									value.concatenate(code, 1);
									continue;
								};
								if (input->inputChar >= '0' && input->inputChar <= '3') {
									char code[4];
									code[3] = 0;
									int codeOctal;
									if(!input->read()) {
										return false;
									};
									code[0] = input->inputChar;
									if(!input->read()) {
										return false;
									};
									code[1] = input->inputChar;
									if(!input->read()) {
										return false;
									};
									code[2] = input->inputChar;
									if(sscanf(code, "%03o", &codeOctal) != 1) {
										return false;
									};
									code[0] = (uint8_t)codeOctal;
									code[1] = 0;
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
							(input->inputChar >= '0') && (input->inputChar <= '9')
						) {

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
			if ((input->inputChar == '_')
				|| ((input->inputChar >= 'A') && (input->inputChar <= 'Z'))
				|| ((input->inputChar >= 'a') && (input->inputChar <= 'z'))
			) {
				int k = 0;
				if (name[k] == input->inputChar) {
					++k;
					while (input->read()) {
						if (
							(input->inputChar == '_')
							|| ((input->inputChar >= 'A') && (input->inputChar <= 'Z'))
							|| ((input->inputChar >= 'a') && (input->inputChar <= 'z'))
							|| ((input->inputChar >= '0') && (input->inputChar <= '9'))

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
};


