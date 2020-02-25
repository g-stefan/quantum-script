//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_INPUT_HPP
#define QUANTUM_SCRIPT_INPUT_HPP

#ifndef QUANTUM_SCRIPT__DEPENDENCY_HPP
#include "quantum-script--dependency.hpp"
#endif

#define QUANTUM_SCRIPT_INPUT_STACK_SIZE 32

namespace Quantum {
	namespace Script {

		class InputError {
			public:
				enum {
					None,
					Eof
				};
		};

		using namespace XYO;

		typedef struct s_InputStack {
			char stack[QUANTUM_SCRIPT_INPUT_STACK_SIZE];
			int length;
		} InputStack;

		class Input :
			public Object {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(Input);
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

				QUANTUM_SCRIPT_EXPORT Input();
				QUANTUM_SCRIPT_EXPORT ~Input();

				QUANTUM_SCRIPT_EXPORT bool init(IRead *input_);
				QUANTUM_SCRIPT_EXPORT bool push();
				QUANTUM_SCRIPT_EXPORT bool read();

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

				QUANTUM_SCRIPT_EXPORT bool isN(const char *name);

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
};


#endif
