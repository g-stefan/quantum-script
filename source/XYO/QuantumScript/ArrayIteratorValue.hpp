// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_ARRAYITERATORVALUE_HPP
#define XYO_QUANTUMSCRIPT_ARRAYITERATORVALUE_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLEARRAY_HPP
#	include <XYO/QuantumScript/VariableArray.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_ITERATOR_HPP
#	include <XYO/QuantumScript/Iterator.hpp>
#endif

namespace XYO::QuantumScript {
	class ArrayIteratorValue;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::ArrayIteratorValue> : public TMemoryPoolActive<XYO::QuantumScript::ArrayIteratorValue> {};
};

namespace XYO::QuantumScript {

	class ArrayIteratorValue : public Iterator {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(ArrayIteratorValue);

		public:
			Integer index;
			TPointer<VariableArray> sourceArray;

			inline ArrayIteratorValue(){};

			XYO_QUANTUMSCRIPT_EXPORT bool next(Variable *out);

			inline void activeDestructor() {
				sourceArray.deleteMemory();
			};
	};

};

#endif
