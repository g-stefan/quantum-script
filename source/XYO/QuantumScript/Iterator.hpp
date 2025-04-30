// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_ITERATOR_HPP
#define XYO_QUANTUMSCRIPT_ITERATOR_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

namespace XYO::QuantumScript {

	class Iterator;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Iterator> : public TMemoryPoolActive<XYO::QuantumScript::Iterator> {};
};

namespace XYO::QuantumScript {

	class Iterator : public virtual Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Iterator);

		public:
			inline Iterator(){};

			XYO_QUANTUMSCRIPT_EXPORT virtual bool next(Variable *out);
	};
};

#endif
