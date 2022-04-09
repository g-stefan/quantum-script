//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-input.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		Input::Input() {
			inputChar = 0;
			stack.length = 0;
			lineNumber = 1;
			error = InputError::None;
			doCapture = false;
		};

		Input::~Input(){};

		bool Input::init(IRead *input_) {
			input = input_;
			inputChar = 0;
			stack.length = 0;
			lineNumber = 1;
			error = InputError::None;
			doCapture = false;
			capture = "";
			return true;
		};

		bool Input::push() {
			if (stack.length == QUANTUM_SCRIPT_INPUT_STACK_SIZE) {
				return false;
			};
			stack.stack[stack.length] = inputChar;
			++stack.length;
			return true;
		};

		bool Input::pop() {
			--stack.length;
			inputChar = stack.stack[stack.length];
			return true;
		};

		bool Input::read() {
			if (stack.length) {
				return pop();
			};
			if (error == InputError::Eof) {
				inputChar = 0;
				return false;
			};
			if (input->read(&inputChar, sizeof(uint8_t))) {
				if (doCapture) {
					capture += inputChar;
				};
				if (inputChar == '\n') {
					++lineNumber;
				};
				return true;
			};
			inputChar = 0;
			error = InputError::Eof;
			return false;
		};

		bool Input::isN(const char *name) {
			int k;
			for (k = 0; name[k] != 0; ++k) {
				if (inputChar == name[k]) {
					if (read()) {
						continue;
					};
				};
				break;
			};
			if (name[k] == 0) {
				return true;
			};
			for (--k; k >= 0; --k) {
				push();
				inputChar = name[k];
			};
			return false;
		};

	};
};
