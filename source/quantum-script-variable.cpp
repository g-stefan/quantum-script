//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-variable.hpp"
#include "quantum-script-variableobject.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variablenull.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"
#include "quantum-script-variablearray.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *Variable::typeUndefinedKey = "{23C345C1-0835-4938-863D-C2B39B68A2CE}";
		const void *Variable::typeUndefined;
		const char *Variable::strTypeUndefined = "undefined";

		bool Variable::toBoolean() {
			return false;
		};

		Number Variable::toNumber() {
			return NAN;
		};

		String Variable::toString() {
			return strTypeUndefined;
		};

		size_t Variable::toIndex() {
			Number nr = toNumber();
			if(isnan(nr)) {
				return 0;
			};
			if(isinf(nr)) {
				return 0;
			};
			if(nr < 0) {
				return 0;
			};
			return (size_t)(nr);
		};

		bool Variable::isString() {
			return false;
		};

		String Variable::getType() {
			return strTypeUndefined;
		};

		TPointerX<Variable> &Variable::operatorIndex(uint32_t index) {
			throw Error("operatorIndex");
		};

		TPointerX<Variable> &Variable::operatorReferenceOwnProperty(Symbol symbolId) {
			throw Error("operatorReferenceOwnProperty");
		};

		Variable &Variable::operatorReference(Symbol symbolId) {
			throw Error("operatorReference");
		};

		TPointer<Variable> Variable::functionApply(Variable *this_, VariableArray *arguments) {
			throw Error("functionApply");
		};

		Variable *Variable::instancePrototype() {
			return nullptr;
		};

		bool Variable::instanceOf(Variable *value) {
			Variable *thisPrototype = instancePrototype();
			Prototype *prototype_;
			if(value->instanceOfPrototype(prototype_)) {
				while(thisPrototype) {
					if(thisPrototype == prototype_->prototype) {
						return true;
					};
					thisPrototype = thisPrototype->instancePrototype();
				};
			};
			return false;
		};

		bool Variable::instanceOfPrototype(Prototype *&out) {
			return false;
		};

		bool Variable::findOwnProperty(Symbol symbolId, Variable *&out) {
			return false;
		};

		Variable &Variable::operatorReferenceX(Symbol symbolId, Variable *prototype) {
			Variable *outX;
			Variable *scan = prototype;
			while(scan) {
				if(scan->findOwnProperty(symbolId, outX)) {
					return *outX;
				};
				scan = scan->instancePrototype();
			};
			return *(Context::getValueUndefined());
		};

		bool Variable::operatorDeleteIndex(Variable *variable) {
			return false;
		};

		bool Variable::operatorDeleteOwnProperty(Symbol symbolId) {
			return false;
		};

		Variable &Variable::operatorIndex2(Variable *variable) {
			throw Error("operatorIndex");
		};

		TPointerX<Variable> &Variable::operatorReferenceIndex(Variable *variable) {
			throw Error("operatorReferenceIndex");
		};

		TPointer<Iterator> Variable::getIteratorKey() {
			throw Error("key not iterable");
		};

		TPointer<Iterator> Variable::getIteratorValue() {
			throw Error("value not iterable");
		};

		bool Variable::hasProperty(Variable *variable) {
			return false;
		};

		void Variable::initMemory() {
			TMemory<String>::initMemory();
		};

		Variable *Variable::clone(SymbolList &inSymbolList) {
			return newVariable();
		};


		TPointer<Variable> Variable::newObjectFunctionApply(VariableArray *arguments) {
			TPointer<Variable> this_(VariableObject::newVariable());
			Prototype *prototype;
			if(instanceOfPrototype(prototype)) {
				((VariableObject *)this_.value())->prototype=prototype;
				return functionApply(this_, arguments);
			};
			return VariableUndefined::newVariable();
		};


		bool Variable::isLess(Variable *b) {
			if(isString() && b->isString()) {
				return (toString() < b->toString());
			};

			Number na = toNumber();

			if(isnan(na)) {
				return false;
			};

			Number nb = b->toNumber();

			if(isnan(nb)) {
				return false;
			};

			return (na < nb);
		};

		bool Variable::isLessOrEqual(Variable *b) {
			if(isString() && b->isString()) {
				return (toString() <= b->toString());
			};

			Number na = toNumber();

			if(isnan(na)) {
				return false;
			};

			Number nb = b->toNumber();

			if(isnan(nb)) {
				return false;
			};

			return (na <= nb);
		};

		bool Variable::isGreater(Variable *b) {
			if(isString() && b->isString()) {
				return (toString() > b->toString());
			};

			Number na = toNumber();

			if(isnan(na)) {
				return false;
			};

			Number nb = b->toNumber();

			if(isnan(nb)) {
				return false;
			};

			return (na > nb);
		};

		bool Variable::isGreaterOrEqual(Variable *b) {
			if(isString() && b->isString()) {
				return (toString() >= b->toString());
			};

			Number na = toNumber();

			if(isnan(na)) {
				return false;
			};

			Number nb = b->toNumber();

			if(isnan(nb)) {
				return false;
			};

			return (na >= nb);
		};

		bool Variable::isEqual(Variable *b) {
			if(variableType == b->variableType) {
				if(VariableUndefined::isVariableUndefined(this)) {
					return true;
				};
				if(VariableNull::isVariableNull(this)) {
					return true;
				};
				if(VariableNumber::isVariableNumber(this)) {
					if(isnan((static_cast<VariableNumber *>(this))->value)) {
						return false;
					};
					if(isnan((static_cast<VariableNumber *>(b))->value)) {
						return false;
					};
					if((static_cast<VariableNumber *>(this))->value == (static_cast<VariableNumber *>(b))->value) {
						return true;
					};
					return false;
				};
				if(isString() && b->isString()) {
					return (toString() == b->toString());
				};
				if(this == b) {
					return true;
				};
				return false;
			};
			if(VariableNull::isVariableNull(this)) {
				if(VariableUndefined::isVariableUndefined(b)) {
					return true;
				};
			};
			if(VariableUndefined::isVariableUndefined(this)) {
				if(VariableNull::isVariableNull(b)) {
					return true;
				};
			};

			if(isString() && b->isString()) {
				return (toString() == b->toString());
			};

			Number na = toNumber();

			if(isnan(na)) {
				return false;
			};

			Number nb = b->toNumber();

			if(isnan(nb)) {
				return false;
			};

			return (na == nb);
		};

		bool Variable::isEqualStrict(Variable *b) {
			if(variableType != b->variableType) {
				if(isString() && b->isString()) {
					return (toString() == b->toString());
				};
				return false;
			};
			if(VariableUndefined::isVariableUndefined(this)) {
				return true;
			};
			if(VariableNull::isVariableNull(this)) {
				return true;
			};
			if(VariableNumber::isVariableNumber(this)) {
				if(isnan((static_cast<VariableNumber *>(this))->value)) {
					return false;
				};
				if(isnan((static_cast<VariableNumber *>(b))->value)) {
					return false;
				};
				if((static_cast<VariableNumber *>(this))->value == (static_cast<VariableNumber *>(b))->value) {
					return true;
				};
				return false;
			};
			if(isString() && b->isString()) {
				return (toString() == b->toString());
			};
			if(this == b) {
				return true;
			};
			return false;
		};

		int Variable::compare(Variable *b) {
			if(isString() && b->isString()) {
				return (toString()).compare(b->toString());
			};
			Number na = toNumber();

			if(isnan(na)) {
				return -1;
			};

			Number nb = b->toNumber();

			if(isnan(nb)) {
				return -1;
			};

			return (na - nb);
		};

		TPointer<Variable> Variable::operatorPlus(Variable *b) {
			if(isString() || b->isString()) {
				return VariableString::newVariable(toString() << b->toString());
			};
			return VariableNumber::newVariable(toNumber() + b->toNumber());
		};


		class TypeRegistryNode:
			public TXRedBlackTreeNode<TypeRegistryNode, const char *> {
			public:
		};

		class TypeRegistry {
			public:
				typedef TXRedBlackTree<TypeRegistryNode, TMemorySystem> RBTree;

				TypeRegistryNode *root;

#ifdef XYO_MULTI_THREAD
				CriticalSection criticalSection;
#endif

				TypeRegistry();
				~TypeRegistry();
		};

		TypeRegistry::TypeRegistry() {
			root = nullptr;
		};

		TypeRegistry::~TypeRegistry() {
			RBTree::empty(root);
		};

		const void *Variable::registerType(const void *&type, const char *key) {

			if(type != nullptr) {
				return type;
			};

			TypeRegistry *typeRegistry = TSingletonProcess<TypeRegistry>::getValue();
			TypeRegistry::RBTree::Node *this_;

#ifdef XYO_MULTI_THREAD
			typeRegistry->criticalSection.enter();
#endif
			this_ = TypeRegistry::RBTree::find(typeRegistry->root, key);
			if(this_ == nullptr) {
				this_ = TypeRegistry::RBTree::newNode();
				this_->key = key;
				TypeRegistry::RBTree::insertNode(typeRegistry->root, this_);
			};

#ifdef XYO_MULTI_THREAD
			typeRegistry->criticalSection.leave();
#endif

			type = this_;
			return type;
		};

		const char *Variable::getVariableType() {
			if(variableType == nullptr) {
				return strTypeUndefined;
			};
			return (reinterpret_cast<const TypeRegistry::RBTree::Node *>(variableType))->key;
		};

	};
};


