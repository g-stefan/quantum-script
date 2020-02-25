//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEVMFUNCTION_HPP
#define QUANTUM_SCRIPT_VARIABLEVMFUNCTION_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_VARIABLEFUNCTION_HPP
#include "quantum-script-variablefunction.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXECUTIVECONTEXT_HPP
#include "quantum-script-executivecontext.hpp"
#endif

#ifndef QUANTUM_SCRIPT_PARSERFUNCTIONHINT_HPP
#include "quantum-script-parserfunctionhint.hpp"
#endif

#ifndef QUANTUM_SCRIPT_INSTRUCTIONX_HPP
#include "quantum-script-instructionx.hpp"
#endif


namespace Quantum {
	namespace Script {

		class VariableVmFunction;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableVmFunction>:
			public TMemoryPoolActive<Quantum::Script::VariableVmFunction> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableVmFunction :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableVmFunction);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeVmFunction;
				QUANTUM_SCRIPT_EXPORT static const char *typeVmFunctionKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeVmFunction;
			public:
				TPointerX<Property> object;
				TPointerX<Prototype> prototype;

				TPointer<InstructionList> instructionList;

				Symbol fnSource;
				ProgramCounter *value;
				ProgramCounter *valueEnd;

				// coroutine
				ProgramCounter *originalValue;
				TPointerX<TStack<TPointerX<ExecutiveContextPc> > > coroutineContext;

				TPointerX<FunctionParent> functionParent;

				int functionHint;

				inline VariableVmFunction() {
					object.pointerLink(this);
					prototype.pointerLink(this);
					coroutineContext.pointerLink(this);
					functionParent.pointerLink(this);
					variableType = registerType(typeVmFunction, typeVmFunctionKey);
					functionHint = ParserFunctionHint::All;
					coroutineContext.newMemory();
					object.newMemory();
					fnSource = 0;
				};

				inline void activeConstructor() {
					object.newMemory();
				};

				inline void activeDestructor() {
					object.deleteMemory();
					prototype.deleteMemory();
					coroutineContext->empty();
					functionParent.deleteMemory();
					instructionList.deleteMemory();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(ProgramCounter *value);


				QUANTUM_SCRIPT_EXPORT String getType();

				QUANTUM_SCRIPT_EXPORT TPointer<Variable> functionApply(Variable *this_, VariableArray *arguments);

				QUANTUM_SCRIPT_EXPORT TPointerX<Variable> &operatorReferenceOwnProperty(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT Variable &operatorReference(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();
				QUANTUM_SCRIPT_EXPORT bool instanceOfPrototype(Prototype *&out);
				QUANTUM_SCRIPT_EXPORT bool findOwnProperty(Symbol symbolId, Variable *&out);
				QUANTUM_SCRIPT_EXPORT bool operatorDeleteIndex(Variable *variable);
				QUANTUM_SCRIPT_EXPORT bool operatorDeleteOwnProperty(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT Variable &operatorIndex2(Variable *variable);
				QUANTUM_SCRIPT_EXPORT TPointerX<Variable> &operatorReferenceIndex(Variable *variable);
				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorKey();
				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorValue();
				QUANTUM_SCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);
				QUANTUM_SCRIPT_EXPORT bool hasProperty(Variable *variable);

				inline static void initMemory() {
					TMemory<Prototype>::initMemory();
					TMemory<InstructionList>::initMemory();
					TMemory<ExecutiveContext>::initMemory();
					TMemory<FunctionParent>::initMemory();
					TPointerX<TStack<TPointerX<ExecutiveContextPc> > >::initMemory();
				};

				QUANTUM_SCRIPT_EXPORT String getSource();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				//
				inline static bool isVariableVmFunction(const Variable *value) {
					if(typeVmFunction == nullptr) {
						typeVmFunction = registerType(typeVmFunction, typeVmFunctionKey);
					};
					return (value->variableType == typeVmFunction);
				};
		};

	};
};


#endif
