// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_PROTOTYPE_HPP
#define XYO_QUANTUMSCRIPT_PROTOTYPE_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

namespace XYO::QuantumScript {

	class Prototype;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Prototype> : public TMemoryPoolActive<XYO::QuantumScript::Prototype> {};
};

namespace XYO::QuantumScript {

	class Prototype : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Prototype);

		public:
			TPointerX<Prototype> parent;
			TPointerX<Variable> prototype;

			XYO_QUANTUMSCRIPT_EXPORT Prototype();

			inline void activeDestructor() {
				parent.deleteMemory();
				prototype.deleteMemory();
			};

			inline static void initMemory() {
				TMemory<Variable>::initMemory();
			};
	};

};

#endif
