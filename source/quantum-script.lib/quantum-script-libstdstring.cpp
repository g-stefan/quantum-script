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
#include <math.h>

#include "quantum-script-libstdstring.hpp"

#include "quantum-script-variablenull.hpp"
#include "quantum-script-variableboolean.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"

//#define QUANTUM_SCRIPT_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {

		namespace LibStdString {

			static TPointer<Variable> indexOf(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- string-index-of\n");
#endif
				size_t index_;
				if (String::indexOf(
						this_->toString(),
						(arguments->index(0))->toString(),
						(arguments->index(1))->toIndex(),
						index_)) {
					return VariableNumber::newVariable((Number) index_);
				};
				return VariableNumber::newVariable((Number) - 1);
			};

			static TPointer<Variable> lastIndexOf(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- string-last-index-of\n");
#endif

				size_t index_;
				if (String::indexOfFromEnd(
						this_->toString(),
						(arguments->index(0))->toString(),
						(arguments->index(1))->toIndex(),
						index_)) {
					return VariableNumber::newVariable((Number) index_);
				};
				return VariableNumber::newVariable((Number) - 1);
			};

			static TPointer<Variable> substring(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- string-substring\n");
#endif
				String value = this_->toString();
				Number ln = (arguments->index(1))->toNumber();
				return VariableString::newVariable(
						String::substring(
							value,
							(arguments->index(0))->toIndex(),
							(isnan(ln)) ? value.length() : ((isinf(ln)) ? value.length() : ((Integer)ln))
						));
			};

			static TPointer<Variable> toLowerCaseAscii(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- string-to-lower-case-ascii\n");
#endif

				return VariableString::newVariable(String::toLowerCaseAscii(this_->toString()));
			};

			static TPointer<Variable> toUpperCaseAscii(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- string-to-upper-case-ascii\n");
#endif

				return VariableString::newVariable(String::toUpperCaseAscii(this_->toString()));
			};

			static TPointer<Variable> replace(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- string-replace\n");
#endif

				return VariableString::newVariable(
						String::replace(
							this_->toString(),
							(arguments->index(0))->toString(),
							(arguments->index(1))->toString()
						)
					);
			};

			static TPointer<Variable> matchAscii(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- string-match-ascii\n");
#endif

				return VariableBoolean::newVariable(
						String::matchAscii(this_->toString(), (arguments->index(1))->toString())
					);
			};

			static TPointer<Variable> getElement(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- string-get-element\n");
#endif

				Number x = (arguments->index(0))->toNumber();
				String this__ = this_->toString();
				if(isnan(x) || isinf(x) || signbit(x)) {
					return VariableNumber::newVariable(0);
				};
				if(x > this__.length()) {
					return VariableNumber::newVariable(0);
				};

				return VariableNumber::newVariable(this__[(int)x]);
			};


			static TPointer<Variable> trim(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- string-trim\n");
#endif

				return VariableString::newVariable(String::trimWithElement(this_->toString(), " \t\r\n"));
			};

			static TPointer<Variable> split(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- string-split\n");
#endif
				size_t index;
				size_t pos;
				int ln;
				String src = this_->toString();
				String sig = (arguments->index(0))->toString();
				TPointer<Variable> retV(VariableArray::newVariable());
				if((src.length() == 0) && (sig.length() == 0)) {
					return retV;
				};
				if(String::indexOf(src, sig, 0, index)) {
					ln = 1;
					retV->setPropertyByIndex(0, VariableString::newVariable(String::substring(src, 0, index)));
					pos = index + 1;
					while(String::indexOf(src, sig, pos, index)) {
						retV->setPropertyByIndex(ln, VariableString::newVariable(String::substring(src, pos, index - pos)));
						pos = index + 1;
						++ln;
					};
					retV->setPropertyByIndex(ln, VariableString::newVariable(String::substring(src, pos)));
					return retV;
				};
				retV->setPropertyByIndex(0, VariableString::newVariable(src));
				return retV;
			};

			static TPointer<Variable> encodeC(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- string-encodeC\n");
#endif

				return VariableString::newVariable(String::encodeC(this_->toString()));
			};

			static TPointer<Variable> encodeCX(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- string-encodeCX\n");
#endif

				return VariableString::newVariable(String::encodeCX(this_->toString()));
			};

			void initExecutive(Executive *executive, void *extensionId) {
				executive->compileStringX("var Utf8=String;");
				executive->setFunction2("String.prototype.indexOf(text,st)", indexOf);
				executive->setFunction2("String.prototype.lastIndexOf(text,st)", lastIndexOf);
				executive->setFunction2("String.prototype.substring(st,ln)", substring);
				executive->setFunction2("String.prototype.toLowerCaseAscii()", toLowerCaseAscii);
				executive->setFunction2("String.prototype.toUpperCaseAscii()", toUpperCaseAscii);
				executive->setFunction2("String.prototype.replace(what_,with_)", replace);
				executive->setFunction2("String.prototype.matchAscii(signature)", matchAscii);
				executive->setFunction2("String.prototype.getElement(pos)", getElement);
				executive->setFunction2("String.prototype.trim()", trim);
				executive->setFunction2("String.prototype.split(str)", split);
				executive->setFunction2("String.prototype.encodeC()", encodeC);
				executive->setFunction2("String.prototype.encodeCX()", encodeCX);
			};

		};
	};
};


