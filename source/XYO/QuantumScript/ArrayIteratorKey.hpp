// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_ARRAYITERATORKEY_HPP
#define XYO_QUANTUMSCRIPT_ARRAYITERATORKEY_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLEARRAY_HPP
#	include <XYO/QuantumScript/VariableArray.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_ITERATOR_HPP
#	include <XYO/QuantumScript/Iterator.hpp>
#endif

namespace XYO::QuantumScript {
	class ArrayIteratorKey;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::ArrayIteratorKey> : public TMemoryPoolActive<XYO::QuantumScript::ArrayIteratorKey> {};
};

namespace XYO::QuantumScript {

	class ArrayIteratorKey : public Iterator {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(ArrayIteratorKey);

		public:
			Integer index;
			TPointer<VariableArray> sourceArray;

			inline ArrayIteratorKey(){};

			XYO_QUANTUMSCRIPT_EXPORT bool next(Variable *out);

			inline void activeDestructor() {
				sourceArray.deleteMemory();
			};
	};

};

#endif
