// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLEARRAY_HPP
#define XYO_QUANTUMSCRIPT_VARIABLEARRAY_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableArray;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableArray> : public TMemoryPoolActive<XYO::QuantumScript::VariableArray> {};
};

namespace XYO::QuantumScript {

	class VariableArray : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableArray);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableArray);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeArray;

		public:
			TPointerX<Array> value;

			XYO_QUANTUMSCRIPT_EXPORT VariableArray();

			inline void activeDestructor() {
				value->activeDestructor();
			};

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable();

			inline static VariableArray *newArray() {
				return TMemory<VariableArray>::newMemory();
			};

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> getPropertyBySymbol(Symbol symbolId);
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> getPropertyByIndex(size_t index);
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> getPropertyByVariable(Variable *index);
			XYO_QUANTUMSCRIPT_EXPORT void setPropertyByIndex(size_t index, Variable *value);
			XYO_QUANTUMSCRIPT_EXPORT void setPropertyByVariable(Variable *index, Variable *value);
			XYO_QUANTUMSCRIPT_EXPORT bool deletePropertyByIndex(size_t index);
			XYO_QUANTUMSCRIPT_EXPORT bool deletePropertyByVariable(Variable *index);

			inline TPointerX<Variable> &index(uint32_t index_) {
				TPointerX<Variable> &retV = (*value)[index_];
				if (!retV) {
					retV = VariableUndefined::newVariable();
				};
				return retV;
			};

			XYO_QUANTUMSCRIPT_EXPORT Variable *instancePrototype();
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Iterator> getIteratorKey();
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Iterator> getIteratorValue();
			XYO_QUANTUMSCRIPT_EXPORT bool hasPropertyByVariable(Variable *variable);

			XYO_QUANTUMSCRIPT_EXPORT static void initMemory();

			XYO_QUANTUMSCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT String toString();

			XYO_QUANTUMSCRIPT_EXPORT String join(String with_);
	};

};

#endif
