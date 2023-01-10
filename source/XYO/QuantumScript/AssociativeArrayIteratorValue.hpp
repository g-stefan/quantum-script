// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_ASSOCIATIVEARRAYITERATORVALUE_HPP
#define XYO_QUANTUMSCRIPT_ASSOCIATIVEARRAYITERATORVALUE_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLEASSOCIATIVEARRAY_HPP
#	include <XYO/QuantumScript/VariableAssociativeArray.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_ITERATOR_HPP
#	include <XYO/QuantumScript/Iterator.hpp>
#endif

namespace XYO::QuantumScript {

	class AssociativeArrayIteratorValue;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::AssociativeArrayIteratorValue> : public TMemoryPoolActive<XYO::QuantumScript::AssociativeArrayIteratorValue> {};
};

namespace XYO::QuantumScript {

	class AssociativeArrayIteratorValue : public Iterator {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(AssociativeArrayIteratorValue);

		public:
			Integer index;
			TPointer<VariableAssociativeArray> sourceArray;

			inline AssociativeArrayIteratorValue(){};

			XYO_QUANTUMSCRIPT_EXPORT bool next(Variable *out);

			inline void activeDestructor() {
				sourceArray.deleteMemory();
			};
	};

};

#endif
