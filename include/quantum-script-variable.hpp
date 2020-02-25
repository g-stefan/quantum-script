//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
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
			public Object {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(Variable);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeUndefined;
				QUANTUM_SCRIPT_EXPORT static const char *typeUndefinedKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeUndefined;
			public:
				const void *variableType;
				//
				// Constructor/Destructor
				//

				inline Variable() {
					variableType = registerType(typeUndefined, typeUndefinedKey);
				};

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
				QUANTUM_SCRIPT_EXPORT virtual TPointerX<Variable> &operatorIndex(uint32_t index);
				QUANTUM_SCRIPT_EXPORT virtual TPointerX<Variable> &operatorReferenceOwnProperty(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT virtual Variable &operatorReference(Symbol symbolId);

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
				QUANTUM_SCRIPT_EXPORT virtual bool findOwnProperty(Symbol symbolId, Variable *&out);

				QUANTUM_SCRIPT_EXPORT static Variable &operatorReferenceX(Symbol symbolId, Variable *prototype);
				QUANTUM_SCRIPT_EXPORT virtual bool operatorDeleteIndex(Variable *variable);
				QUANTUM_SCRIPT_EXPORT virtual bool operatorDeleteOwnProperty(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT virtual Variable &operatorIndex2(Variable *variable);
				QUANTUM_SCRIPT_EXPORT virtual TPointerX<Variable> &operatorReferenceIndex(Variable *variable);

				QUANTUM_SCRIPT_EXPORT virtual TPointer<Iterator> getIteratorKey();
				QUANTUM_SCRIPT_EXPORT virtual TPointer<Iterator> getIteratorValue();

				QUANTUM_SCRIPT_EXPORT virtual bool hasProperty(Variable *variable);

				//
				//
				//
				QUANTUM_SCRIPT_EXPORT virtual String getType();
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

				//
				QUANTUM_SCRIPT_EXPORT static const void *registerType(const void *&type, const char *key);
				QUANTUM_SCRIPT_EXPORT const char *getVariableType();

				//
				inline static bool isVariableUndefined(const Variable *value) {
					if(typeUndefined == nullptr) {
						typeUndefined = registerType(typeUndefined, typeUndefinedKey);
					};
					return (value->variableType == typeUndefined);
				};
				//
		};


	};
};

#endif
