// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_INPUT_HPP
#define XYO_QUANTUMSCRIPT_INPUT_HPP

#ifndef XYO_QUANTUMSCRIPT_DEPENDENCY_HPP
#	include <XYO/QuantumScript/Dependency.hpp>
#endif

#define XYO_QUANTUMSCRIPT_INPUT_STACK_SIZE 32

namespace XYO::QuantumScript {

	class InputError {
		public:
			enum {
				None,
				Eof
			};
	};

	typedef struct s_InputStack {
			char stack[XYO_QUANTUMSCRIPT_INPUT_STACK_SIZE];
			int length;
	} InputStack;

	class Input : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Input);

		protected:
			TPointer<IRead> input;
			InputStack stack;

			bool pop();

		public:
			int error;
			uint32_t lineNumber;
			char inputChar;
			bool doCapture;
			String capture;

			XYO_QUANTUMSCRIPT_EXPORT Input();
			XYO_QUANTUMSCRIPT_EXPORT ~Input();

			XYO_QUANTUMSCRIPT_EXPORT bool init(IRead *input_);
			XYO_QUANTUMSCRIPT_EXPORT bool push();
			XYO_QUANTUMSCRIPT_EXPORT bool read();

			inline bool isEof() {
				return (error == InputError::Eof);
			};

			inline bool is(const char *inputChar_) {
				return (inputChar == inputChar_[0]);
			};

			inline void set(const char *inputChar_) {
				inputChar = inputChar_[0];
			};

			inline bool is1(const char *inputChar_) {
				if (inputChar == inputChar_[0]) {
					read();
					return true;
				};
				return false;
			};

			XYO_QUANTUMSCRIPT_EXPORT bool isN(const char *name);

			inline void beginCapture() {
				doCapture = true;
				capture = "";
			};

			inline void beginCapture(const String prefix) {
				doCapture = true;
				capture = prefix;
			};

			inline String endCapture() {
				String retV(capture);
				doCapture = false;
				capture = "";
				return retV;
			};
	};

};

#endif
