// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLEFUNCTIONWITHYIELD_HPP
#define XYO_QUANTUMSCRIPT_VARIABLEFUNCTIONWITHYIELD_HPP

#ifndef XYO_QUANTUMSCRIPT_CONTEXT_HPP
#	include <XYO/QuantumScript/Context.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_PROTOTYPE_HPP
#	include <XYO/QuantumScript/Prototype.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_FUNCTIONPARENT_HPP
#	include <XYO/QuantumScript/FunctionParent.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_VARIABLEARRAY_HPP
#	include <XYO/QuantumScript/VariableArray.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_VARIABLEOBJECT_HPP
#	include <XYO/QuantumScript/VariableObject.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableFunctionWithYield;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableFunctionWithYield> : public TMemoryPoolActive<XYO::QuantumScript::VariableFunctionWithYield> {};
};

namespace XYO::QuantumScript {

	class VariableFunctionWithYield;
	typedef TPointer<Variable> (*FunctionProcedureWithYield)(VariableFunctionWithYield *function, Variable *this_, VariableArray *arguments);

	class VariableFunctionWithYield : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableFunctionWithYield);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableFunctionWithYield);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeFunction;

		public:
			TPointerX<Object> super;
			void *valueSuper;

			TPointerX<Property> object;
			TPointerX<Prototype> prototype;

			TPointerX<FunctionParent> functionParent;
			FunctionProcedureWithYield functionProcedure;

			uint32_t yieldStep;
			TPointerX<Variable> yieldVariables;

			XYO_QUANTUMSCRIPT_EXPORT VariableFunctionWithYield();

			inline void activeConstructor() {
				valueSuper = nullptr;

				object.newMemory();
				prototype.newMemory();
				prototype->prototype = VariableObject::newVariable();
				yieldStep = 0;
				yieldVariables = VariableArray::newVariable();
			};

			inline void activeDestructor() {
				functionParent.deleteMemory();
				object.deleteMemory();
				prototype.deleteMemory();
				yieldVariables.deleteMemory();
				super.deleteMemory();
			};

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable(FunctionParent *functionParent, VariableArray *parentVariables, VariableArray *parentArguments, FunctionProcedureWithYield functionProcedure, Object *super, void *valueSuper);

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

			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> functionApply(Variable *this_, VariableArray *arguments);

			XYO_QUANTUMSCRIPT_EXPORT Variable *instancePrototype();
			XYO_QUANTUMSCRIPT_EXPORT bool instanceOfPrototype(Prototype *&out);

			XYO_QUANTUMSCRIPT_EXPORT TPointer<Iterator> getIteratorKey();
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Iterator> getIteratorValue();

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT String toString();

			XYO_QUANTUMSCRIPT_EXPORT static void initMemory();

			XYO_QUANTUMSCRIPT_EXPORT virtual bool isFunction();
	};

};

#define XYO_QUANTUMSCRIPT_FUNCTIONWITHYIELD_BEGIN() \
	switch (function->yieldStep) {              \
	case 0:

#define XYO_QUANTUMSCRIPT_FUNCTIONWITHYIELD_YIELD(returnValue) \
	function->yieldStep = __LINE__;                        \
	return returnValue;                                    \
	break;                                                 \
case __LINE__:

#define XYO_QUANTUMSCRIPT_FUNCTIONWITHYIELD_END(returnValue) \
	}                                                    \
	;                                                    \
	function->yieldStep = 0;                             \
	return returnValue

#endif
