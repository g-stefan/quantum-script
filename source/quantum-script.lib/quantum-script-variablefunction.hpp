//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEFUNCTION_HPP
#define QUANTUM_SCRIPT_VARIABLEFUNCTION_HPP

#ifndef QUANTUM_SCRIPT_CONTEXT_HPP
#include "quantum-script-context.hpp"
#endif

#ifndef QUANTUM_SCRIPT_PROTOTYPE_HPP
#include "quantum-script-prototype.hpp"
#endif

#ifndef QUANTUM_SCRIPT_FUNCTIONPARENT_HPP
#include "quantum-script-functionparent.hpp"
#endif

#ifndef QUANTUM_SCRIPT_VARIABLEOBJECT_HPP
#include "quantum-script-variableobject.hpp"
#endif

#ifndef QUANTUM_SCRIPT_VARIABLEARRAY_HPP
#include "quantum-script-variablearray.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableFunction;

	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableFunction>:
			public TMemoryPoolActive<Quantum::Script::VariableFunction> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableFunction;
		typedef TPointer<Variable> (*FunctionProcedure)(VariableFunction *function, Variable *this_, VariableArray *arguments);

		class VariableFunction :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableFunction);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableFunction);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeFunction;
			public:

				TPointerX<Object> super;
				void *valueSuper;

				TPointerX<Property> object;
				TPointerX<Prototype> prototype;

				TPointerX<FunctionParent> functionParent;
				FunctionProcedure functionProcedure;

				QUANTUM_SCRIPT_EXPORT  VariableFunction();

				inline void activeConstructor() {					
					valueSuper = nullptr;
					object.newMemory();
					prototype.newMemory();
					prototype->prototype=VariableObject::newVariable();
				};

				inline void activeDestructor() {
					functionParent.deleteMemory();
					object.deleteMemory();
					prototype.deleteMemory();
					super.deleteMemory();
				};


				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(FunctionParent *functionParent, VariableArray *parentVariables, VariableArray *parentArguments, FunctionProcedure functionProcedure, Object *super, void *valueSuper);

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

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				QUANTUM_SCRIPT_EXPORT static void initMemory();
		};

	};
};



#endif


