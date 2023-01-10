// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLEASSOCIATIVEARRAY_HPP
#define XYO_QUANTUMSCRIPT_VARIABLEASSOCIATIVEARRAY_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

namespace XYO::ManagedMemory {

	template <>
	class TComparator<XYO::QuantumScript::Variable> {
		public:
			inline static bool isLess(const XYO::QuantumScript::Variable &a, const XYO::QuantumScript::Variable &b) {
				return ((const_cast<XYO::QuantumScript::Variable &>(a)).compare(&(const_cast<XYO::QuantumScript::Variable &>(b))) < 0);
			};

			inline static int compare(const XYO::QuantumScript::Variable &a, const XYO::QuantumScript::Variable &b) {
				return (const_cast<XYO::QuantumScript::Variable &>(a)).compare(&(const_cast<XYO::QuantumScript::Variable &>(b)));
			};
	};

};

namespace XYO::QuantumScript {

	class VariableAssociativeArray;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableAssociativeArray> : public TMemoryPoolActive<XYO::QuantumScript::VariableAssociativeArray> {};
};

namespace XYO::QuantumScript {

	typedef TAssociativeArray<TPointerX<Variable>, TPointerX<Variable>, 4, TMemoryPoolActive> AssociativeArray;

	class VariableAssociativeArray : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableAssociativeArray);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableAssociativeArray);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeAssociativeArray;

		public:
			TPointerX<AssociativeArray> value;

			XYO_QUANTUMSCRIPT_EXPORT VariableAssociativeArray();

			inline void activeDestructor() {
				value->activeDestructor();
			};

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> getPropertyBySymbol(Symbol symbolId);
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> getPropertyByIndex(size_t index);
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> getPropertyByVariable(Variable *index);
			XYO_QUANTUMSCRIPT_EXPORT void setPropertyByIndex(size_t index, Variable *value);
			XYO_QUANTUMSCRIPT_EXPORT void setPropertyByVariable(Variable *index, Variable *value);
			XYO_QUANTUMSCRIPT_EXPORT bool deletePropertyByIndex(size_t index);
			XYO_QUANTUMSCRIPT_EXPORT bool deletePropertyByVariable(Variable *index);

			XYO_QUANTUMSCRIPT_EXPORT Variable *instancePrototype();
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Iterator> getIteratorKey();
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Iterator> getIteratorValue();

			XYO_QUANTUMSCRIPT_EXPORT static void initMemory();

			XYO_QUANTUMSCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT String toString();
	};

};

#endif
