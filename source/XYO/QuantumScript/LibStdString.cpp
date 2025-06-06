﻿// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript/LibStdString.hpp>

#include <XYO/QuantumScript/VariableNull.hpp>
#include <XYO/QuantumScript/VariableBoolean.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>
#include <XYO/QuantumScript/VariableString.hpp>

// #define XYO_QUANTUMSCRIPT_DEBUG_RUNTIME

namespace XYO::QuantumScript {

	namespace LibStdString {

		static TPointer<Variable> indexOf(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- string-index-of\n");
#endif
			size_t index_;
			if (this_->toString().indexOf(
			        (arguments->index(0))->toString(),
			        (arguments->index(1))->toIndex(),
			        index_)) {
				return VariableNumber::newVariable((Number)index_);
			};
			return VariableNumber::newVariable((Number)-1);
		};

		static TPointer<Variable> lastIndexOf(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- string-last-index-of\n");
#endif

			size_t index_;
			if (this_->toString().indexOfFromEnd(
			        (arguments->index(0))->toString(),
			        (arguments->index(1))->toIndex(),
			        index_)) {
				return VariableNumber::newVariable((Number)index_);
			};
			return VariableNumber::newVariable((Number)-1);
		};

		static TPointer<Variable> substring(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- string-substring\n");
#endif
			String value = this_->toString();
			Number ln = (arguments->index(1))->toNumber();
			return VariableString::newVariable(
			    value.substring(
			        (arguments->index(0))->toIndex(),
			        (isnan(ln)) ? value.length() : ((isinf(ln)) ? value.length() : ((Integer)ln))));
		};

		static TPointer<Variable> toLowerCaseASCII(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- string-to-lower-case-ascii\n");
#endif

			return VariableString::newVariable(this_->toString().toLowerCaseASCII());
		};

		static TPointer<Variable> toUpperCaseASCII(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- string-to-upper-case-ascii\n");
#endif

			return VariableString::newVariable(this_->toString().toUpperCaseASCII());
		};

		static TPointer<Variable> replace(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- string-replace\n");
#endif

			return VariableString::newVariable(
			    this_->toString().replace(
			        (arguments->index(0))->toString(),
			        (arguments->index(1))->toString()));
		};

		static TPointer<Variable> matchASCII(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- string-match-ascii\n");
#endif

			return VariableBoolean::newVariable(this_->toString().matchASCII((arguments->index(1))->toString()));
		};

		static TPointer<Variable> getElement(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- string-get-element\n");
#endif

			Number x = (arguments->index(0))->toNumber();
			String this__ = this_->toString();
			if (isnan(x) || isinf(x) || signbit(x)) {
				return VariableNumber::newVariable(0);
			};
			if (x > this__.length()) {
				return VariableNumber::newVariable(0);
			};

			return VariableNumber::newVariable(this__[(int)x]);
		};

		static TPointer<Variable> trim(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- string-trim\n");
#endif

			return VariableString::newVariable(this_->toString().trimWithElement(" \t\r\n"));
		};

		static TPointer<Variable> split(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- string-split\n");
#endif
			size_t index;
			size_t pos;
			int ln;
			String src = this_->toString();
			String sig = (arguments->index(0))->toString();
			TPointer<Variable> retV(VariableArray::newVariable());
			if ((src.length() == 0) && (sig.length() == 0)) {
				return retV;
			};
			if (src.indexOf(sig, 0, index)) {
				ln = 1;
				retV->setPropertyByIndex(0, VariableString::newVariable(src.substring(0, index)));
				pos = index + sig.length();
				while (src.indexOf(sig, pos, index)) {
					retV->setPropertyByIndex(ln, VariableString::newVariable(src.substring(pos, index - pos)));
					pos = index + sig.length();
					++ln;
				};
				retV->setPropertyByIndex(ln, VariableString::newVariable(src.substring(pos)));
				return retV;
			};
			retV->setPropertyByIndex(0, VariableString::newVariable(src));
			return retV;
		};

		static TPointer<Variable> encodeC(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- string-encodeC\n");
#endif

			return VariableString::newVariable(this_->toString().encodeC());
		};

		static TPointer<Variable> encodeCX(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- string-encodeCX\n");
#endif

			return VariableString::newVariable(this_->toString().encodeCX());
		};

		void initExecutive(Executive *executive, void *extensionId) {
			executive->setFunction2("String.prototype.indexOf(text,st)", indexOf);
			executive->setFunction2("String.prototype.lastIndexOf(text,st)", lastIndexOf);
			executive->setFunction2("String.prototype.substring(st,ln)", substring);
			executive->setFunction2("String.prototype.toLowerCaseASCII()", toLowerCaseASCII);
			executive->setFunction2("String.prototype.toUpperCaseASCII()", toUpperCaseASCII);
			executive->setFunction2("String.prototype.replace(what_,with_)", replace);
			executive->setFunction2("String.prototype.matchASCII(signature)", matchASCII);
			executive->setFunction2("String.prototype.getElement(pos)", getElement);
			executive->setFunction2("String.prototype.trim()", trim);
			executive->setFunction2("String.prototype.split(str)", split);
			executive->setFunction2("String.prototype.encodeC()", encodeC);
			executive->setFunction2("String.prototype.encodeCX()", encodeCX);
		};

	};
};
