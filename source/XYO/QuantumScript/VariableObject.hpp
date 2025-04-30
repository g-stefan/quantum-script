// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLEOBJECT_HPP
#define XYO_QUANTUMSCRIPT_VARIABLEOBJECT_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_PROTOTYPE_HPP
#	include <XYO/QuantumScript/Prototype.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableObject;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableObject> : public TMemoryPoolActive<XYO::QuantumScript::VariableObject> {};
};

namespace XYO::QuantumScript {

	class VariableObject : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableObject);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableObject);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeObject;

		public:
			TPointerX<Property> value;
			TPointerX<Prototype> prototype;

			XYO_QUANTUMSCRIPT_EXPORT VariableObject();

			inline void activeConstructor() {
				prototype.newMemory();
				value.newMemory();
			};

			inline void activeDestructor() {
				prototype.deleteMemory();
				value.deleteMemory();
			};

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> getPropertyBySymbol(Symbol symbolId);
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> getPropertyByIndex(size_t index);
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> getPropertyByVariable(Variable *index);
			XYO_QUANTUMSCRIPT_EXPORT void setPropertyBySymbol(Symbol symbolId, Variable *value);
			XYO_QUANTUMSCRIPT_EXPORT void setPropertyByIndex(size_t index, Variable *value);
			XYO_QUANTUMSCRIPT_EXPORT void setPropertyByVariable(Variable *index, Variable *value);
			XYO_QUANTUMSCRIPT_EXPORT bool deletePropertyBySymbol(Symbol symbolId);
			XYO_QUANTUMSCRIPT_EXPORT bool deletePropertyByIndex(size_t index);
			XYO_QUANTUMSCRIPT_EXPORT bool deletePropertyByVariable(Variable *index);

			XYO_QUANTUMSCRIPT_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXPORT TPointer<Iterator> getIteratorKey();
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Iterator> getIteratorValue();

			XYO_QUANTUMSCRIPT_EXPORT static void initMemory();

			XYO_QUANTUMSCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);
			XYO_QUANTUMSCRIPT_EXPORT bool hasPropertyByVariable(Variable *variable);

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT String toString();
	};

};

#endif
