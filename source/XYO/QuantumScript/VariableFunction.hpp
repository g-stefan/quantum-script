// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLEFUNCTION_HPP
#define XYO_QUANTUMSCRIPT_VARIABLEFUNCTION_HPP

#ifndef XYO_QUANTUMSCRIPT_CONTEXT_HPP
#	include <XYO/QuantumScript/Context.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_PROTOTYPE_HPP
#	include <XYO/QuantumScript/Prototype.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_FUNCTIONPARENT_HPP
#	include <XYO/QuantumScript/FunctionParent.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_VARIABLEOBJECT_HPP
#	include <XYO/QuantumScript/VariableObject.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_VARIABLEARRAY_HPP
#	include <XYO/QuantumScript/VariableArray.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableFunction;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableFunction> : public TMemoryPoolActive<XYO::QuantumScript::VariableFunction> {};
};

namespace XYO::QuantumScript {

	class VariableFunction;
	typedef TPointer<Variable> (*FunctionProcedure)(VariableFunction *function, Variable *this_, VariableArray *arguments);

	class VariableFunction : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableFunction);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableFunction);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeFunction;

		public:
			TPointerX<Object> super;
			void *valueSuper;

			TPointerX<Property> object;
			TPointerX<Prototype> prototype;

			TPointerX<FunctionParent> functionParent;
			FunctionProcedure functionProcedure;

			XYO_QUANTUMSCRIPT_EXPORT VariableFunction();

			inline void activeConstructor() {
				valueSuper = nullptr;
				object.newMemory();
				prototype.newMemory();
				prototype->prototype = VariableObject::newVariable();
			};

			inline void activeDestructor() {
				functionParent.deleteMemory();
				object.deleteMemory();
				prototype.deleteMemory();
				super.deleteMemory();
			};

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable(FunctionParent *functionParent, VariableArray *parentVariables, VariableArray *parentArguments, FunctionProcedure functionProcedure, Object *super, void *valueSuper);

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
	};

};

#endif
