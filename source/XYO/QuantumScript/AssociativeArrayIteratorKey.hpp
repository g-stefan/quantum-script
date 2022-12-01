// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_ASSOCIATIVEARRAYITERATORKEY_HPP
#define XYO_QUANTUMSCRIPT_ASSOCIATIVEARRAYITERATORKEY_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLEASSOCIATIVEARRAY_HPP
#	include <XYO/QuantumScript/VariableAssociativeArray.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_ITERATOR_HPP
#	include <XYO/QuantumScript/Iterator.hpp>
#endif

namespace XYO::QuantumScript {

	class AssociativeArrayIteratorKey;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::AssociativeArrayIteratorKey> : public TMemoryPoolActive<XYO::QuantumScript::AssociativeArrayIteratorKey> {};
};

namespace XYO::QuantumScript {

	class AssociativeArrayIteratorKey : public Iterator {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(AssociativeArrayIteratorKey);

		public:
			Integer index;
			TPointer<VariableAssociativeArray> sourceArray;

			inline AssociativeArrayIteratorKey(){};

			XYO_QUANTUMSCRIPT_EXPORT bool next(Variable *out);

			inline void activeDestructor() {
				sourceArray.deleteMemory();
			};
	};

};

#endif
