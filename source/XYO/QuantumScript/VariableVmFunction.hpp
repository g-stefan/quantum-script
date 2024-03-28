// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLEVMFUNCTION_HPP
#define XYO_QUANTUMSCRIPT_VARIABLEVMFUNCTION_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_VARIABLEFUNCTION_HPP
#	include <XYO/QuantumScript/VariableFunction.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_EXECUTIVECONTEXT_HPP
#	include <XYO/QuantumScript/ExecutiveContext.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_PARSERFUNCTIONHINT_HPP
#	include <XYO/QuantumScript/ParserFunctionHint.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_INSTRUCTIONX_HPP
#	include <XYO/QuantumScript/InstructionX.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableVmFunction;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableVmFunction> : public TMemoryPoolActive<XYO::QuantumScript::VariableVmFunction> {};
};

namespace XYO::QuantumScript {

	class VariableVmFunction : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableVmFunction);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableVmFunction);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeVmFunction;

		public:
			TPointerX<Property> object;
			TPointerX<Prototype> prototype;

			TPointer<InstructionList> instructionList;

			Symbol fnSource;
			ProgramCounter *value;
			ProgramCounter *valueEnd;

			// coroutine
			ProgramCounter *originalValue;
			TPointerX<TStack<TPointerX<ExecutiveContextPc>>> coroutineContext;

			TPointerX<FunctionParent> functionParent;

			int functionHint;

			XYO_QUANTUMSCRIPT_EXPORT VariableVmFunction();

			inline void activeConstructor() {
				object.newMemory();
				prototype.newMemory();
				prototype->prototype = VariableObject::newVariable();
			};

			inline void activeDestructor() {
				object.deleteMemory();
				prototype.deleteMemory();
				coroutineContext->empty();
				functionParent.deleteMemory();
				instructionList.deleteMemory();
			};

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable(ProgramCounter *value);

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

			XYO_QUANTUMSCRIPT_EXPORT String getSource();

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT String toString();

			XYO_QUANTUMSCRIPT_EXPORT static void initMemory();

			XYO_QUANTUMSCRIPT_EXPORT virtual bool isFunction();
	};

};

#endif
