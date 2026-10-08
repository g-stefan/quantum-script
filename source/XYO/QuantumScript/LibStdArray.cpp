// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <vector>

#include <XYO/QuantumScript/LibStdArray.hpp>
#include <XYO/QuantumScript/VariableArray.hpp>
#include <XYO/QuantumScript/VariableString.hpp>
#include <XYO/QuantumScript/VariableNull.hpp>

#include <XYO/QuantumScript/LibStdArray.Source.cpp>

// #define XYO_QUANTUMSCRIPT_DEBUG_RUNTIME

namespace XYO::QuantumScript {

	namespace LibStdArray {

		static TPointer<Variable> arrayJoin(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- array-join\n");
#endif
			if (TIsType<VariableArray>(this_)) {
				return VariableString::newVariable((static_cast<VariableArray *>(this_))->join((arguments->index(0))->toString()));
			};
			return Context::getValueUndefined();
		};

		static size_t getLength(Variable *this_) {
			return (this_->getPropertyBySymbol(Context::getSymbolLength()))->toIndex();
		};

		static TPointer<Variable> arrayPush(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- array-push\n");
#endif
			size_t k;
			size_t length = arguments->value->length();
			if (TIsType<VariableArray>(this_)) {
				Array *array = (static_cast<VariableArray *>(this_))->value.value();
				for (k = 0; k < length; ++k) {
					array->push() = arguments->index(k).value();
				};
				return Context::getValueUndefined();
			};
			for (k = 0; k < length; ++k) {
				this_->setPropertyByIndex(getLength(this_), arguments->index(k));
			};
			return Context::getValueUndefined();
		};

		static TPointer<Variable> arrayPop(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- array-pop\n");
#endif
			size_t length = getLength(this_);
			if (length == 0) {
				return Context::getValueUndefined();
			};
			TPointer<Variable> retV(this_->getPropertyByIndex(length - 1));
			this_->deletePropertyByIndex(length - 1);
			return retV;
		};

		static TPointer<Variable> arrayShift(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- array-shift\n");
#endif
			size_t length = getLength(this_);
			if (length == 0) {
				return Context::getValueUndefined();
			};
			TPointer<Variable> retV(this_->getPropertyByIndex(0));
			if (TIsType<VariableArray>(this_)) {
				(static_cast<VariableArray *>(this_))->value->remove(0);
				return retV;
			};
			size_t k;
			for (k = 1; k < length; ++k) {
				this_->setPropertyByIndex(k - 1, this_->getPropertyByIndex(k));
			};
			this_->deletePropertyByIndex(length - 1);
			return retV;
		};

		class SortCompare {
			public:
				Variable *fnCompare; // nullptr - use Variable::compare
				bool isReverse;

				// true if left must be placed before right, keep order of equal elements (stable)
				inline bool isLeftFirst(Variable *left, Variable *right) {
					Number result;
					if (fnCompare) {
						TPointer<VariableArray> fnArguments(VariableArray::newArray());
						fnArguments->index(0) = left;
						fnArguments->index(1) = right;
						result = (fnCompare->functionApply(Context::getValueUndefined(), fnArguments))->toNumber();
					} else {
						result = left->compare(right);
					};
					if (isReverse) {
						return (result >= 0);
					};
					return (result <= 0);
				};
		};

		// Bottom-up merge sort, O(n log n) for any input, returns a new array
		static TPointer<Variable> sortArray(Variable *this_, Variable *fnCompare, bool isReverse) {
			TPointer<Variable> retV(VariableArray::newVariable());
			size_t length = getLength(this_);
			if (length == 0) {
				return retV;
			};

			SortCompare sortCompare;
			sortCompare.fnCompare = fnCompare;
			sortCompare.isReverse = isReverse;
			if (TIsTypeExact<VariableUndefined>(fnCompare) || TIsType<VariableNull>(fnCompare)) {
				sortCompare.fnCompare = nullptr;
			};

			// keep a reference to items, the compare function can change the source
			std::vector<TPointer<Variable>> items(length);
			std::vector<Variable *> bufferA(length);
			std::vector<Variable *> bufferB(length);
			size_t k;
			for (k = 0; k < length; ++k) {
				items[k] = this_->getPropertyByIndex(k);
				bufferA[k] = items[k];
			};

			Variable **source = bufferA.data();
			Variable **target = bufferB.data();
			Variable **swap;
			size_t width;
			size_t low;
			size_t middle;
			size_t high;
			size_t i;
			size_t j;
			for (width = 1; width < length; width *= 2) {
				for (low = 0; low < length; low += 2 * width) {
					middle = low + width;
					if (middle > length) {
						middle = length;
					};
					high = low + 2 * width;
					if (high > length) {
						high = length;
					};
					i = low;
					j = middle;
					k = low;
					while ((i < middle) && (j < high)) {
						if (sortCompare.isLeftFirst(source[i], source[j])) {
							target[k++] = source[i++];
							continue;
						};
						target[k++] = source[j++];
					};
					while (i < middle) {
						target[k++] = source[i++];
					};
					while (j < high) {
						target[k++] = source[j++];
					};
				};
				swap = source;
				source = target;
				target = swap;
			};

			Array *array = (static_cast<VariableArray *>(retV.value()))->value.value();
			for (k = 0; k < length; ++k) {
				array->push() = source[k];
			};
			return retV;
		};

		static TPointer<Variable> arraySort(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- array-sort\n");
#endif
			return sortArray(this_, arguments->index(0), false);
		};

		static TPointer<Variable> arrayReverseSort(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
			printf("- array-reverse-sort\n");
#endif
			return sortArray(this_, arguments->index(0), true);
		};

		void initExecutive(Executive *executive, void *extensionId) {

			executive->setFunction2("Array.prototype.join(text)", arrayJoin);
			executive->setFunction2("Array.prototype.push()", arrayPush);
			executive->setFunction2("Array.prototype.pop()", arrayPop);
			executive->setFunction2("Array.prototype.shift()", arrayShift);
			executive->setFunction2("Array.prototype.sort(fnCompare)", arraySort);
			executive->setFunction2("Array.prototype.reverseSort(fnCompare)", arrayReverseSort);

			executive->compileStringX(libStdArraySource);
		};

	};
};
