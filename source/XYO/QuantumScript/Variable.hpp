// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
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

	class OperandStack;
	typedef OperandStack Stack;
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

	// first node with key greater than key, used by iterators,
	// a node pointer can't be kept, the node can be removed while iterating
	inline PropertyNode *propertyNodeNext(Property *property, Symbol key) {
		PropertyNode *scan = property->root;
		PropertyNode *retV = nullptr;
		while (scan) {
			if (key < scan->key) {
				retV = scan;
				scan = scan->left;
				continue;
			};
			scan = scan->right;
		};
		return retV;
	};

	class SymbolList;
	class Context;
	class Prototype;
	typedef Variable VariableUndefined;
	class Iterator;

	class VariableArray;

#ifdef XYO_PLATFORM_OS_WINDOWS
#	define XYO_QUANTUMSCRIPT_FORMAT_INTEGER "%I64d"
#	define XYO_QUANTUMSCRIPT_FORMAT_NUMBER "%g"
#	define XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INTEGER "%.0lf"
#	define XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INPUT "%lf"
#	define XYO_QUANTUMSCRIPT_FORMAT_UINT32 "%u"
#endif

#ifdef XYO_PLATFORM_OS_LINUX
#	ifdef XYO_PLATFORM_64BIT
#		define XYO_QUANTUMSCRIPT_FORMAT_INTEGER "%lld"
#		define XYO_QUANTUMSCRIPT_FORMAT_NUMBER "%g"
#		define XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INTEGER "%.0lf"
#		define XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INPUT "%lf"
#		define XYO_QUANTUMSCRIPT_FORMAT_UINT32 "%u"
#	else
#		define XYO_QUANTUMSCRIPT_FORMAT_INTEGER "%ld"
#		define XYO_QUANTUMSCRIPT_FORMAT_NUMBER "%g"
#		define XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INTEGER "%.0lf"
#		define XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INPUT "%lf"
#		define XYO_QUANTUMSCRIPT_FORMAT_UINT32 "%u"
#	endif
#endif

	class Variable : public DynamicObject {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Variable);
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

			//
			XYO_QUANTUMSCRIPT_EXPORT virtual bool isFunction();
	};

	//
	// Operand stack of the virtual machine, contiguous memory,
	// push/pop without allocation
	//
	class OperandStack : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(OperandStack);

		protected:
			TPointer<Variable> *stack_;
			size_t size_;
			size_t capacity_;

			XYO_QUANTUMSCRIPT_EXPORT void grow();

		public:
			inline OperandStack() {
				stack_ = nullptr;
				size_ = 0;
				capacity_ = 0;
			};

			inline ~OperandStack() {
				empty();
				delete[] stack_;
			};

			inline void push(const Variable *value) {
				if (size_ == capacity_) {
					grow();
				};
				stack_[size_] = value;
				++size_;
			};

			inline void push() {
				if (size_ == capacity_) {
					grow();
				};
				++size_;
			};

			inline void pop(TPointer<Variable> &value) {
				--size_;
				value = std::move(stack_[size_]);
			};

			inline void pop(TPointerX<Variable> &value) {
				--size_;
				value = stack_[size_].value();
				stack_[size_].deleteMemory();
			};

			inline void popExecutive(TPointerX<Variable> &value) {
				--size_;
				value.setExecutive(std::move(stack_[size_]));
			};

			inline TTransfer<Variable> *popTransfer() {
				--size_;
				return stack_[size_].transfer();
			};

			inline void pop() {
				--size_;
				stack_[size_].deleteMemory();
			};

			inline void peek(TPointer<Variable> &value) {
				value = stack_[size_ - 1];
			};

			inline void peek(TPointerX<Variable> &value) {
				value = stack_[size_ - 1].value();
			};

			inline void peek(Variable *&value) {
				value = stack_[size_ - 1].value();
			};

			inline void duplicate() {
				if (size_ == capacity_) {
					grow();
				};
				stack_[size_] = stack_[size_ - 1];
				++size_;
			};

			inline void swap() {
				TPointer<Variable> top(std::move(stack_[size_ - 1]));
				stack_[size_ - 1] = std::move(stack_[size_ - 2]);
				stack_[size_ - 2] = std::move(top);
			};

			inline bool isEmpty() const {
				return (size_ == 0);
			};

			inline size_t getDepth() const {
				return size_;
			};

			// pop until depth
			inline void unwindTo(size_t depth) {
				while (size_ > depth) {
					pop();
				};
			};

			inline void empty() {
				unwindTo(0);
			};

			inline void activeDestructor() {
				empty();
			};
	};

};

#endif
