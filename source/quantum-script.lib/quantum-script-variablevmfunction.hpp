//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEVMFUNCTION_HPP
#define QUANTUM_SCRIPT_VARIABLEVMFUNCTION_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#	include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_VARIABLEFUNCTION_HPP
#	include "quantum-script-variablefunction.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXECUTIVECONTEXT_HPP
#	include "quantum-script-executivecontext.hpp"
#endif

#ifndef QUANTUM_SCRIPT_PARSERFUNCTIONHINT_HPP
#	include "quantum-script-parserfunctionhint.hpp"
#endif

#ifndef QUANTUM_SCRIPT_INSTRUCTIONX_HPP
#	include "quantum-script-instructionx.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableVmFunction;
	};
};

namespace XYO {
	namespace ManagedMemory {
		template <>
		class TMemory<Quantum::Script::VariableVmFunction> : public TMemoryPoolActive<Quantum::Script::VariableVmFunction> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableVmFunction : public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableVmFunction);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableVmFunction);

			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeVmFunction;

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

				QUANTUM_SCRIPT_EXPORT VariableVmFunction();

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

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(ProgramCounter *value);

				QUANTUM_SCRIPT_EXPORT String getVariableType();

				QUANTUM_SCRIPT_EXPORT TPointer<Variable> getPropertyBySymbol(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT TPointer<Variable> getPropertyByIndex(size_t index);
				QUANTUM_SCRIPT_EXPORT TPointer<Variable> getPropertyByVariable(Variable *index);
				QUANTUM_SCRIPT_EXPORT void setPropertyBySymbol(Symbol symbolId, Variable *value);
				QUANTUM_SCRIPT_EXPORT void setPropertyByIndex(size_t index, Variable *value);
				QUANTUM_SCRIPT_EXPORT void setPropertyByVariable(Variable *index, Variable *value);
				QUANTUM_SCRIPT_EXPORT bool deletePropertyBySymbol(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT bool deletePropertyByIndex(size_t index);
				QUANTUM_SCRIPT_EXPORT bool deletePropertyByVariable(Variable *index);

				QUANTUM_SCRIPT_EXPORT TPointer<Variable> functionApply(Variable *this_, VariableArray *arguments);

				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();
				QUANTUM_SCRIPT_EXPORT bool instanceOfPrototype(Prototype *&out);

				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorKey();
				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorValue();

				QUANTUM_SCRIPT_EXPORT String getSource();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				QUANTUM_SCRIPT_EXPORT static void initMemory();
		};

	};
};

#endif
