//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "quantum-script-context.hpp"
#include "quantum-script-variablefunction.hpp"
#include "quantum-script-variablearray.hpp"
#include "quantum-script-variableassociativearray.hpp"
#include "quantum-script-variableboolean.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"
#include "quantum-script-variablenull.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		Symbol Context::getSymbol(String name) {
			return (TSingletonThread<SymbolList>::getValue())->getSymbol(name);
		};

		String Context::getSymbolMirror(Symbol id) {
			return (TSingletonThread<SymbolList>::getValue())->getSymbolMirror(id);
		};

		SymbolList &Context::getSymbolList() {
			return *(TSingletonThread<SymbolList>::getValue());
		};

		class XValueUndefined:
			public Object {
			public:
				TPointerX<Variable> valueUndefined;
				XValueUndefined();
				static void initMemory();
		};

		XValueUndefined::XValueUndefined() {
			valueUndefined.pointerLink(this);
		};

		void XValueUndefined::initMemory() {
			TMemory<Variable>::initMemory();
		};

		TPointerX<Variable> &Context::getValueUndefined() {
			return  (TSingletonThread<XValueUndefined>::getValue())->valueUndefined;
		};

		class XGlobalObject:
			public Object {
			public:
				TPointerX<Variable> globalObject;
				XGlobalObject();
				static void initMemory();
		};

		XGlobalObject::XGlobalObject() {
			globalObject.pointerLink(this);
		};

		void XGlobalObject::initMemory() {
			TMemory<Variable>::initMemory();
			TMemory<VariableObject>::initMemory();
		};

		TPointerX<Variable> &Context::getGlobalObject() {
			return  (TSingletonThread<XGlobalObject>::getValue())->globalObject;
		};

		class XContext:
			public Object {
			public:

				TPointerX<Prototype> prototypeBoolean;
				TPointerX<Prototype> prototypeNumber;
				TPointerX<Prototype> prototypeString;
				TPointerX<Prototype> prototypeArray;
				TPointerX<Prototype> prototypeFunction;
				TPointerX<Prototype> prototypeObject;
				TPointerX<Prototype> prototypeAssociativeArray;
				TPointerX<Prototype> prototypeResource;

				Symbol symbolLength;
				Symbol symbolPrototype;
				Symbol symbolFunctionBoolean;
				Symbol symbolFunctionNumber;
				Symbol symbolFunctionString;
				Symbol symbolFunctionArray;
				Symbol symbolFunctionFunction;
				Symbol symbolFunctionObject;
				Symbol symbolFunctionAssociativeArray;
				Symbol symbolFunctionResource;
				Symbol symbolSize;
				Symbol symbolToString;
				Symbol symbolToNumber;

				XContext();

				void newContext();
				void deleteContext();

				static void initMemory();
		};

		XContext::XContext() {
			prototypeBoolean.pointerLink(this);
			prototypeNumber.pointerLink(this);
			prototypeString.pointerLink(this);
			prototypeArray.pointerLink(this);
			prototypeFunction.pointerLink(this);
			prototypeObject.pointerLink(this);
			prototypeAssociativeArray.pointerLink(this);
			prototypeResource.pointerLink(this);
		};

		static TPointer<Variable> functionBoolean(VariableFunction *function, Variable *this_, VariableArray *arguments) {
			return VariableBoolean::newVariable((arguments->index(0))->toBoolean());
		};

		static TPointer<Variable> functionNumber(VariableFunction *function, Variable *this_, VariableArray *arguments) {
			return VariableNumber::newVariable((arguments->index(0))->toNumber());
		};

		static TPointer<Variable> functionString(VariableFunction *function, Variable *this_, VariableArray *arguments) {
			return VariableString::newVariable((arguments->index(0))->toString());
		};

		static TPointer<Variable> functionFunction(VariableFunction *function, Variable *this_, VariableArray *arguments) {
			return Context::getValueUndefined();
		};

		static TPointer<Variable> functionObject(VariableFunction *function, Variable *this_, VariableArray *arguments) {
			return this_;
		};

		static TPointer<Variable> functionArray(VariableFunction *function, Variable *this_, VariableArray *arguments) {
			return VariableArray::newVariable();
		};

		static TPointer<Variable> functionAssociativeArray(VariableFunction *function, Variable *this_, VariableArray *arguments) {
			return VariableAssociativeArray::newVariable();
		};

		void XContext::newContext() {
			VariableFunction *defaultPrototypeFunction;
			symbolLength = Context::getSymbol("length");
			symbolPrototype = Context::getSymbol("prototype");
			symbolFunctionBoolean = Context::getSymbol("Boolean");
			symbolFunctionNumber = Context::getSymbol("Number");
			symbolFunctionString = Context::getSymbol("String");
			symbolFunctionArray = Context::getSymbol("Array");
			symbolFunctionFunction = Context::getSymbol("Function");
			symbolFunctionObject = Context::getSymbol("Object");
			symbolFunctionAssociativeArray = Context::getSymbol("AssociativeArray");
			symbolFunctionResource = Context::getSymbol("Resource");
			symbolSize = Context::getSymbol("size");
			symbolToString = Context::getSymbol("toString");
			symbolToNumber = Context::getSymbol("toNumber");

			prototypeBoolean.newMemory();
			prototypeNumber.newMemory();
			prototypeString.newMemory();
			prototypeArray.newMemory();
			prototypeFunction.newMemory();
			prototypeObject.newMemory();
			prototypeAssociativeArray.newMemory();
			prototypeResource.newMemory();

			defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionBoolean, nullptr, nullptr);
			((Context::getGlobalObject())->operatorReferenceOwnProperty(symbolFunctionBoolean))=defaultPrototypeFunction;
			prototypeBoolean = defaultPrototypeFunction->prototype;

			defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionNumber, nullptr, nullptr);
			((Context::getGlobalObject())->operatorReferenceOwnProperty(symbolFunctionNumber))=defaultPrototypeFunction;
			prototypeNumber = defaultPrototypeFunction->prototype;

			defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionString, nullptr, nullptr);
			((Context::getGlobalObject())->operatorReferenceOwnProperty(symbolFunctionString))=defaultPrototypeFunction;
			prototypeString = defaultPrototypeFunction->prototype;

			defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionArray, nullptr, nullptr);
			((Context::getGlobalObject())->operatorReferenceOwnProperty(symbolFunctionArray))=defaultPrototypeFunction;
			prototypeArray = defaultPrototypeFunction->prototype;

			defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionFunction, nullptr, nullptr);
			((Context::getGlobalObject())->operatorReferenceOwnProperty(symbolFunctionFunction))=defaultPrototypeFunction;
			prototypeFunction = defaultPrototypeFunction->prototype;

			defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionObject, nullptr, nullptr);
			((Context::getGlobalObject())->operatorReferenceOwnProperty(symbolFunctionObject))=defaultPrototypeFunction;
			prototypeObject = defaultPrototypeFunction->prototype;

			defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionAssociativeArray, nullptr, nullptr);
			((Context::getGlobalObject())->operatorReferenceOwnProperty(symbolFunctionAssociativeArray))=defaultPrototypeFunction;
			prototypeAssociativeArray = defaultPrototypeFunction->prototype;

			defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionFunction, nullptr, nullptr);
			((Context::getGlobalObject())->operatorReferenceOwnProperty(symbolFunctionResource))=defaultPrototypeFunction;
			prototypeResource = defaultPrototypeFunction->prototype;
		};

		void XContext::deleteContext() {
			prototypeBoolean.deleteMemory();
			prototypeNumber.deleteMemory();
			prototypeString.deleteMemory();
			prototypeArray.deleteMemory();
			prototypeFunction.deleteMemory();
			prototypeObject.deleteMemory();
			prototypeAssociativeArray.deleteMemory();
			prototypeResource.deleteMemory();

			symbolLength = 0;
			symbolPrototype = 0;
			symbolFunctionBoolean = 0;
			symbolFunctionNumber = 0;
			symbolFunctionString = 0;
			symbolFunctionArray = 0;
			symbolFunctionFunction = 0;
			symbolFunctionObject = 0;
			symbolFunctionAssociativeArray = 0;
			symbolFunctionResource = 0;
			symbolSize = 0;
			symbolToString = 0;
			symbolToNumber = 0;
		};

		void XContext::initMemory() {
			TMemory<SymbolList>::initMemory();
			TMemory<Prototype>::initMemory();
			TMemory<Variable>::initMemory();
			TMemory<VariableObject>::initMemory();
			TMemory<VariableFunction>::initMemory();
		};

		TPointerX<Prototype> &Context::getPrototypeBoolean() {
			return (TSingletonThread<XContext>::getValue())->prototypeBoolean;
		};

		TPointerX<Prototype> &Context::getPrototypeNumber() {
			return (TSingletonThread<XContext>::getValue())->prototypeNumber;
		};

		TPointerX<Prototype> &Context::getPrototypeString() {
			return (TSingletonThread<XContext>::getValue())->prototypeString;
		};

		TPointerX<Prototype> &Context::getPrototypeArray() {
			return (TSingletonThread<XContext>::getValue())->prototypeArray;
		};

		TPointerX<Prototype> &Context::getPrototypeFunction() {
			return (TSingletonThread<XContext>::getValue())->prototypeFunction;
		};

		TPointerX<Prototype> &Context::getPrototypeObject() {
			return (TSingletonThread<XContext>::getValue())->prototypeObject;
		};

		TPointerX<Prototype> &Context::getPrototypeAssociativeArray() {
			return (TSingletonThread<XContext>::getValue())->prototypeAssociativeArray;
		};

		TPointerX<Prototype> &Context::getPrototypeResource() {
			return (TSingletonThread<XContext>::getValue())->prototypeResource;
		};

		Symbol Context::getSymbolLength() {
			return (TSingletonThread<XContext>::getValue())->symbolLength;
		};

		Symbol Context::getSymbolPrototype() {
			return (TSingletonThread<XContext>::getValue())->symbolPrototype;
		};

		Symbol Context::getSymbolFunctionBoolean() {
			return (TSingletonThread<XContext>::getValue())->symbolFunctionBoolean;
		};

		Symbol Context::getSymbolFunctionNumber() {
			return (TSingletonThread<XContext>::getValue())->symbolFunctionNumber;
		};

		Symbol Context::getSymbolFunctionString() {
			return (TSingletonThread<XContext>::getValue())->symbolFunctionString;
		};

		Symbol Context::getSymbolFunctionArray() {
			return (TSingletonThread<XContext>::getValue())->symbolFunctionArray;
		};

		Symbol Context::getSymbolFunctionFunction() {
			return (TSingletonThread<XContext>::getValue())->symbolFunctionFunction;
		};

		Symbol Context::getSymbolFunctionObject() {
			return (TSingletonThread<XContext>::getValue())->symbolFunctionObject;
		};

		Symbol Context::getSymbolFunctionAssociativeArray() {
			return (TSingletonThread<XContext>::getValue())->symbolFunctionAssociativeArray;
		};

		Symbol Context::getSymbolFunctionResource() {
			return (TSingletonThread<XContext>::getValue())->symbolFunctionResource;
		};

		Symbol Context::getSymbolSize() {
			return (TSingletonThread<XContext>::getValue())->symbolSize;
		};

		Symbol Context::getSymbolToNumber() {
			return (TSingletonThread<XContext>::getValue())->symbolToNumber;
		};

		Symbol Context::getSymbolToString() {
			return (TSingletonThread<XContext>::getValue())->symbolToString;
		};

		void Context::initMemory() {
			TSingletonThread<SymbolList>::initMemory();
			TSingletonThread<XValueUndefined>::initMemory();
			TSingletonThread<XContext>::initMemory();
			TSingletonThread<XGlobalObject>::initMemory();
		};

		void Context::newContext() {
			(TSingletonThread<SymbolList>::getValue())->symbolList.empty();
			(TSingletonThread<SymbolList>::getValue())->symbolListMirror.empty();
			(TSingletonThread<XValueUndefined>::getValue())->valueUndefined.newMemory();
			(TSingletonThread<XGlobalObject>::getValue())->globalObject = VariableObject::newVariable();
			(TSingletonThread<XContext>::getValue())->newContext();
		};

		void Context::deleteContext() {
			(TSingletonThread<XContext>::getValue())->deleteContext();
			(TSingletonThread<XGlobalObject>::getValue())->globalObject.deleteMemory();
			(TSingletonThread<XValueUndefined>::getValue())->valueUndefined.deleteMemory();
			(TSingletonThread<SymbolList>::getValue())->symbolList.empty();
			(TSingletonThread<SymbolList>::getValue())->symbolListMirror.empty();
		};

	};
};


