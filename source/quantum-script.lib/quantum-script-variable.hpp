//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#define QUANTUM_SCRIPT_VARIABLE_HPP

#ifndef QUANTUM_SCRIPT__DEPENDENCY_HPP
#include "quantum-script--dependency.hpp"
#endif

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class Variable;

		typedef TDynamicArray<TPointerX<Variable>, 4, TMemoryPoolActive> Array;

		typedef uint32_t Symbol;
		typedef TRedBlackTree<Symbol, TPointerX<Variable>, TMemoryPoolActive> Property;

		typedef TStackPointerUnsafe<Variable, TMemoryPoolActive> Stack;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Variable>:
			public TMemoryPoolActive<Quantum::Script::Variable> {};

		template<>
		class TMemory<Quantum::Script::Array>:
			public TMemoryPoolActive<Quantum::Script::Array> {};

		template<>
		class TMemory<Quantum::Script::Property>:
			public TMemoryPoolActive<Quantum::Script::Property> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class Variable;

		typedef bool Boolean;
		typedef int64_t Integer;
		typedef uint64_t UInteger;
		typedef double Number;

		typedef Property::Node PropertyNode;

		class SymbolList;
		class Context;
		class Prototype;
		typedef Variable VariableUndefined;
		class Iterator;

		class VariableArray;

#ifdef XYO_OS_WINDOWS
#define QUANTUM_SCRIPT_FORMAT_INTEGER "%I64d"
#define QUANTUM_SCRIPT_FORMAT_NUMBER "%g"
#define QUANTUM_SCRIPT_FORMAT_NUMBER_INTEGER "%.0lf"
#define QUANTUM_SCRIPT_FORMAT_NUMBER_INPUT "%lf"
#define QUANTUM_SCRIPT_FORMAT_DWORD XYO_FORMAT_UINT32T
#endif

#ifdef XYO_OS_UNIX
#ifdef XYO_APPLICATION_64BIT
#define QUANTUM_SCRIPT_FORMAT_INTEGER "%ld"
#define QUANTUM_SCRIPT_FORMAT_NUMBER "%g"
#define QUANTUM_SCRIPT_FORMAT_NUMBER_INTEGER "%.0lf"
#define QUANTUM_SCRIPT_FORMAT_NUMBER_INPUT "%lf"
#define QUANTUM_SCRIPT_FORMAT_DWORD XYO_FORMAT_UINT32T
#else
#define QUANTUM_SCRIPT_FORMAT_INTEGER "%lld"
#define QUANTUM_SCRIPT_FORMAT_NUMBER "%g"
#define QUANTUM_SCRIPT_FORMAT_NUMBER_INTEGER "%.0lf"
#define QUANTUM_SCRIPT_FORMAT_NUMBER_INPUT "%lf"
#define QUANTUM_SCRIPT_FORMAT_DWORD "%u"
#endif
#endif

		class Variable :
			public DynamicObject {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(Variable);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, Variable);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeUndefined;
			public:
				//
				// Constructor/Destructor
				//
				QUANTUM_SCRIPT_EXPORT Variable();

				inline static Variable *newVariable() {
					return TMemory<Variable>::newMemory();
				};

				//
				// Basic conversion
				//
				QUANTUM_SCRIPT_EXPORT virtual bool toBoolean();
				QUANTUM_SCRIPT_EXPORT virtual Number toNumber();
				QUANTUM_SCRIPT_EXPORT virtual String toString();
				QUANTUM_SCRIPT_EXPORT virtual bool isString();
				QUANTUM_SCRIPT_EXPORT size_t toIndex();

				//
				// Member access operators
				//
				QUANTUM_SCRIPT_EXPORT virtual TPointer<Variable> getPropertyBySymbol(Symbol symbolId); // x.y
				QUANTUM_SCRIPT_EXPORT virtual TPointer<Variable> getPropertyByIndex(size_t index); // x[y]
				QUANTUM_SCRIPT_EXPORT virtual TPointer<Variable> getPropertyByVariable(Variable *index); // x[y]
				QUANTUM_SCRIPT_EXPORT virtual void setPropertyBySymbol(Symbol symbolId, Variable *value); // x.y = ...
				QUANTUM_SCRIPT_EXPORT virtual void setPropertyByIndex(size_t index, Variable *value); // x[y] = ...
				QUANTUM_SCRIPT_EXPORT virtual void setPropertyByVariable(Variable *index, Variable *value); // x[y] = ...
				QUANTUM_SCRIPT_EXPORT virtual bool deletePropertyBySymbol(Symbol symbolId); // delete x.y
				QUANTUM_SCRIPT_EXPORT virtual bool deletePropertyByIndex(size_t index); // delete x[y]
				QUANTUM_SCRIPT_EXPORT virtual bool deletePropertyByVariable(Variable *index); // delete x[y]
				QUANTUM_SCRIPT_EXPORT virtual bool hasPropertyByVariable(Variable *variable); // y in x
				//
				QUANTUM_SCRIPT_EXPORT virtual TPointer<Variable> referenceSet(Variable *value);
				QUANTUM_SCRIPT_EXPORT virtual TPointer<Variable> referenceGet();
				QUANTUM_SCRIPT_EXPORT virtual Number referenceToNumber();
				QUANTUM_SCRIPT_EXPORT virtual String referenceToString();
				QUANTUM_SCRIPT_EXPORT virtual TPointer<Variable> referenceOperatorPlus(Variable *b);
				QUANTUM_SCRIPT_EXPORT virtual void referenceSetA1(Variable *value); // Accelerator

				//
				// Function call
				//
				QUANTUM_SCRIPT_EXPORT virtual TPointer<Variable> functionApply(Variable *this_, VariableArray *arguments);

				//
				// Object
				//
				QUANTUM_SCRIPT_EXPORT virtual Variable *instancePrototype();
				QUANTUM_SCRIPT_EXPORT bool instanceOf(Variable *value);
				QUANTUM_SCRIPT_EXPORT virtual bool instanceOfPrototype(Prototype *&out);

				QUANTUM_SCRIPT_EXPORT virtual TPointer<Iterator> getIteratorKey();
				QUANTUM_SCRIPT_EXPORT virtual TPointer<Iterator> getIteratorValue();

				//
				//
				//
				QUANTUM_SCRIPT_EXPORT virtual String getVariableType();
				QUANTUM_SCRIPT_EXPORT static void initMemory();

				//
				// Thread copy
				//
				QUANTUM_SCRIPT_EXPORT virtual Variable *clone(SymbolList &inSymbolList);

				//
				//
				//
				QUANTUM_SCRIPT_EXPORT TPointer<Variable> newObjectFunctionApply(VariableArray *arguments);
				QUANTUM_SCRIPT_EXPORT bool isLess(Variable *b);
				QUANTUM_SCRIPT_EXPORT bool isLessOrEqual(Variable *b);
				QUANTUM_SCRIPT_EXPORT bool isGreater(Variable *b);
				QUANTUM_SCRIPT_EXPORT bool isGreaterOrEqual(Variable *b);
				QUANTUM_SCRIPT_EXPORT bool isEqual(Variable *b);
				QUANTUM_SCRIPT_EXPORT bool isEqualStrict(Variable *b);

				QUANTUM_SCRIPT_EXPORT int compare(Variable *b);

				QUANTUM_SCRIPT_EXPORT TPointer<Variable> operatorPlus(Variable *b);
		};


	};
};

#endif
