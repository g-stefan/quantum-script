//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEFUNCTIONWITHYIELD_HPP
#define QUANTUM_SCRIPT_VARIABLEFUNCTIONWITHYIELD_HPP

#ifndef QUANTUM_SCRIPT_CONTEXT_HPP
#include "quantum-script-context.hpp"
#endif

#ifndef QUANTUM_SCRIPT_PROTOTYPE_HPP
#include "quantum-script-prototype.hpp"
#endif

#ifndef QUANTUM_SCRIPT_FUNCTIONPARENT_HPP
#include "quantum-script-functionparent.hpp"
#endif

#ifndef QUANTUM_SCRIPT_VARIABLEARRAY_HPP
#include "quantum-script-variablearray.hpp"
#endif

#ifndef QUANTUM_SCRIPT_VARIABLEOBJECT_HPP
#include "quantum-script-variableobject.hpp"
#endif

namespace Quantum {
	namespace Script {


		class VariableFunctionWithYield;

	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableFunctionWithYield>:
			public TMemoryPoolActive<Quantum::Script::VariableFunctionWithYield> {};
	};
};


namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableFunctionWithYield;
		typedef TPointer<Variable> (*FunctionProcedureWithYield)(VariableFunctionWithYield *function, Variable *this_, VariableArray *arguments);

		class VariableFunctionWithYield :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableFunctionWithYield);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeFunction;
				QUANTUM_SCRIPT_EXPORT static const char *typeFunctionWithYieldKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeFunctionWithYield;
			public:

				TPointerX<Object> super;
				void *valueSuper;

				TPointerX<Property> object;
				TPointerX<Prototype> prototype;

				TPointerX<FunctionParent> functionParent;
				FunctionProcedureWithYield functionProcedure;

				uint32_t yieldStep;
				TPointerX<Variable> yieldVariables;


				inline VariableFunctionWithYield() {
					variableType = registerType(typeFunctionWithYield, typeFunctionWithYieldKey);

					object.pointerLink(this);
					super.pointerLink(this);
					prototype.pointerLink(this);

					object.newMemory();

					prototype.newMemory();
					prototype->prototype=VariableObject::newVariable();

					functionParent.pointerLink(this);
					yieldVariables.pointerLink(this);
					functionProcedure = nullptr;
					yieldStep = 0;
					yieldVariables=VariableArray::newVariable();
					valueSuper = nullptr;

				};

				inline void activeConstructor() {
					valueSuper = nullptr;

					object.newMemory();

					prototype.newMemory();
					prototype->prototype=VariableObject::newVariable();
					yieldStep = 0;
					yieldVariables=VariableArray::newVariable();
				};

				inline void activeDestructor() {
					functionParent.deleteMemory();

					object.deleteMemory();

					prototype.deleteMemory();
					yieldVariables.deleteMemory();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(FunctionParent *functionParent, VariableArray *parentVariables, VariableArray *parentArguments, FunctionProcedureWithYield functionProcedure, Object *super, void *valueSuper);


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

				QUANTUM_SCRIPT_EXPORT static void initMemory();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				//
				inline static bool isVariableFunctionWithYield(const Variable *value) {
					if(typeFunctionWithYield == nullptr) {
						typeFunctionWithYield = registerType(typeFunctionWithYield, typeFunctionWithYieldKey);
					};
					return (value->variableType == typeFunctionWithYield);
				};

		};

	};
};


#define QUANTUM_SCRIPT_FUNCTIONWITHYIELD_BEGIN() \
	switch(function->yieldStep){ \
	case 0:


#define QUANTUM_SCRIPT_FUNCTIONWITHYIELD_YIELD(returnValue) \
	function->yieldStep=__LINE__;\
	return returnValue;\
	break;\
	case __LINE__:

#define QUANTUM_SCRIPT_FUNCTIONWITHYIELD_END(returnValue) \
	};\
	function->yieldStep=0;\
	return returnValue


#endif


