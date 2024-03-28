// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_OBJECTITERATORVALUE_HPP
#define XYO_QUANTUMSCRIPT_OBJECTITERATORVALUE_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLEOBJECT_HPP
#	include <XYO/QuantumScript/VariableObject.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_ITERATOR_HPP
#	include <XYO/QuantumScript/Iterator.hpp>
#endif

namespace XYO::QuantumScript {

	class ObjectIteratorValue;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::ObjectIteratorValue> : public TMemoryPoolActive<XYO::QuantumScript::ObjectIteratorValue> {};
};

namespace XYO::QuantumScript {

	class ObjectIteratorValue : public Iterator {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(ObjectIteratorValue);

		public:
			TPointer<Variable> value_;
			PropertyNode *value;
			TRedBlackTree<uint32_t, String, TMemoryPoolActive> *mirrorList;

			inline ObjectIteratorValue(){};

			XYO_QUANTUMSCRIPT_EXPORT bool next(Variable *out);

			inline void activeDestructor() {
				value_.deleteMemory();
			};
	};

};

#endif
