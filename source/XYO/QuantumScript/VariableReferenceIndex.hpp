// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLEREFERENCEINDEX_HPP
#define XYO_QUANTUMSCRIPT_VARIABLEREFERENCEINDEX_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_SYMBOLLIST_HPP
#	include <XYO/QuantumScript/SymbolList.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableReferenceIndex;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableReferenceIndex> : public TMemoryPoolActive<XYO::QuantumScript::VariableReferenceIndex> {};
};

namespace XYO::QuantumScript {

	class VariableReferenceIndex : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableReferenceIndex);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableReferenceIndex);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeReferenceObjectIndex;

		public:
			TPointer<Variable> object;
			size_t index;

			XYO_QUANTUMSCRIPT_EXPORT VariableReferenceIndex();

			inline void activeDestructor() {
				object.deleteMemory();
			};

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable(Variable *object, size_t index);

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT String toString();

			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> referenceSet(Variable *value);
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> referenceGet();
			XYO_QUANTUMSCRIPT_EXPORT Number referenceToNumber();
			XYO_QUANTUMSCRIPT_EXPORT String referenceToString();
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> referenceOperatorPlus(Variable *b);
			XYO_QUANTUMSCRIPT_EXPORT void referenceSetA1(Variable *value);
	};

};

#endif
