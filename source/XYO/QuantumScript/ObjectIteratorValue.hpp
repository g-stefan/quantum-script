// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
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
			TPointer<Variable> value_; // keep owner of property alive
			Property *property;
			Symbol key;
			bool hasKey;
			bool isEnd;
			TRedBlackTree<uint32_t, String, TMemoryPoolActive> *mirrorList;

			inline ObjectIteratorValue() {
				property = nullptr;
				key = 0;
				hasKey = false;
				isEnd = false;
				mirrorList = nullptr;
			};

			inline void setProperty(Variable *owner, Property *property_) {
				value_ = owner;
				property = property_;
				key = 0;
				hasKey = false;
				isEnd = false;
			};

			inline PropertyNode *nextNode() {
				if (isEnd) {
					return nullptr;
				};
				PropertyNode *node = hasKey ? propertyNodeNext(property, key) : property->begin();
				if (node) {
					key = node->key;
					hasKey = true;
					return node;
				};
				isEnd = true;
				return nullptr;
			};

			XYO_QUANTUMSCRIPT_EXPORT bool next(Variable *out);

			inline void activeDestructor() {
				value_.deleteMemory();
			};
	};

};

#endif
