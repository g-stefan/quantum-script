// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#define XYO_QUANTUMSCRIPT_VARIABLE_HPP

#ifndef XYO_QUANTUMSCRIPT_DEPENDENCY_HPP
#	include <XYO/QuantumScript/Dependency.hpp>
#endif

namespace XYO::QuantumScript {

	class Variable;

	typedef TDynamicArray<TPointerX<Variable>, 4, TMemoryPoolActive> Array;

	typedef uint32_t Symbol;
	typedef TRedBlackTree<Symbol, TPointerX<Variable>, TMemoryPoolActive> Property;

	typedef TStackPointerUnsafe<Variable, TMemoryPoolActive> Stack;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Variable> : public TMemoryPoolActive<XYO::QuantumScript::Variable> {};

	template <>
	class TMemory<XYO::QuantumScript::Array> : public TMemoryPoolActive<XYO::QuantumScript::Array> {};

	template <>
	class TMemory<XYO::QuantumScript::Property> : public TMemoryPoolActive<XYO::QuantumScript::Property> {};
};

namespace XYO::QuantumScript {

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
#	define XYO_QUANTUMSCRIPT_FORMAT_INTEGER "%I64d"
#	define XYO_QUANTUMSCRIPT_FORMAT_NUMBER "%g"
#	define XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INTEGER "%.0lf"
#	define XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INPUT "%lf"
#	define XYO_QUANTUMSCRIPT_FORMAT_DWORD "%u"
#endif

#ifdef XYO_OS_LINUX
#	ifdef XYO_APPLICATION_64BIT
#		define XYO_QUANTUMSCRIPT_FORMAT_INTEGER "%ld"
#		define XYO_QUANTUMSCRIPT_FORMAT_NUMBER "%g"
#		define XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INTEGER "%.0lf"
#		define XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INPUT "%lf"
#		define XYO_QUANTUMSCRIPT_FORMAT_DWORD "%u"
#	else
#		define XYO_QUANTUMSCRIPT_FORMAT_INTEGER "%lld"
#		define XYO_QUANTUMSCRIPT_FORMAT_NUMBER "%g"
#		define XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INTEGER "%.0lf"
#		define XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INPUT "%lf"
#		define XYO_QUANTUMSCRIPT_FORMAT_DWORD "%lu"
#	endif
#endif

	class Variable : public DynamicObject {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(Variable);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, Variable);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeUndefined;

		public:
			//
			// Constructor/Destructor
			//
			XYO_QUANTUMSCRIPT_EXPORT Variable();

			inline static Variable *newVariable() {
				return TMemory<Variable>::newMemory();
			};

			//
			// Basic conversion
			//
			XYO_QUANTUMSCRIPT_EXPORT virtual bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT virtual Number toNumber();
			XYO_QUANTUMSCRIPT_EXPORT virtual String toString();
			XYO_QUANTUMSCRIPT_EXPORT virtual bool isString();
			XYO_QUANTUMSCRIPT_EXPORT size_t toIndex();

			//
			// Member access operators
			//
			XYO_QUANTUMSCRIPT_EXPORT virtual TPointer<Variable> getPropertyBySymbol(Symbol symbolId);      // x.y
			XYO_QUANTUMSCRIPT_EXPORT virtual TPointer<Variable> getPropertyByIndex(size_t index);          // x[y]
			XYO_QUANTUMSCRIPT_EXPORT virtual TPointer<Variable> getPropertyByVariable(Variable *index);    // x[y]
			XYO_QUANTUMSCRIPT_EXPORT virtual void setPropertyBySymbol(Symbol symbolId, Variable *value);   // x.y = ...
			XYO_QUANTUMSCRIPT_EXPORT virtual void setPropertyByIndex(size_t index, Variable *value);       // x[y] = ...
			XYO_QUANTUMSCRIPT_EXPORT virtual void setPropertyByVariable(Variable *index, Variable *value); // x[y] = ...
			XYO_QUANTUMSCRIPT_EXPORT virtual bool deletePropertyBySymbol(Symbol symbolId);                 // delete x.y
			XYO_QUANTUMSCRIPT_EXPORT virtual bool deletePropertyByIndex(size_t index);                     // delete x[y]
			XYO_QUANTUMSCRIPT_EXPORT virtual bool deletePropertyByVariable(Variable *index);               // delete x[y]
			XYO_QUANTUMSCRIPT_EXPORT virtual bool hasPropertyByVariable(Variable *variable);               // y in x
			//
			XYO_QUANTUMSCRIPT_EXPORT virtual TPointer<Variable> referenceSet(Variable *value);
			XYO_QUANTUMSCRIPT_EXPORT virtual TPointer<Variable> referenceGet();
			XYO_QUANTUMSCRIPT_EXPORT virtual Number referenceToNumber();
			XYO_QUANTUMSCRIPT_EXPORT virtual String referenceToString();
			XYO_QUANTUMSCRIPT_EXPORT virtual TPointer<Variable> referenceOperatorPlus(Variable *b);
			XYO_QUANTUMSCRIPT_EXPORT virtual void referenceSetA1(Variable *value); // Accelerator

			//
			// Function call
			//
			XYO_QUANTUMSCRIPT_EXPORT virtual TPointer<Variable> functionApply(Variable *this_, VariableArray *arguments);

			//
			// Object
			//
			XYO_QUANTUMSCRIPT_EXPORT virtual Variable *instancePrototype();
			XYO_QUANTUMSCRIPT_EXPORT bool instanceOf(Variable *value);
			XYO_QUANTUMSCRIPT_EXPORT virtual bool instanceOfPrototype(Prototype *&out);

			XYO_QUANTUMSCRIPT_EXPORT virtual TPointer<Iterator> getIteratorKey();
			XYO_QUANTUMSCRIPT_EXPORT virtual TPointer<Iterator> getIteratorValue();

			//
			//
			//
			XYO_QUANTUMSCRIPT_EXPORT virtual String getVariableType();
			XYO_QUANTUMSCRIPT_EXPORT static void initMemory();

			//
			// Thread copy
			//
			XYO_QUANTUMSCRIPT_EXPORT virtual Variable *clone(SymbolList &inSymbolList);

			//
			//
			//
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> newObjectFunctionApply(VariableArray *arguments);
			XYO_QUANTUMSCRIPT_EXPORT bool isLess(Variable *b);
			XYO_QUANTUMSCRIPT_EXPORT bool isLessOrEqual(Variable *b);
			XYO_QUANTUMSCRIPT_EXPORT bool isGreater(Variable *b);
			XYO_QUANTUMSCRIPT_EXPORT bool isGreaterOrEqual(Variable *b);
			XYO_QUANTUMSCRIPT_EXPORT bool isEqual(Variable *b);
			XYO_QUANTUMSCRIPT_EXPORT bool isEqualStrict(Variable *b);

			XYO_QUANTUMSCRIPT_EXPORT int compare(Variable *b);

			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> operatorPlus(Variable *b);
	};

};

#endif
