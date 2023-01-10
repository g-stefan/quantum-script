// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_OBJECTITERATORKEY_HPP
#define XYO_QUANTUMSCRIPT_OBJECTITERATORKEY_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLEOBJECT_HPP
#	include <XYO/QuantumScript/VariableObject.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_ITERATOR_HPP
#	include <XYO/QuantumScript/Iterator.hpp>
#endif

namespace XYO::QuantumScript {
	class ObjectIteratorKey;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::ObjectIteratorKey> : public TMemoryPoolActive<XYO::QuantumScript::ObjectIteratorKey> {};
};

namespace XYO::QuantumScript {

	class ObjectIteratorKey : public Iterator {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(ObjectIteratorKey);

		public:
			TPointer<Variable> value_;
			PropertyNode *value;

			inline ObjectIteratorKey(){};

			XYO_QUANTUMSCRIPT_EXPORT bool next(Variable *out);

			inline void activeDestructor() {
				value_.deleteMemory();
			};
	};

};

#endif
