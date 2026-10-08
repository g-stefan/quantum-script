// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include <XYO/QuantumScript.Application/CompileCpp.hpp>
#include <XYO/QuantumScript/AsmOpcode.hpp>
#include <XYO/QuantumScript/ParserAsm.hpp>
#include <XYO/QuantumScript/VariableSymbol.hpp>
#include <XYO/QuantumScript/VariableArgumentLevel.hpp>
#include <XYO/QuantumScript/VariableOperator21.hpp>
#include <XYO/QuantumScript/VariableOperator22.hpp>
#include <XYO/QuantumScript/VariableOperator31.hpp>

// Must match XYO_QUANTUMSCRIPT_NATIVERUNTIME_ABI of the native runtime
#define XYO_QUANTUMSCRIPT_COMPILECPP_ABI 1

namespace XYO::QuantumScript::Application {

	static String toStringSize(size_t value) {
		char buf[32];
		snprintf(buf, sizeof(buf), XYO_PLATFORM_FORMAT_SIZET, value);
		return buf;
	};

	static String toStringInt(int value) {
		char buf[32];
		snprintf(buf, sizeof(buf), "%d", value);
		return buf;
	};

	// C++ literal of a number
	static String toStringNumber(Number value) {
		char buf[64];
		if (isnan(value)) {
			return "NAN";
		};
		if (isinf(value)) {
			return (value < 0) ? "(-INFINITY)" : "INFINITY";
		};
		if (value == 0 && signbit(value)) {
			return "(-0.0)";
		};
		snprintf(buf, sizeof(buf), "%.17g", value);
		String retV(buf);
		size_t index;
		if (!(retV.indexOf(".", 0, index) || retV.indexOf("e", 0, index))) {
			retV << ".0";
		};
		if (value < 0) {
			return String("(") << retV << ")";
		};
		return retV;
	};

	static String toStringC(const String &value) {
		String retV("\"");
		size_t k;
		char buf[8];
		for (k = 0; k < value.length(); ++k) {
			uint8_t ch = (uint8_t)value[k];
			if (ch == '\\' || ch == '"') {
				retV << "\\" << (char)ch;
				continue;
			};
			if (ch < 32 || ch > 126 || ch == '?') {
				snprintf(buf, sizeof(buf), "\\%03o", ch);
				retV << buf;
				continue;
			};
			retV << (char)ch;
		};
		retV << "\"";
		return retV;
	};

	static std::string toStdString(const String &value) {
		return std::string(value.value(), value.length());
	};

	//
	// Constants of the module: symbols, strings, numbers, sources
	//

	class CompileCppConstant {
		public:
			std::vector<String> symbol;
			std::vector<String> string;
			std::vector<Number> number;
			std::vector<String> source;

			size_t symbolIndex(const String &name) {
				return find(symbol, symbolMap, name);
			};

			size_t stringIndex(const String &value) {
				return find(string, stringMap, value);
			};

			size_t sourceIndex(const String &value) {
				return find(source, sourceMap, value);
			};

			size_t numberIndex(Number value) {
				uint64_t key;
				memcpy(&key, &value, sizeof(key));
				std::map<uint64_t, size_t>::iterator it = numberMap.find(key);
				if (it != numberMap.end()) {
					return it->second;
				};
				number.push_back(value);
				numberMap[key] = number.size() - 1;
				return number.size() - 1;
			};

		protected:
			std::map<std::string, size_t> symbolMap;
			std::map<std::string, size_t> stringMap;
			std::map<std::string, size_t> sourceMap;
			std::map<uint64_t, size_t> numberMap;

			static size_t find(std::vector<String> &list, std::map<std::string, size_t> &map, const String &value) {
				std::string key = toStdString(value);
				std::map<std::string, size_t>::iterator it = map.find(key);
				if (it != map.end()) {
					return it->second;
				};
				list.push_back(value);
				map[key] = list.size() - 1;
				return list.size() - 1;
			};
	};

	//
	// A region of a unit: the top level code (0) or the body of a function
	//

	class CompileCppRegion {
		public:
			size_t parent;
			AsmLabel begin;
			std::vector<AsmLabel> code;
			bool isCoroutine;
			bool localsCaptured; // inner functions use the local variables
			bool heapLocals;     // local variables in a VariableArray, else C++ locals
			uint32_t localCount;
			size_t contextCount; // contexts of the function, from the analysis
			String name;

			inline CompileCppRegion() {
				parent = 0;
				begin = 0;
				isCoroutine = false;
				localsCaptured = false;
				heapLocals = false;
				localCount = 0;
				contextCount = 1;
			};
	};

	//
	// Static operand stack and contexts
	//

	class CompileCppEntry {
		public:
			enum {
				Dynamic,           // a value, in a slot at the begin of a block
				ReferenceGlobal,   // global.symbol, index of the symbol in the constants
				ReferenceLocal,    // local variable
				ReferenceArgument, // argument
			};

			int kind;
			uint32_t index;

			inline CompileCppEntry() {
				kind = Dynamic;
				index = 0;
			};

			inline CompileCppEntry(int kind_, uint32_t index_) {
				kind = kind_;
				index = index_;
			};

			inline bool operator==(const CompileCppEntry &value) const {
				return (kind == value.kind) && (index == value.index);
			};
	};

	class CompileCppContext {
		public:
			AsmLabel break_;
			AsmLabel continue_;
			AsmLabel catch_;
			AsmLabel finally_;    // the finally ends with return to pc_
			AsmLabel tryFinally_; // the finally ends with finally-end
			AsmLabel tryBreak_;
			AsmLabel tryContinue_;
			AsmLabel tryReturn_;
			AsmLabel tryThrow_;
			int stackLink;      // operand stack depth of the try, -1 not set
			AsmLabel finallyOf; // a finally context: the finally label, pc_ / finally-next is set
			int referenceLocal; // for-in on a C++ local variable, -1 else

			inline CompileCppContext() {
				break_ = 0;
				continue_ = 0;
				catch_ = 0;
				finally_ = 0;
				tryFinally_ = 0;
				tryBreak_ = 0;
				tryContinue_ = 0;
				tryReturn_ = 0;
				tryThrow_ = 0;
				stackLink = -1;
				finallyOf = 0;
				referenceLocal = -1;
			};

			inline bool operator==(const CompileCppContext &value) const {
				return (break_ == value.break_) &&
				       (continue_ == value.continue_) &&
				       (catch_ == value.catch_) &&
				       (finally_ == value.finally_) &&
				       (tryFinally_ == value.tryFinally_) &&
				       (tryBreak_ == value.tryBreak_) &&
				       (tryContinue_ == value.tryContinue_) &&
				       (tryReturn_ == value.tryReturn_) &&
				       (tryThrow_ == value.tryThrow_) &&
				       (stackLink == value.stackLink) &&
				       (finallyOf == value.finallyOf) &&
				       (referenceLocal == value.referenceLocal);
			};
	};

	class CompileCppState {
		public:
			bool isSet;
			std::vector<CompileCppEntry> stack;
			std::vector<CompileCppContext> context; // context 0 is the function

			inline CompileCppState() {
				isSet = false;
			};
	};

	class CompileCppEdge {
		public:
			AsmLabel target;
			CompileCppState state;
			bool isFallThrough;
	};

	// break, continue, return and throw, resolved on the contexts
	class CompileCppWalk {
		public:
			enum {
				Error,         // no target, a runtime error
				Jump,          // to target, the contexts from contextIndex are left
				Catch,         // to the catch of the context contextIndex, the value is pushed
				Finally,       // to the finally of the context contextIndex, pc_ = pc, the value is pending
				FunctionReturn // out of the function
			};

			int kind;
			AsmLabel target;
			size_t contextIndex;
			AsmLabel pc;
			bool withValue;

			inline CompileCppWalk() {
				kind = Error;
				target = 0;
				contextIndex = 0;
				pc = 0;
				withValue = false;
			};
	};

	// the finally of a try context, 0 if none
	static AsmLabel finallyOfTry(const CompileCppContext &context) {
		if (context.tryFinally_) {
			return context.tryFinally_;
		};
		return context.finally_;
	};

	static CompileCppWalk walkBreak(const CompileCppState &state, bool isContinue) {
		CompileCppWalk retV;
		size_t k;
		for (k = state.context.size(); k > 1; --k) {
			const CompileCppContext &context = state.context[k - 1];
			AsmLabel tryTarget = isContinue ? context.tryContinue_ : context.tryBreak_;
			AsmLabel target = isContinue ? context.continue_ : context.break_;
			// the finally, then the break / continue again
			if (finallyOfTry(context) && tryTarget) {
				retV.kind = CompileCppWalk::Finally;
				retV.target = finallyOfTry(context);
				retV.contextIndex = k - 1;
				retV.pc = tryTarget;
				return retV;
			};
			if (target) {
				retV.kind = CompileCppWalk::Jump;
				retV.target = target;
				retV.contextIndex = k - 1;
				return retV;
			};
		};
		return retV;
	};

	// return value: a finally on the way runs first, the value is pending
	static CompileCppWalk walkReturn(const CompileCppState &state) {
		CompileCppWalk retV;
		size_t k;
		for (k = state.context.size(); k > 1; --k) {
			const CompileCppContext &context = state.context[k - 1];
			if (finallyOfTry(context) && context.tryReturn_) {
				retV.kind = CompileCppWalk::Finally;
				retV.target = finallyOfTry(context);
				retV.contextIndex = k - 1;
				retV.pc = context.tryReturn_;
				retV.withValue = true;
				return retV;
			};
		};
		retV.kind = CompileCppWalk::FunctionReturn;
		return retV;
	};

	static CompileCppWalk walkThrow(const CompileCppState &state) {
		CompileCppWalk retV;
		size_t k;
		for (k = state.context.size(); k > 1; --k) {
			const CompileCppContext &context = state.context[k - 1];
			if (context.catch_) {
				retV.kind = CompileCppWalk::Catch;
				retV.target = context.catch_;
				retV.contextIndex = k - 1;
				retV.withValue = true;
				return retV;
			};
			if (finallyOfTry(context) && context.tryThrow_) {
				retV.kind = CompileCppWalk::Finally;
				retV.target = finallyOfTry(context);
				retV.contextIndex = k - 1;
				retV.pc = context.tryThrow_;
				retV.withValue = true;
				return retV;
			};
		};
		retV.kind = CompileCppWalk::FunctionReturn;
		return retV;
	};

	// the state at the target of a walk, false if the stack of the try is not known
	static bool walkState(const CompileCppState &state, const CompileCppWalk &walk, CompileCppState &out) {
		out = state;
		switch (walk.kind) {
		case CompileCppWalk::Jump:
			out.context.resize(walk.contextIndex);
			return true;
		case CompileCppWalk::Catch: {
			int stackLink = state.context[walk.contextIndex].stackLink;
			if (stackLink < 0 || (size_t)stackLink > state.stack.size()) {
				return false;
			};
			out.context.resize(walk.contextIndex);
			out.stack.resize(stackLink);
			out.stack.push_back(CompileCppEntry());
			return true;
		};
		case CompileCppWalk::Finally: {
			int stackLink = state.context[walk.contextIndex].stackLink;
			if (stackLink < 0 || (size_t)stackLink > state.stack.size()) {
				return false;
			};
			out.context.resize(walk.contextIndex);
			CompileCppContext finallyContext;
			finallyContext.finallyOf = walk.target;
			// finally-end: break / continue in the finally leave the try
			if (state.context[walk.contextIndex].tryFinally_) {
				finallyContext.break_ = state.context[walk.contextIndex].tryBreak_;
				finallyContext.continue_ = state.context[walk.contextIndex].tryContinue_;
			};
			out.context.push_back(finallyContext);
			out.stack.resize(stackLink);
			return true;
		};
		default:
			break;
		};
		return false;
	};

	// the instruction can throw (C++ exception): the handler of the try is a successor
	static bool mayThrow(int opcode) {
		switch (opcode) {
		case ParserAsm::Nop:
		case ParserAsm::Mark:
		case ParserAsm::Goto:
		case ParserAsm::EnterContext:
		case ParserAsm::LeaveContext:
		case ParserAsm::ContextSetBreak:
		case ParserAsm::ContextSetContinue:
		case ParserAsm::ContextSetCatch:
		case ParserAsm::ContextSetFinally:
		case ParserAsm::ContextSetTryBreak:
		case ParserAsm::ContextSetTryContinue:
		case ParserAsm::ContextSetTryReturn:
		case ParserAsm::ContextSetTryThrow:
		case ParserAsm::ContextSetPC:
		case ParserAsm::ContextSetTryFinally:
		case ParserAsm::ContextSetFinallyNext:
		case ParserAsm::FinallyEnd:
		case ParserAsm::ContextSetStack:
		case ParserAsm::ContextSetRegisterValue:
		case ParserAsm::ContextPushRegisterValue:
		case ParserAsm::Pop1:
		case ParserAsm::Duplicate:
		case ParserAsm::PushUndefined:
		case ParserAsm::PushNumber:
		case ParserAsm::PushString:
		case ParserAsm::PushBoolean:
		case ParserAsm::PushGlobal:
		case ParserAsm::ContextPushThis:
		case ParserAsm::ContextPushSelf:
		case ParserAsm::PushArguments:
		case ParserAsm::PushObjectReference:
		case ParserAsm::LocalVariablesPushObjectReference:
		case ParserAsm::ArgumentsPushObjectReference:
		case ParserAsm::Break:
		case ParserAsm::Continue:
		case ParserAsm::Return:
		case ParserAsm::Yield:
			return false;
		default:
			break;
		};
		return true;
	};

	// can not continue with the next instruction
	static bool isTerminal(int opcode) {
		switch (opcode) {
		case ParserAsm::Goto:
		case ParserAsm::Return:
		case ParserAsm::Throw:
		case ParserAsm::Break:
		case ParserAsm::Continue:
		case ParserAsm::Yield:
		case ParserAsm::FinallyEnd:
		case ParserAsm::XTailCall:
		case ParserAsm::XTailCallSymbol:
		case ParserAsm::XTailCallThis:
		case ParserAsm::XTailCallThisModeCall:
		case ParserAsm::XTailCallThisModeApply:
		case ParserAsm::XTailCallWithThisReference:
		case ParserAsm::CurrentContextExit:
		case ParserAsm::EndExecution:
			return true;
		default:
			break;
		};
		return false;
	};

	static uint32_t operandIndex(CodeInstruction &in) {
		return (uint32_t)((VariableSymbol *)in.operand.value())->value;
	};

	static Symbol operandSymbol(CodeInstruction &in) {
		return ((VariableSymbol *)in.operand.value())->value;
	};

	//
	// Value of the operand stack while generating a block
	//

	class CompileCppValue {
		public:
			enum {
				TypeSlot,      // in the slot of its position
				TypeVariable,  // TPointer<Variable> temp
				TypeBorrowed,  // Variable *, owned by the module or a slot
				TypeBoolean,   // bool
				TypeNumber,    // Number
				TypeReference, // reference to a property, created only if used as a value
				TypeLocal,     // the value of a C++ local variable
				TypeStatic     // static reference: global symbol, local variable, argument
			};

			enum {
				ReferenceIndex,
				ReferenceSymbol,
				ReferenceVariable
			};

			int type;
			String name;
			bool hasNumber; // TypeBorrowed: a number constant
			Number number;
			bool hasBoolean; // TypeBorrowed: a boolean constant
			bool boolean;
			int referenceType;
			String referenceObject; // Variable * expression
			String referenceKey;    // index / symbol / Variable * expression
			CompileCppEntry entry;  // TypeStatic
			uint32_t index;         // TypeSlot: position, TypeLocal: local variable

			inline CompileCppValue() {
				type = TypeVariable;
				hasNumber = false;
				number = 0;
				hasBoolean = false;
				boolean = false;
				referenceType = ReferenceIndex;
				index = 0;
			};

			inline bool isNumber() const {
				return (type == TypeNumber) || hasNumber;
			};
	};

	class CompileCppUnit;

	//
	// One region as a C++ function: analysis, then code
	//

	class CompileCppFunction {
		public:
			CompileCppUnit &owner;
			CodeUnit &unit;
			CompileCppConstant &constant;
			std::vector<CompileCppRegion> &regions;
			std::vector<size_t> &regionOf;
			size_t regionIndex;
			CompileCppRegion &region;
			bool withInline;
			String error;
			bool needHeapLocals;

			// analysis, by label
			std::vector<CompileCppState> state;
			std::vector<bool> blockStart;
			std::vector<bool> finallyEnd; // Return at the end of a finally: continue at pc_
			std::vector<bool> rethrow;    // Throw after a finally, of the exception of the try
			std::map<AsmLabel, std::vector<std::pair<AsmLabel, bool>>> finallyTarget; // pc_ of a finally, with value
			std::vector<AsmLabel> handler;      // handler id - 1 to the label of the try instruction state key
			std::vector<CompileCppWalk> handlerWalk;
			std::vector<CompileCppState> handlerState;
			std::vector<AsmLabel> resume; // instructions after a yield
			size_t contextCount;

			// code
			String out;
			int indent;
			std::vector<CompileCppValue> stack;
			size_t tempCount;
			bool blockOpen;
			bool hasSource;
			uint32_t currentLine;
			size_t currentSource;
			int currentTryState;
			bool useGlobal;
			bool useSymbol;
			bool useLocals;
			bool useExceptionTrace;
			bool useSelfTailCall;
			std::vector<bool> useSlot;
			std::vector<bool> useLocal;
			std::vector<bool> useRegister;
			std::vector<bool> useReference;
			std::vector<bool> useIterator;
			std::vector<bool> usePc;
			std::vector<bool> usePending;

			CompileCppFunction(CompileCppUnit &owner_, size_t regionIndex_, bool withInline_);

			// --- analysis

			bool fail(AsmLabel label, const char *message) {
				error = message;
				error << " at instruction " << toStringSize(label);
				return false;
			};

			AsmLabel next(AsmLabel label) {
				if ((size_t)label + 1 > unit.length()) {
					return 0;
				};
				if (regionOf[label + 1] != regionIndex) {
					return 0;
				};
				return label + 1;
			};

			// the handler of the try of a state, 0 none, -1 not found (lookupOnly)
			int handlerId(const CompileCppState &value, bool lookupOnly) {
				CompileCppWalk walk = walkThrow(value);
				if (walk.kind == CompileCppWalk::FunctionReturn) {
					return 0;
				};
				size_t k;
				for (k = 0; k < handlerWalk.size(); ++k) {
					if (handlerWalk[k].kind == walk.kind &&
					    handlerWalk[k].target == walk.target &&
					    handlerWalk[k].contextIndex == walk.contextIndex &&
					    handlerWalk[k].pc == walk.pc &&
					    handlerState[k].context == std::vector<CompileCppContext>(value.context.begin(), value.context.begin() + walk.contextIndex + 1)) {
						return (int)(k + 1);
					};
				};
				if (lookupOnly) {
					return -1;
				};
				CompileCppState target;
				if (!walkState(value, walk, target)) {
					return -1;
				};
				handlerWalk.push_back(walk);
				handlerState.push_back(target);
				// the contexts that identify the handler
				handlerState.back().context = std::vector<CompileCppContext>(value.context.begin(), value.context.begin() + walk.contextIndex + 1);
				return (int)handlerWalk.size();
			};

			bool step(AsmLabel label, const CompileCppState &in, std::vector<CompileCppEdge> &edges);
			bool merge(AsmLabel label, const CompileCppState &value, std::vector<AsmLabel> &work);
			bool prepare();
			bool analyze();

			// --- code

			void line(const String &value) {
				out << "\t\t\t\t\t";
				int k;
				for (k = 0; k < indent; ++k) {
					out << "\t";
				};
				out << value << "\n";
			};

			String newTemp(const char *type) {
				String retV(type);
				retV << toStringSize(tempCount);
				++tempCount;
				return retV;
			};

			String symbol(Symbol id) {
				useSymbol = true;
				return String("S[") << toStringSize(constant.symbolIndex(Context::getSymbolMirror(id))) << "]";
			};

			String global() {
				useGlobal = true;
				return "global";
			};

			String slot(size_t index) {
				if (useSlot.size() <= index) {
					useSlot.resize(index + 1, false);
				};
				useSlot[index] = true;
				return String("s") << toStringSize(index);
			};

			String local(uint32_t index) {
				if (useLocal.size() <= index) {
					useLocal.resize(index + 1, false);
				};
				useLocal[index] = true;
				return String("l") << toStringSize(index);
			};

			String contextVariable(std::vector<bool> &use, const char *name, size_t depth) {
				if (use.size() <= depth) {
					use.resize(depth + 1, false);
				};
				use[depth] = true;
				return String(name) << toStringSize(depth);
			};

			String localsArray() {
				useLocals = true;
				return "locals";
			};

			void pushVariable(const String &expression) {
				CompileCppValue value;
				value.type = CompileCppValue::TypeVariable;
				value.name = newTemp("t");
				line(String("TPointer<Variable> ") << value.name << "(" << expression << ");");
				stack.push_back(value);
			};

			void pushBoolean(const String &expression) {
				CompileCppValue value;
				value.type = CompileCppValue::TypeBoolean;
				value.name = newTemp("b");
				line(String("bool ") << value.name << " = " << expression << ";");
				stack.push_back(value);
			};

			void pushNumber(const String &expression) {
				CompileCppValue value;
				value.type = CompileCppValue::TypeNumber;
				value.name = newTemp("n");
				line(String("Number ") << value.name << " = " << expression << ";");
				stack.push_back(value);
			};

			CompileCppValue &pushBorrowed(const String &expression) {
				CompileCppValue value;
				value.type = CompileCppValue::TypeBorrowed;
				value.name = expression;
				stack.push_back(value);
				return stack.back();
			};

			void pushStatic(int kind, uint32_t index) {
				CompileCppValue value;
				value.type = CompileCppValue::TypeStatic;
				value.entry = CompileCppEntry(kind, index);
				stack.push_back(value);
			};

			void pushReference(int referenceType, const String &object, const String &key) {
				CompileCppValue value;
				value.type = CompileCppValue::TypeReference;
				value.referenceType = referenceType;
				value.referenceObject = object;
				value.referenceKey = key;
				stack.push_back(value);
			};

			CompileCppValue pop() {
				CompileCppValue retV = stack.back();
				stack.pop_back();
				return retV;
			};

			// before a C++ local variable is set: its values on the stack are copied
			void spillLocal(uint32_t index) {
				for (CompileCppValue &value : stack) {
					if (value.type == CompileCppValue::TypeLocal && value.index == index) {
						String name = newTemp("t");
						line(String("TPointer<Variable> ") << name << "(" << local(index) << ");");
						value.type = CompileCppValue::TypeVariable;
						value.name = name;
					};
				};
			};

			bool isCppLocals() {
				return !region.heapLocals;
			};

			// Variable * expression of a static reference as a Variable
			String newStaticReference(const CompileCppEntry &entry) {
				switch (entry.kind) {
				case CompileCppEntry::ReferenceGlobal:
					return String("VariableReferenceSymbol::newVariable(") << global() << ", S[" << toStringSize(entry.index) << "])";
				case CompileCppEntry::ReferenceArgument:
					return String("VariableReferenceIndex::newVariable(arguments, ") << toStringSize(entry.index) << ")";
				case CompileCppEntry::ReferenceLocal:
					if (isCppLocals()) {
						needHeapLocals = true;
						return "nullptr";
					};
					return String("VariableReferenceIndex::newVariable(") << localsArray() << ", " << toStringSize(entry.index) << ")";
				default:
					break;
				};
				return "nullptr";
			};

			// Variable * expression, the value becomes a Variable
			String asVariable(CompileCppValue &value) {
				switch (value.type) {
				case CompileCppValue::TypeSlot:
					return slot(value.index) + ".value()";
				case CompileCppValue::TypeVariable:
					return value.name + ".value()";
				case CompileCppValue::TypeBorrowed:
					return value.name;
				case CompileCppValue::TypeLocal:
					return local(value.index) + ".value()";
				case CompileCppValue::TypeBoolean:
					return String("boolean(module, ") << value.name << ")";
				case CompileCppValue::TypeNumber:
				case CompileCppValue::TypeReference:
				case CompileCppValue::TypeStatic: {
					String name = newTemp("t");
					line(String("TPointer<Variable> ") << name << "(" << newVariable(value) << ");");
					value.type = CompileCppValue::TypeVariable;
					value.name = name;
					return name + ".value()";
				};
				default:
					break;
				};
				return "nullptr";
			};

			// Variable * expression, the value is not changed (code in a conditional block)
			String asVariableLocal(CompileCppValue &value) {
				CompileCppValue copy = value;
				return asVariable(copy);
			};

			// new Variable * expression of a value that is not a Variable
			String newVariable(CompileCppValue &value) {
				switch (value.type) {
				case CompileCppValue::TypeBoolean:
					return String("boolean(module, ") << value.name << ")";
				case CompileCppValue::TypeNumber:
					return String("VariableNumber::newVariable(") << value.name << ")";
				case CompileCppValue::TypeReference:
					switch (value.referenceType) {
					case CompileCppValue::ReferenceIndex:
						return String("VariableReferenceIndex::newVariable(") << value.referenceObject << ", " << value.referenceKey << ")";
					case CompileCppValue::ReferenceSymbol:
						return String("VariableReferenceSymbol::newVariable(") << value.referenceObject << ", " << value.referenceKey << ")";
					default:
						return String("VariableReferenceVariable::newVariable(") << value.referenceObject << ", " << value.referenceKey << ")";
					};
				case CompileCppValue::TypeStatic:
					return newStaticReference(value.entry);
				default:
					break;
				};
				return asVariable(value);
			};

			// Number expression, as Variable::toNumber
			String asNumber(CompileCppValue &value) {
				switch (value.type) {
				case CompileCppValue::TypeNumber:
					return value.name;
				case CompileCppValue::TypeBoolean:
					return String("(") << value.name << " ? 1.0 : 0.0)";
				case CompileCppValue::TypeBorrowed:
					if (value.hasNumber) {
						return toStringNumber(value.number);
					};
					break;
				default:
					break;
				};
				return String("(") << asVariable(value) << ")->toNumber()";
			};

			// bool expression, as Variable::toBoolean
			String asBoolean(CompileCppValue &value) {
				switch (value.type) {
				case CompileCppValue::TypeBoolean:
					return value.name;
				case CompileCppValue::TypeNumber:
					return String("(!isnan(") << value.name << ") && (" << value.name << " != 0))";
				case CompileCppValue::TypeBorrowed:
					if (value.hasBoolean) {
						return value.boolean ? "true" : "false";
					};
					if (value.hasNumber) {
						return (isnan(value.number) || (value.number == 0)) ? "false" : "true";
					};
					break;
				default:
					break;
				};
				return String("(") << asVariable(value) << ")->toBoolean()";
			};

			// number fast path: condition (empty if always) and Number expression
			bool asNumberPath(CompileCppValue &value, String &condition, String &number) {
				if (value.isNumber()) {
					condition = "";
					number = asNumber(value);
					return true;
				};
				String variable;
				switch (value.type) {
				case CompileCppValue::TypeSlot:
				case CompileCppValue::TypeVariable:
				case CompileCppValue::TypeBorrowed:
				case CompileCppValue::TypeLocal:
					variable = asVariable(value);
					condition = String("isNumber(") << variable << ")";
					number = String("numberValue(") << variable << ")";
					return true;
				default:
					break;
				};
				return false;
			};

			// --- references

			String referenceGet(CompileCppValue &value) {
				if (value.type == CompileCppValue::TypeStatic) {
					switch (value.entry.kind) {
					case CompileCppEntry::ReferenceGlobal:
						return String(global()) << "->getPropertyBySymbol(S[" << toStringSize(value.entry.index) << "])";
					case CompileCppEntry::ReferenceArgument:
						return String("arguments->index(") << toStringSize(value.entry.index) << ").value()";
					case CompileCppEntry::ReferenceLocal:
						if (isCppLocals()) {
							return local(value.entry.index) + ".value()";
						};
						return String(localsArray()) << "->index(" << toStringSize(value.entry.index) << ").value()";
					default:
						break;
					};
				};
				if (value.type == CompileCppValue::TypeReference) {
					switch (value.referenceType) {
					case CompileCppValue::ReferenceIndex:
						return String("(") << value.referenceObject << ")->getPropertyByIndex(" << value.referenceKey << ")";
					case CompileCppValue::ReferenceSymbol:
						return String("(") << value.referenceObject << ")->getPropertyBySymbol(" << value.referenceKey << ")";
					default:
						return String("(") << value.referenceObject << ")->getPropertyByVariable(" << value.referenceKey << ")";
					};
				};
				return String("(") << asVariable(value) << ")->referenceGet()";
			};

			String referenceToNumber(CompileCppValue &value) {
				if (value.type == CompileCppValue::TypeStatic || value.type == CompileCppValue::TypeReference) {
					return String("(") << referenceGet(value) << ")->toNumber()";
				};
				return String("(") << asVariable(value) << ")->referenceToNumber()";
			};

			// statement, variable is a Variable * expression
			void referenceSet(CompileCppValue &value, const String &variable) {
				if (value.type == CompileCppValue::TypeStatic) {
					switch (value.entry.kind) {
					case CompileCppEntry::ReferenceGlobal:
						line(String(global()) << "->setPropertyBySymbol(S[" << toStringSize(value.entry.index) << "], " << variable << ");");
						return;
					case CompileCppEntry::ReferenceArgument:
						line(String("arguments->index(") << toStringSize(value.entry.index) << ") = " << variable << ";");
						return;
					case CompileCppEntry::ReferenceLocal:
						if (isCppLocals()) {
							spillLocal(value.entry.index);
							line(local(value.entry.index) << " = " << variable << ";");
							return;
						};
						line(String(localsArray()) << "->index(" << toStringSize(value.entry.index) << ") = " << variable << ";");
						return;
					default:
						break;
					};
				};
				if (value.type == CompileCppValue::TypeReference) {
					switch (value.referenceType) {
					case CompileCppValue::ReferenceIndex:
						line(String("(") << value.referenceObject << ")->setPropertyByIndex(" << value.referenceKey << ", " << variable << ");");
						return;
					case CompileCppValue::ReferenceSymbol:
						line(String("(") << value.referenceObject << ")->setPropertyBySymbol(" << value.referenceKey << ", " << variable << ");");
						return;
					default:
						line(String("(") << value.referenceObject << ")->setPropertyByVariable(" << value.referenceKey << ", " << variable << ");");
						return;
					};
				};
				line(String("(") << asVariable(value) << ")->referenceSetA1(" << variable << ");");
			};

			// a, b - comparison with the number fast path
			String emitCompare(CompileCppValue &a, CompileCppValue &b, const char *operatorCpp, const char *method) {
				String result = newTemp("b");
				// numbers: NaN compares false, as Variable::is*
				if (a.isNumber() && b.isNumber()) {
					line(String("bool ") << result << " = ((" << asNumber(a) << ") " << operatorCpp << " (" << asNumber(b) << "));");
					return result;
				};
				String conditionA;
				String conditionB;
				String numberA;
				String numberB;
				if (asNumberPath(a, conditionA, numberA) && asNumberPath(b, conditionB, numberB)) {
					String condition = conditionA;
					if (conditionB.length()) {
						if (condition.length()) {
							condition << " && ";
						};
						condition << conditionB;
					};
					line(String("bool ") << result << ";");
					line(String("if (") << condition << ") {");
					line(String("\t") << result << " = ((" << numberA << ") " << operatorCpp << " (" << numberB << "));");
					line("} else {");
					String va = asVariableLocal(a);
					String vb = asVariableLocal(b);
					line(String("\t") << result << " = (" << va << ")->" << method << "(" << vb << ");");
					line("};");
					return result;
				};
				String va = asVariable(a);
				String vb = asVariable(b);
				line(String("bool ") << result << " = (" << va << ")->" << method << "(" << vb << ");");
				return result;
			};

			// --- state

			void setLine(CodeInstruction &in) {
				// the source of x-push-function is the source of the function
				if (!hasSource || in.sourceSymbol == 0 || in.asmType == ParserAsm::XPushFunction) {
					return;
				};
				size_t source = constant.sourceIndex(Context::getSymbolMirror(in.sourceSymbol));
				if (source != currentSource) {
					line(String("source = ") << toStringSize(source) << ";");
					currentSource = source;
				};
				if (in.sourceLineNumber != currentLine) {
					line(String("line = ") << toStringSize(in.sourceLineNumber) << ";");
					currentLine = in.sourceLineNumber;
				};
			};

			void setTryState(const CompileCppState &value) {
				if (handlerWalk.empty()) {
					return;
				};
				int tryState = handlerId(value, true);
				if (tryState < 0) {
					return;
				};
				if (tryState != currentTryState) {
					line(String("tryState = ") << toStringInt(tryState) << ";");
					currentTryState = tryState;
				};
			};

			// the stack as the state of target: values in slots, static references
			bool transfer(std::vector<CompileCppValue> values, const CompileCppState &target) {
				if (values.size() != target.stack.size()) {
					error = "Invalid operand stack";
					return false;
				};
				size_t k;
				for (k = 0; k < values.size(); ++k) {
					CompileCppValue &value = values[k];
					const CompileCppEntry &entry = target.stack[k];
					if (entry.kind == CompileCppEntry::Dynamic) {
						if (value.type == CompileCppValue::TypeSlot && value.index == k) {
							continue;
						};
						String expression = asVariable(value);
						line(slot(k) << " = " << expression << ";");
						continue;
					};
					if (value.type == CompileCppValue::TypeStatic && value.entry == entry) {
						continue;
					};
					if (entry.kind == CompileCppEntry::ReferenceLocal) {
						needHeapLocals = true;
					};
					error = "Invalid operand stack";
					return false;
				};
				return true;
			};

			bool jump(AsmLabel target) {
				if (!transfer(stack, state[target])) {
					return false;
				};
				line(String("goto L") << toStringSize(target) << ";");
				return true;
			};

			// the code of a walk, the walk value is a Variable * expression (or empty)
			bool emitWalk(const CompileCppState &from, const CompileCppWalk &walk, const String &value);

			// stack entries of a state, at the begin of a block
			void setStack(const CompileCppState &value) {
				stack.clear();
				size_t k;
				for (k = 0; k < value.stack.size(); ++k) {
					CompileCppValue item;
					if (value.stack[k].kind == CompileCppEntry::Dynamic) {
						item.type = CompileCppValue::TypeSlot;
						item.index = (uint32_t)k;
					} else {
						item.type = CompileCppValue::TypeStatic;
						item.entry = value.stack[k];
					};
					stack.push_back(item);
				};
			};

			void openBlock(AsmLabel label) {
				out << "\t\t\t\tL" << toStringSize(label) << ":;\n";
				out << "\t\t\t\t{\n";
				blockOpen = true;
				currentLine = 0xFFFFFFFF;
				currentSource = (size_t)-1;
				currentTryState = -1;
			};

			void closeBlock() {
				if (blockOpen) {
					out << "\t\t\t\t};\n";
					blockOpen = false;
				};
			};

			String returnValue(const String &value) {
				if (region.isCoroutine) {
					return String("{\n\t\t\t\t\t\tcoroutineReset(function);\n\t\t\t\t\t\treturn ") << value << ";\n\t\t\t\t\t};";
				};
				return String("return ") << value << ";";
			};

			bool emitInstruction(AsmLabel label, bool &fallThrough);
			bool emitInline(CodeInstruction &in);
			bool emitRuntime(CodeInstruction &in);
			bool emitCall(CodeInstruction &in);
			void emitSelfTailCall(CodeInstruction &in);
			bool generate(String &output);
	};

	//
	// One unit: regions and functions
	//

	class CompileCppUnit {
		public:
			CodeUnit &unit;
			CompileCppConstant &constant;
			String prefix;
			bool withInline;
			String error;

			std::vector<size_t> regionOf; // by label, 0 for the top level code
			std::vector<CompileCppRegion> regions;
			std::map<AsmLabel, size_t> regionByBegin;

			inline CompileCppUnit(CodeUnit &unit_, CompileCppConstant &constant_, const String &prefix_, bool withInline_) : unit(unit_), constant(constant_), prefix(prefix_), withInline(withInline_){};

			bool fail(AsmLabel label, const char *message) {
				error = message;
				error << " at instruction " << toStringSize(label);
				return false;
			};

			bool analyze() {
				size_t length = unit.length();
				size_t k;

				regionOf.assign(length + 2, 0);
				regions.clear();
				regionByBegin.clear();

				// function bodies, outer before inner
				std::vector<std::pair<AsmLabel, AsmLabel>> function;
				for (k = 1; k <= length; ++k) {
					CodeInstruction &in = unit.at((AsmLabel)k);
					const AsmOpcodeInfo *info = AsmOpcode::getInfo(in.asmType);
					if (!info || !info->procedure) {
						return fail((AsmLabel)k, "Unsupported instruction");
					};
					switch (info->operand) {
					case AsmOperand::Function:
						if (in.target == 0 || in.targetRange < in.target || in.targetRange > length || in.target <= k) {
							return fail((AsmLabel)k, "Invalid function");
						};
						function.push_back(std::pair<AsmLabel, AsmLabel>(in.target, in.targetRange));
						break;
					case AsmOperand::ProgramCounter:
					case AsmOperand::IndexNumberGoto:
					case AsmOperand::SymbolNumberGoto:
						if (in.target > length) {
							return fail((AsmLabel)k, "Invalid jump");
						};
						// context-set-catch without catch, ... - 0 is not set
						if (in.target == 0) {
							switch (in.asmType) {
							case ParserAsm::ContextSetBreak:
							case ParserAsm::ContextSetContinue:
							case ParserAsm::ContextSetCatch:
							case ParserAsm::ContextSetFinally:
							case ParserAsm::ContextSetTryFinally:
							case ParserAsm::ContextSetTryBreak:
							case ParserAsm::ContextSetTryContinue:
							case ParserAsm::ContextSetTryReturn:
							case ParserAsm::ContextSetTryThrow:
								break;
							default:
								return fail((AsmLabel)k, "Invalid jump");
							};
						};
						break;
					case AsmOperand::ProgramCounterRange:
					case AsmOperand::Native:
						return fail((AsmLabel)k, "Unsupported instruction");
					default:
						break;
					};
				};
				std::sort(function.begin(), function.end());

				regions.resize(function.size() + 1);
				regions[0].begin = length ? 1 : 0;
				for (k = 0; k < function.size(); ++k) {
					AsmLabel scan;
					for (scan = function[k].first; scan <= function[k].second; ++scan) {
						regionOf[scan] = k + 1;
					};
					regions[k + 1].begin = function[k].first;
					regionByBegin[function[k].first] = k + 1;
				};
				for (k = 1; k <= length; ++k) {
					regions[regionOf[k]].code.push_back((AsmLabel)k);
				};

				// parent of each function, the region of its x-push-function
				for (k = 1; k <= length; ++k) {
					CodeInstruction &in = unit.at((AsmLabel)k);
					if (AsmOpcode::getInfo(in.asmType)->operand == AsmOperand::Function) {
						regions[regionByBegin[in.target]].parent = regionOf[k];
					};
				};

				for (k = 0; k < regions.size(); ++k) {
					regions[k].name = prefix;
					regions[k].name << "f" << toStringSize(k);
					regions[k].heapLocals = !withInline;
				};

				// local variables: count, captured by inner functions; coroutines
				for (k = 1; k <= length; ++k) {
					CodeInstruction &in = unit.at((AsmLabel)k);
					CompileCppRegion &region = regions[regionOf[k]];
					uint32_t index1 = 0;
					uint32_t index2 = 0;
					bool isLocal = false;
					switch (in.asmType) {
					case ParserAsm::LocalVariablesPushSymbol:
					case ParserAsm::LocalVariablesPushObjectReference:
					case ParserAsm::LocalVariablesPlusPlus:
					case ParserAsm::OperatorPow2LocalVariablesSymbol:
						index1 = index2 = operandIndex(in);
						isLocal = true;
						break;
					case ParserAsm::OperatorMulLocalVariablesSymbol2:
					case ParserAsm::OperatorMinusLocalVariablesSymbol2:
					case ParserAsm::OperatorPlusLocalVariablesSymbol2:
					case ParserAsm::AssignLocalVariablesObjectReferencePow2LocalVariablesSymbol:
						index1 = (uint32_t)((VariableOperator22 *)in.operand.value())->symbol1;
						index2 = (uint32_t)((VariableOperator22 *)in.operand.value())->symbol2;
						isLocal = true;
						break;
					case ParserAsm::LocalVariablesLevelPushSymbol:
					case ParserAsm::LocalVariablesLevelPushObjectReference: {
						int level = ((VariableArgumentLevel *)in.operand.value())->level;
						size_t scan = regionOf[k];
						for (; level > 0; --level) {
							if (scan == 0) {
								break;
							};
							scan = regions[scan].parent;
						};
						regions[scan].localsCaptured = true;
						regions[scan].heapLocals = true;
						break;
					};
					case ParserAsm::Yield:
						if (regionOf[k] != 0) {
							region.isCoroutine = true;
						};
						break;
					default:
						break;
					};
					if (isLocal) {
						if (index1 + 1 > region.localCount) {
							region.localCount = index1 + 1;
						};
						if (index2 + 1 > region.localCount) {
							region.localCount = index2 + 1;
						};
					};
				};
				return true;
			};

			bool generate(String &output) {
				size_t k;
				if (!analyze()) {
					return false;
				};

				// the analysis of all functions: contexts of coroutines
				std::vector<String> code(regions.size());
				for (k = 0; k < regions.size(); ++k) {
					CompileCppFunction function(*this, k, withInline);
					if (!function.analyze()) {
						error = function.error;
						return false;
					};
					regions[k].contextCount = function.contextCount;
				};
				for (k = 0; k < regions.size(); ++k) {
					bool isOk = false;
					for (int retry = 0; retry < 2; ++retry) {
						CompileCppFunction function(*this, k, withInline);
						if (!function.analyze()) {
							error = function.error;
							return false;
						};
						code[k] = "";
						if (function.generate(code[k])) {
							isOk = true;
							break;
						};
						if (function.needHeapLocals && !regions[k].heapLocals) {
							regions[k].heapLocals = true;
							continue;
						};
						error = function.error;
						return false;
					};
					if (!isOk) {
						error = "Unable to generate the function";
						return false;
					};
				};

				for (k = 0; k < regions.size(); ++k) {
					output << code[k];
				};
				return true;
			};

			void declare(String &output) {
				size_t k;
				for (k = 0; k < regions.size(); ++k) {
					if (regions[k].isCoroutine) {
						output << "\tstatic TPointer<Variable> " << regions[k].name << "(VariableFunctionWithYield *function, Variable *this_, VariableArray *arguments);\n";
						continue;
					};
					output << "\tstatic TPointer<Variable> " << regions[k].name << "(VariableFunction *function, Variable *this_, VariableArray *arguments);\n";
				};
			};
	};

	CompileCppFunction::CompileCppFunction(CompileCppUnit &owner_, size_t regionIndex_, bool withInline_) : owner(owner_), unit(owner_.unit), constant(owner_.constant), regions(owner_.regions), regionOf(owner_.regionOf), regionIndex(regionIndex_), region(owner_.regions[regionIndex_]), withInline(withInline_) {
		needHeapLocals = false;
		contextCount = 1;
		tempCount = 0;
		indent = 0;
		blockOpen = false;
		hasSource = false;
		currentLine = 0;
		currentSource = 0;
		currentTryState = -1;
		useGlobal = false;
		useSymbol = false;
		useLocals = false;
		useExceptionTrace = false;
		useSelfTailCall = false;
	};

	//
	// Analysis
	//

	bool CompileCppFunction::prepare() {
		size_t length = unit.length();
		state.assign(length + 2, CompileCppState());
		blockStart.assign(length + 2, false);
		finallyEnd.assign(length + 2, false);
		rethrow.assign(length + 2, false);
		finallyTarget.clear();

		// the targets of the pc_ / finally-next of each finally:
		// context-set-pc / context-set-finally-next (normal end of the try)
		// and the try-break / continue / return / throw of the try (and of its catch)
		AsmLabel finallyLabel = 0;
		auto addFinallyTarget = [&](AsmLabel finallyOf, AsmLabel target, bool withValue) {
			std::vector<std::pair<AsmLabel, bool>> &list = finallyTarget[finallyOf];
			for (auto &scan : list) {
				if (scan.first == target) {
					return;
				};
			};
			list.push_back(std::pair<AsmLabel, bool>(target, withValue));
		};
		for (AsmLabel label : region.code) {
			CodeInstruction &in = unit.at(label);
			switch (in.asmType) {
			case ParserAsm::ContextSetFinally:
			case ParserAsm::ContextSetTryFinally:
				finallyLabel = in.target;
				break;
			case ParserAsm::ContextSetTryBreak:
			case ParserAsm::ContextSetTryContinue:
				if (finallyLabel && in.target) {
					addFinallyTarget(finallyLabel, in.target, false);
				};
				break;
			case ParserAsm::ContextSetTryReturn:
				if (finallyLabel && in.target) {
					addFinallyTarget(finallyLabel, in.target, true);
				};
				break;
			case ParserAsm::ContextSetTryThrow:
				if (finallyLabel && in.target) {
					addFinallyTarget(finallyLabel, in.target, true);
					if (unit.at(in.target).asmType == ParserAsm::Throw) {
						rethrow[in.target] = true;
					};
				};
				break;
			case ParserAsm::ContextSetCatch:
			case ParserAsm::ContextSetBreak:
			case ParserAsm::ContextSetContinue:
			case ParserAsm::ContextSetStack:
			case ParserAsm::EnterContext:
				break;
			case ParserAsm::ContextSetPC: {
				// the finally follows: enter-context, context-set-pc, [finally]
				AsmLabel finallyOf = label + 1;
				if (finallyOf > length) {
					return fail(label, "Unsupported finally");
				};
				addFinallyTarget(finallyOf, in.target, false);
				// the return at the end of the finally continues at pc_
				if (in.target < 2 || unit.at(in.target - 1).asmType != ParserAsm::Return) {
					return fail(label, "Unsupported finally");
				};
				finallyEnd[in.target - 1] = true;
				finallyLabel = 0;
				break;
			};
			case ParserAsm::ContextSetFinallyNext: {
				// the finally follows: enter-context, context-set-break / continue, context-set-finally-next, [finally]
				AsmLabel finallyOf = label + 1;
				if (finallyOf > length) {
					return fail(label, "Unsupported finally");
				};
				addFinallyTarget(finallyOf, in.target, false);
				// finally-end continues at finally-next
				if (in.target < 2 || unit.at(in.target - 1).asmType != ParserAsm::FinallyEnd) {
					return fail(label, "Unsupported finally");
				};
				finallyLabel = 0;
				break;
			};
			default:
				finallyLabel = 0;
				break;
			};
		};
		return true;
	};

	bool CompileCppFunction::merge(AsmLabel label, const CompileCppState &value, std::vector<AsmLabel> &work) {
		CompileCppState &current = state[label];
		if (!current.isSet) {
			current = value;
			current.isSet = true;
			work.push_back(label);
			return true;
		};
		if (current.stack.size() != value.stack.size()) {
			return fail(label, "Unsupported code, operand stack");
		};
		bool changed = false;
		size_t k;
		if (current.context.size() != value.context.size()) {
			return fail(label, "Unsupported code, contexts");
		};
		for (k = 0; k < value.context.size(); ++k) {
			CompileCppContext context = value.context[k];
			// after for-in loops: the reference is not known, -2
			if (context.referenceLocal != current.context[k].referenceLocal) {
				context.referenceLocal = -2;
				if (current.context[k].referenceLocal != -2) {
					current.context[k].referenceLocal = -2;
					changed = true;
				};
			};
			if (!(current.context[k] == context)) {
				return fail(label, "Unsupported code, contexts");
			};
		};
		for (k = 0; k < value.stack.size(); ++k) {
			if (!(current.stack[k] == value.stack[k])) {
				if (current.stack[k].kind != CompileCppEntry::Dynamic) {
					current.stack[k] = CompileCppEntry();
					changed = true;
				};
			};
		};
		if (changed) {
			work.push_back(label);
		};
		return true;
	};

	bool CompileCppFunction::analyze() {
		if (!prepare()) {
			return false;
		};
		if (region.code.empty()) {
			contextCount = 1;
			return true;
		};

		std::vector<AsmLabel> work;
		CompileCppState initial;
		initial.isSet = true;
		initial.context.push_back(CompileCppContext());
		AsmLabel entryLabel = region.code[0];
		blockStart[entryLabel] = true;
		if (!merge(entryLabel, initial, work)) {
			return false;
		};

		std::vector<CompileCppEdge> edges;
		while (!work.empty()) {
			AsmLabel label = work.back();
			work.pop_back();
			edges.clear();
			if (!step(label, state[label], edges)) {
				return false;
			};
			for (CompileCppEdge &edge : edges) {
				if (edge.target == 0 || regionOf[edge.target] != regionIndex) {
					return fail(label, "Unsupported jump");
				};
				if (!edge.isFallThrough) {
					blockStart[edge.target] = true;
				};
				if (!merge(edge.target, edge.state, work)) {
					return false;
				};
			};
		};

		// a block starts after a yield, also after an instruction that does not continue
		contextCount = 1;
		for (AsmLabel label : region.code) {
			if (!state[label].isSet) {
				continue;
			};
			if (state[label].context.size() > contextCount) {
				contextCount = state[label].context.size();
			};
			// a successor reached by fall through and by a jump
			AsmLabel previous = label - 1;
			if (label > 1 && regionOf[previous] == regionIndex && state[previous].isSet && !isTerminal(unit.at(previous).asmType)) {
				continue;
			};
			blockStart[label] = true;
		};
		return true;
	};

	bool CompileCppFunction::step(AsmLabel label, const CompileCppState &in_, std::vector<CompileCppEdge> &edges) {
		CodeInstruction &in = unit.at(label);
		CompileCppState s = in_;
		CompileCppEdge edge;
		AsmLabel nextLabel = next(label);

		auto pop = [&](size_t count) -> bool {
			if (s.stack.size() < count) {
				return false;
			};
			s.stack.resize(s.stack.size() - count);
			return true;
		};
		auto push = [&](const CompileCppEntry &entry) {
			s.stack.push_back(entry);
		};
		auto addEdge = [&](AsmLabel target, const CompileCppState &value, bool isFallThrough) {
			edge.target = target;
			edge.state = value;
			edge.isFallThrough = isFallThrough;
			edges.push_back(edge);
		};
		auto addWalk = [&](const CompileCppState &from, const CompileCppWalk &walk) -> bool {
			CompileCppState target;
			if (walk.kind == CompileCppWalk::Jump || walk.kind == CompileCppWalk::Catch || walk.kind == CompileCppWalk::Finally) {
				if (!walkState(from, walk, target)) {
					return fail(label, "Unsupported try");
				};
				addEdge(walk.target, target, false);
			};
			return true;
		};

		// exception: to the handler of the try
		if (mayThrow(in.asmType)) {
			CompileCppWalk walk = walkThrow(s);
			if (walk.kind != CompileCppWalk::FunctionReturn) {
				if (!addWalk(s, walk)) {
					return false;
				};
				handlerId(s, false);
			};
		};

		bool fallThrough = true;
		switch (in.asmType) {
		case ParserAsm::Nop:
		case ParserAsm::Mark:
		case ParserAsm::Catch:
		case ParserAsm::SymbolPlusPlus:
		case ParserAsm::LocalVariablesPlusPlus:
		case ParserAsm::AssignLocalVariablesObjectReferencePow2LocalVariablesSymbol:
		case ParserAsm::ClearIncludedFile:
			break;

		case ParserAsm::PushUndefined:
		case ParserAsm::PushSymbol:
		case ParserAsm::PushNumber:
		case ParserAsm::PushString:
		case ParserAsm::PushBoolean:
		case ParserAsm::PushNull:
		case ParserAsm::PushGlobal:
		case ParserAsm::ContextPushThis:
		case ParserAsm::ContextPushSelf:
		case ParserAsm::PushArguments:
		case ParserAsm::PushNewObject:
		case ParserAsm::PushNewArray:
		case ParserAsm::XPushNewArray:
		case ParserAsm::LocalVariablesPushSymbol:
		case ParserAsm::ArgumentsPushSymbol:
		case ParserAsm::ArgumentsLevelPushSymbol:
		case ParserAsm::LocalVariablesLevelPushSymbol:
		case ParserAsm::ArgumentsLevelPushObjectReference:
		case ParserAsm::LocalVariablesLevelPushObjectReference:
		case ParserAsm::ContextPushRegisterValue:
		case ParserAsm::XPushFunction:
		case ParserAsm::OperatorMinusArgumentsSymbolXNumber:
		case ParserAsm::OperatorMulLocalVariablesSymbol2:
		case ParserAsm::OperatorMinusLocalVariablesSymbol2:
		case ParserAsm::OperatorPlusLocalVariablesSymbol2:
		case ParserAsm::OperatorPow2LocalVariablesSymbol:
		case ParserAsm::OperatorNextReferenceIndex:
			push(CompileCppEntry());
			break;

		case ParserAsm::PushObjectReference:
			push(CompileCppEntry(CompileCppEntry::ReferenceGlobal, (uint32_t)constant.symbolIndex(Context::getSymbolMirror(operandSymbol(in)))));
			break;
		case ParserAsm::LocalVariablesPushObjectReference:
			push(CompileCppEntry(CompileCppEntry::ReferenceLocal, operandIndex(in)));
			break;
		case ParserAsm::ArgumentsPushObjectReference:
			push(CompileCppEntry(CompileCppEntry::ReferenceArgument, operandIndex(in)));
			break;

		case ParserAsm::Reference:
		case ParserAsm::ReferenceObjectReference:
		case ParserAsm::OperatorUnaryPlus:
		case ParserAsm::OperatorNot:
		case ParserAsm::OperatorBitwiseNot:
		case ParserAsm::OperatorTypeOf:
		case ParserAsm::OperatorPlusPlusLeft:
		case ParserAsm::OperatorPlusPlusRight:
		case ParserAsm::OperatorMinusMinusLeft:
		case ParserAsm::OperatorMinusMinusRight:
		case ParserAsm::OperatorSetReferenceIndexKey:
		case ParserAsm::OperatorSetReferenceIndexValue:
		case ParserAsm::OperatorReferenceDeleteReference:
		case ParserAsm::XCallSymbol:
		case ParserAsm::OperatorEqual1:
		case ParserAsm::OperatorAssignXPrototype:
			if (!pop(1)) {
				return fail(label, "Invalid operand stack");
			};
			push(CompileCppEntry());
			break;

		case ParserAsm::OperatorReference:
		case ParserAsm::OperatorReferenceReference:
		case ParserAsm::OperatorAssign:
		case ParserAsm::OperatorEqual:
		case ParserAsm::OperatorEqualStrict:
		case ParserAsm::OperatorPlus:
		case ParserAsm::OperatorMinus:
		case ParserAsm::OperatorMul:
		case ParserAsm::OperatorDiv:
		case ParserAsm::OperatorMod:
		case ParserAsm::OperatorNotEqual:
		case ParserAsm::OperatorNotEqualStrict:
		case ParserAsm::OperatorLessOrEqual:
		case ParserAsm::OperatorLess:
		case ParserAsm::OperatorGreaterOrEqual:
		case ParserAsm::OperatorGreater:
		case ParserAsm::OperatorLogicalOr:
		case ParserAsm::OperatorBitwiseOr:
		case ParserAsm::OperatorLogicalAnd:
		case ParserAsm::OperatorBitwiseAnd:
		case ParserAsm::OperatorBitwiseLeftShift:
		case ParserAsm::OperatorBitwiseRightShift:
		case ParserAsm::OperatorBitwiseRightShiftZero:
		case ParserAsm::OperatorBitwiseXor:
		case ParserAsm::OperatorAssignPlus:
		case ParserAsm::OperatorAssignMinus:
		case ParserAsm::OperatorAssignMod:
		case ParserAsm::OperatorAssignDiv:
		case ParserAsm::OperatorAssignMul:
		case ParserAsm::OperatorAssignBitwiseOr:
		case ParserAsm::OperatorAssignBitwiseAnd:
		case ParserAsm::OperatorAssignBitwiseLeftShift:
		case ParserAsm::OperatorAssignBitwiseRightShift:
		case ParserAsm::OperatorAssignBitwiseRightShiftZero:
		case ParserAsm::OperatorAssignBitwiseXor:
		case ParserAsm::OperatorIn:
		case ParserAsm::OperatorInstanceOf:
		case ParserAsm::OperatorArrayPush:
		case ParserAsm::OperatorReferenceDeleteIndex:
		case ParserAsm::AssignNewObject:
		case ParserAsm::XCall:
		case ParserAsm::XCallWithThisReference:
			if (!pop(2)) {
				return fail(label, "Invalid operand stack");
			};
			push(CompileCppEntry());
			break;

		case ParserAsm::XCallThis:
		case ParserAsm::XCallThisModeCall:
		case ParserAsm::XCallThisModeApply:
			if (!pop(3)) {
				return fail(label, "Invalid operand stack");
			};
			push(CompileCppEntry());
			break;

		case ParserAsm::Assign:
		case ParserAsm::AssignReverse:
			if (!pop(2)) {
				return fail(label, "Invalid operand stack");
			};
			break;

		case ParserAsm::Pop1:
		case ParserAsm::ContextSetRegisterValue:
		case ParserAsm::ArrayPush:
		case ParserAsm::XArrayPushWithTransfer:
			if (!pop(1)) {
				return fail(label, "Invalid operand stack");
			};
			break;

		case ParserAsm::ContextSetReference: {
			if (s.stack.empty()) {
				return fail(label, "Invalid operand stack");
			};
			CompileCppEntry entry = s.stack.back();
			pop(1);
			s.context.back().referenceLocal = -1;
			if (entry.kind == CompileCppEntry::ReferenceLocal && !region.heapLocals) {
				s.context.back().referenceLocal = (int)entry.index;
			};
			break;
		};

		case ParserAsm::Duplicate: {
			if (s.stack.empty()) {
				return fail(label, "Invalid operand stack");
			};
			CompileCppEntry entry = s.stack.back();
			push(entry);
			break;
		};

		case ParserAsm::IfFalseGoto:
		case ParserAsm::IfTrueGoto:
			if (!pop(1)) {
				return fail(label, "Invalid operand stack");
			};
			addEdge(in.target, s, false);
			break;
		case ParserAsm::IfNotGreaterGoto:
			if (!pop(2)) {
				return fail(label, "Invalid operand stack");
			};
			addEdge(in.target, s, false);
			break;
		case ParserAsm::IfArgumentsSymbolNotEqualNumberGoto:
		case ParserAsm::IfSymbolNotLessNumberGoto:
		case ParserAsm::IfArgumentsSymbolNotLessNumberGoto:
			addEdge(in.target, s, false);
			break;
		case ParserAsm::Goto:
			addEdge(in.target, s, false);
			fallThrough = false;
			break;

		case ParserAsm::ContextSetBreak:
			s.context.back().break_ = in.target;
			break;
		case ParserAsm::ContextSetContinue:
			s.context.back().continue_ = in.target;
			break;
		case ParserAsm::ContextSetCatch:
			s.context.back().catch_ = in.target;
			break;
		case ParserAsm::ContextSetFinally:
			s.context.back().finally_ = in.target;
			break;
		case ParserAsm::ContextSetTryFinally:
			s.context.back().tryFinally_ = in.target;
			break;
		case ParserAsm::ContextSetTryBreak:
			s.context.back().tryBreak_ = in.target;
			break;
		case ParserAsm::ContextSetTryContinue:
			s.context.back().tryContinue_ = in.target;
			break;
		case ParserAsm::ContextSetTryReturn:
			s.context.back().tryReturn_ = in.target;
			break;
		case ParserAsm::ContextSetTryThrow:
			s.context.back().tryThrow_ = in.target;
			break;
		case ParserAsm::ContextSetPC:
		case ParserAsm::ContextSetFinallyNext:
			if (s.context.size() < 2) {
				return fail(label, "Unsupported context");
			};
			s.context.back().finallyOf = label + 1;
			break;
		case ParserAsm::ContextSetStack:
			s.context.back().stackLink = (int)s.stack.size();
			break;
		case ParserAsm::EnterContext:
			s.context.push_back(CompileCppContext());
			break;
		case ParserAsm::LeaveContext:
			if (s.context.size() < 2) {
				return fail(label, "Unsupported context");
			};
			s.context.pop_back();
			break;

		case ParserAsm::Break:
		case ParserAsm::Continue: {
			CompileCppWalk walk = walkBreak(s, in.asmType == ParserAsm::Continue);
			if (!addWalk(s, walk)) {
				return false;
			};
			fallThrough = false;
			break;
		};

		case ParserAsm::FinallyEnd:
		case ParserAsm::Return:
			if (finallyEnd[label] || (in.asmType == ParserAsm::FinallyEnd)) {
				// continue at pc_ / finally-next of the finally context
				if (s.context.size() < 2 || s.context.back().finallyOf == 0) {
					return fail(label, "Unsupported finally");
				};
				AsmLabel finallyOf = s.context.back().finallyOf;
				s.context.pop_back();
				for (auto &target : finallyTarget[finallyOf]) {
					CompileCppState targetState = s;
					if (target.second) {
						targetState.stack.push_back(CompileCppEntry());
					};
					addEdge(target.first, targetState, false);
				};
				fallThrough = false;
				break;
			};
			// fall through
		case ParserAsm::XTailCall:
		case ParserAsm::XTailCallSymbol:
		case ParserAsm::XTailCallThis:
		case ParserAsm::XTailCallThisModeCall:
		case ParserAsm::XTailCallThisModeApply:
		case ParserAsm::XTailCallWithThisReference:
		case ParserAsm::CurrentContextExit: {
			size_t count = 1;
			switch (in.asmType) {
			case ParserAsm::XTailCall:
			case ParserAsm::XTailCallWithThisReference:
				count = 2;
				break;
			case ParserAsm::XTailCallThis:
			case ParserAsm::XTailCallThisModeCall:
			case ParserAsm::XTailCallThisModeApply:
				count = 3;
				break;
			default:
				break;
			};
			if (!pop(count)) {
				return fail(label, "Invalid operand stack");
			};
			if (in.asmType != ParserAsm::CurrentContextExit) {
				if (!addWalk(s, walkReturn(s))) {
					return false;
				};
			};
			fallThrough = false;
			break;
		};

		case ParserAsm::EndExecution:
			fallThrough = false;
			break;

		case ParserAsm::Throw:
			if (!pop(1)) {
				return fail(label, "Invalid operand stack");
			};
			fallThrough = false;
			break;

		case ParserAsm::Yield:
			if (!pop(1)) {
				return fail(label, "Invalid operand stack");
			};
			if (region.isCoroutine) {
				if (!nextLabel) {
					return fail(label, "Unsupported yield");
				};
				addEdge(nextLabel, s, false);
			};
			fallThrough = false;
			break;

		default:
			return fail(label, "Unsupported instruction");
		};

		if (fallThrough) {
			if (!nextLabel) {
				// the end of the top level code
				if (regionIndex == 0 && (size_t)label == unit.length()) {
					return true;
				};
				return fail(label, "Unsupported code, no next instruction");
			};
			addEdge(nextLabel, s, true);
		};
		return true;
	};

	//
	// Code
	//

	bool CompileCppFunction::emitWalk(const CompileCppState &from, const CompileCppWalk &walk, const String &value) {
		CompileCppState target;
		switch (walk.kind) {
		case CompileCppWalk::Jump:
			return jump(walk.target);
		case CompileCppWalk::Finally: {
			if (!walkState(from, walk, target)) {
				error = "Unsupported try";
				return false;
			};
			if (walk.withValue) {
				line(contextVariable(usePending, "pd", walk.contextIndex) << " = " << value << ";");
			};
			line(contextVariable(usePc, "pc", walk.contextIndex) << " = " << toStringSize(walk.pc) << ";");
			std::vector<CompileCppValue> values(stack.begin(), stack.begin() + target.stack.size());
			if (!transfer(values, state[walk.target])) {
				return false;
			};
			line(String("goto L") << toStringSize(walk.target) << ";");
			return true;
		};
		case CompileCppWalk::FunctionReturn:
			line(returnValue(value));
			return true;
		default:
			break;
		};
		error = "Unsupported walk";
		return false;
	};

	bool CompileCppFunction::emitCall(CodeInstruction &in) {
		CompileCppValue functionArguments;
		CompileCppValue function;
		CompileCppValue this_;
		String result;
		switch (in.asmType) {
		case ParserAsm::XCall:
		case ParserAsm::XTailCall: {
			functionArguments = pop();
			function = pop();
			String vFunction = asVariable(function);
			String vArguments = asVariable(functionArguments);
			result = String("call(module, ") << vFunction << ", module->undefined, (VariableArray *)" << vArguments << ")";
			break;
		};
		case ParserAsm::XCallSymbol:
		case ParserAsm::XTailCallSymbol: {
			functionArguments = pop();
			String vArguments = asVariable(functionArguments);
			result = String("callSymbol(module, ") << symbol(operandSymbol(in)) << ", (VariableArray *)" << vArguments << ")";
			break;
		};
		case ParserAsm::XCallThis:
		case ParserAsm::XTailCallThis: {
			functionArguments = pop();
			function = pop();
			this_ = pop();
			String vThis = asVariable(this_);
			String vFunction = asVariable(function);
			String vArguments = asVariable(functionArguments);
			result = String("call(module, ") << vFunction << ", " << vThis << ", (VariableArray *)" << vArguments << ")";
			break;
		};
		case ParserAsm::XCallThisModeCall:
		case ParserAsm::XTailCallThisModeCall:
		case ParserAsm::XCallThisModeApply:
		case ParserAsm::XTailCallThisModeApply: {
			functionArguments = pop();
			this_ = pop();
			function = pop();
			String vFunction = asVariable(function);
			String vThis = asVariable(this_);
			String vArguments = asVariable(functionArguments);
			if (in.asmType == ParserAsm::XCallThisModeCall || in.asmType == ParserAsm::XTailCallThisModeCall) {
				result = String("call(module, ") << vFunction << ", " << vThis << ", (VariableArray *)" << vArguments << ")";
				break;
			};
			result = String("callThisModeApply(module, ") << vFunction << ", " << vThis << ", (VariableArray *)" << vArguments << ")";
			break;
		};
		case ParserAsm::XCallWithThisReference:
		case ParserAsm::XTailCallWithThisReference: {
			functionArguments = pop();
			this_ = pop();
			String vThis = asVariable(this_);
			String vArguments = asVariable(functionArguments);
			result = String("callWithThisReference(module, ") << vThis << ", " << symbol(operandSymbol(in)) << ", (VariableArray *)" << vArguments << ")";
			break;
		};
		default:
			error = "Unsupported call";
			return false;
		};
		pushVariable(result);
		return true;
	};

	// return f(...) where f is this function: the C++ function starts again,
	// as the tail call of the interpreter, recursion without stack
	void CompileCppFunction::emitSelfTailCall(CodeInstruction &in) {
		size_t size = stack.size();
		String vFunction;
		String vThis = "module->undefined";
		String vArguments = asVariable(stack[size - 1]);
		switch (in.asmType) {
		case ParserAsm::XTailCall:
			vFunction = asVariable(stack[size - 2]);
			break;
		case ParserAsm::XTailCallSymbol:
			vFunction = newTemp("t");
			line(String("TPointer<Variable> ") << vFunction << "(" << global() << "->getPropertyBySymbol(" << symbol(operandSymbol(in)) << "));");
			vFunction << ".value()";
			break;
		case ParserAsm::XTailCallThis:
			vFunction = asVariable(stack[size - 2]);
			vThis = asVariable(stack[size - 3]);
			break;
		case ParserAsm::XTailCallThisModeCall:
			vFunction = asVariable(stack[size - 3]);
			vThis = asVariable(stack[size - 2]);
			break;
		case ParserAsm::XTailCallWithThisReference:
			vThis = asVariable(stack[size - 2]);
			vFunction = newTemp("t");
			line(String("TPointer<Variable> ") << vFunction << "((" << vThis << ")->getPropertyBySymbol(" << symbol(operandSymbol(in)) << "));");
			vFunction << ".value()";
			break;
		default:
			return;
		};
		useSelfTailCall = true;
		line(String("if (") << vFunction << " == (Variable *)function) {");
		++indent;
		line(String("thisValue = ") << vThis << ";");
		line("this_ = thisValue;");
		line(String("arguments_ = (VariableArray *)(") << vArguments << ");");
		line("arguments = arguments_;");
		line("selfTailCall();");
		line(String("goto L") << toStringSize(region.code[0]) << ";");
		--indent;
		line("};");
	};

	bool CompileCppFunction::emitInstruction(AsmLabel label, bool &fallThrough) {
		CodeInstruction &in = unit.at(label);
		const CompileCppState &current = state[label];
		int opcode = in.asmType;
		fallThrough = true;

		if (stack.size() != current.stack.size()) {
			return fail(label, "Invalid operand stack");
		};

		if (mayThrow(opcode)) {
			setLine(in);
			setTryState(current);
		};

		switch (opcode) {
		case ParserAsm::Nop:
		case ParserAsm::Mark:
		case ParserAsm::ClearIncludedFile:
			return true;

		// --- jumps

		case ParserAsm::Goto:
			fallThrough = false;
			return jump(in.target);
		case ParserAsm::IfFalseGoto:
		case ParserAsm::IfTrueGoto: {
			CompileCppValue value = pop();
			String condition = newTemp("b");
			line(String("bool ") << condition << " = " << asBoolean(value) << ";");
			line(String("if (") << ((opcode == ParserAsm::IfFalseGoto) ? String("!") + condition : condition) << ") {");
			++indent;
			if (!jump(in.target)) {
				return false;
			};
			--indent;
			line("};");
			return true;
		};
		case ParserAsm::IfNotGreaterGoto: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String condition;
			if (withInline) {
				condition = emitCompare(a, b, ">", "isGreater");
			} else {
				condition = newTemp("b");
				String va = asVariable(a);
				String vb = asVariable(b);
				line(String("bool ") << condition << " = (" << va << ")->isGreater(" << vb << ");");
			};
			line(String("if (!") << condition << ") {");
			++indent;
			if (!jump(in.target)) {
				return false;
			};
			--indent;
			line("};");
			return true;
		};
		case ParserAsm::IfArgumentsSymbolNotEqualNumberGoto:
		case ParserAsm::IfArgumentsSymbolNotLessNumberGoto:
		case ParserAsm::IfSymbolNotLessNumberGoto: {
			VariableOperator31 *operand = (VariableOperator31 *)in.operand.value();
			String value;
			if (opcode == ParserAsm::IfSymbolNotLessNumberGoto) {
				value = newTemp("t");
				line(String("TPointer<Variable> ") << value << "(" << global() << "->getPropertyBySymbol(" << symbol(operand->symbol) << "));");
			} else {
				value = String("arguments->index(") << toStringSize((uint32_t)operand->symbol) << ")";
			};
			String condition = newTemp("b");
			if (opcode == ParserAsm::IfArgumentsSymbolNotEqualNumberGoto) {
				line(String("bool ") << condition << " = (" << value << "->toNumber() != " << toStringNumber(operand->value) << ");");
			} else {
				line(String("bool ") << condition << " = !(" << value << "->toNumber() < " << toStringNumber(operand->value) << ");");
			};
			line(String("if (") << condition << ") {");
			++indent;
			if (!jump(in.target)) {
				return false;
			};
			--indent;
			line("};");
			return true;
		};

		// --- contexts

		case ParserAsm::EnterContext:
		case ParserAsm::LeaveContext:
		case ParserAsm::ContextSetBreak:
		case ParserAsm::ContextSetContinue:
		case ParserAsm::ContextSetCatch:
		case ParserAsm::ContextSetFinally:
		case ParserAsm::ContextSetTryFinally:
		case ParserAsm::ContextSetTryBreak:
		case ParserAsm::ContextSetTryContinue:
		case ParserAsm::ContextSetTryReturn:
		case ParserAsm::ContextSetTryThrow:
		case ParserAsm::ContextSetStack:
			return true;
		case ParserAsm::ContextSetPC:
		case ParserAsm::ContextSetFinallyNext:
			line(contextVariable(usePc, "pc", current.context.size() - 1) << " = " << toStringSize(in.target) << ";");
			return true;
		case ParserAsm::ContextSetRegisterValue: {
			CompileCppValue value = pop();
			String vValue = asVariable(value);
			line(contextVariable(useRegister, "rv", current.context.size() - 1) << " = " << vValue << ";");
			return true;
		};
		case ParserAsm::ContextPushRegisterValue:
			pushVariable(contextVariable(useRegister, "rv", current.context.size() - 1));
			return true;
		case ParserAsm::ContextSetReference: {
			CompileCppValue value = pop();
			if (state[next(label)].context.back().referenceLocal >= 0) {
				// for-in on a C++ local variable, the iterator sets the value of the module
				return true;
			};
			String vValue = asVariable(value);
			line(contextVariable(useReference, "rf", current.context.size() - 1) << " = " << vValue << ";");
			return true;
		};
		case ParserAsm::OperatorSetReferenceIndexKey:
		case ParserAsm::OperatorSetReferenceIndexValue:
		case ParserAsm::OperatorNextReferenceIndex: {
			size_t depth = current.context.size() - 1;
			String iterator = contextVariable(useIterator, "it", depth);
			if (opcode != ParserAsm::OperatorNextReferenceIndex) {
				CompileCppValue value = pop();
				String vValue = asVariable(value);
				line(String(iterator) << " = (" << vValue << ")->" << ((opcode == ParserAsm::OperatorSetReferenceIndexKey) ? "getIteratorKey" : "getIteratorValue") << "();");
			};
			String result = newTemp("b");
			int referenceLocal = current.context.back().referenceLocal;
			if (referenceLocal == -2) {
				error = "Unsupported for-in";
				return false;
			};
			if (referenceLocal >= 0) {
				spillLocal((uint32_t)referenceLocal);
				line(String("bool ") << result << " = " << iterator << "->next(module->iteratorReference);");
				line(local((uint32_t)referenceLocal) << " = module->iteratorValue->index(0);");
			} else {
				line(String("bool ") << result << " = " << iterator << "->next(" << contextVariable(useReference, "rf", depth) << ");");
			};
			CompileCppValue value;
			value.type = CompileCppValue::TypeBoolean;
			value.name = result;
			stack.push_back(value);
			return true;
		};

		// --- control

		case ParserAsm::Break:
		case ParserAsm::Continue: {
			fallThrough = false;
			CompileCppWalk walk = walkBreak(current, opcode == ParserAsm::Continue);
			if (walk.kind == CompileCppWalk::Error) {
				setLine(in);
				line(String("throw Error(\"") << ((opcode == ParserAsm::Continue) ? "continue without loop" : "break without loop/switch") << "\");");
				return true;
			};
			return emitWalk(current, walk, "");
		};
		case ParserAsm::FinallyEnd:
		case ParserAsm::Return: {
			fallThrough = false;
			if (finallyEnd[label] || (opcode == ParserAsm::FinallyEnd)) {
				size_t depth = current.context.size() - 1;
				AsmLabel finallyOf = current.context.back().finallyOf;
				line(String("switch (") << contextVariable(usePc, "pc", depth) << ") {");
				for (auto &target : finallyTarget[finallyOf]) {
					line(String("\tcase ") << toStringSize(target.first) << ": {");
					indent += 2;
					std::vector<CompileCppValue> values = stack;
					if (target.second) {
						CompileCppValue value;
						value.type = CompileCppValue::TypeBorrowed;
						value.name = contextVariable(usePending, "pd", depth) + ".value()";
						values.push_back(value);
					};
					if (!transfer(values, state[target.first])) {
						return false;
					};
					line(String("goto L") << toStringSize(target.first) << ";");
					indent -= 2;
					line("\t};");
				};
				line("\tdefault:");
				line("\t\tbreak;");
				line("};");
				line("throw Error(\"Invalid finally\");");
				return true;
			};
			CompileCppValue value = pop();
			String vValue = asVariable(value);
			return emitWalk(current, walkReturn(current), vValue);
		};
		case ParserAsm::CurrentContextExit: {
			fallThrough = false;
			CompileCppValue value = pop();
			String vValue = asVariable(value);
			line(returnValue(vValue));
			return true;
		};
		case ParserAsm::EndExecution:
			fallThrough = false;
			line(returnValue("module->undefined"));
			return true;
		case ParserAsm::Throw: {
			fallThrough = false;
			CompileCppValue value = pop();
			String vValue = asVariable(value);
			if (rethrow[label]) {
				useExceptionTrace = true;
				line("{");
				line(String("\tException exception(newException(module, ") << vValue << "));");
				line("\texception.stackTrace = exceptionTrace;");
				line("\tthrow exception;");
				line("};");
				return true;
			};
			line(String("throw newException(module, ") << vValue << ");");
			return true;
		};
		case ParserAsm::Yield: {
			fallThrough = false;
			CompileCppValue value = pop();
			String vValue = asVariable(value);
			if (!region.isCoroutine) {
				line("throw Error(\"yield\");");
				return true;
			};
			AsmLabel resumeLabel = next(label);
			String yieldValue = newTemp("t");
			line(String("TPointer<Variable> ") << yieldValue << "(" << vValue << ");");
			if (!transfer(stack, state[resumeLabel])) {
				return false;
			};
			resume.push_back(resumeLabel);
			line(String("function->yieldStep = ") << toStringSize(handlerWalk.size() + resume.size()) << ";");
			line("yieldSave();");
			line(String("return ") << yieldValue << ";");
			return true;
		};
		case ParserAsm::Catch: {
			useExceptionTrace = true;
			CompileCppValue &value = stack.back();
			String vValue = asVariable(value);
			line(String("setStackTrace(module, ") << vValue << ", exceptionTrace);");
			line("exceptionTrace = nullptr;");
			return true;
		};

		// --- calls

		case ParserAsm::XCall:
		case ParserAsm::XCallSymbol:
		case ParserAsm::XCallThis:
		case ParserAsm::XCallThisModeCall:
		case ParserAsm::XCallThisModeApply:
		case ParserAsm::XCallWithThisReference:
			return emitCall(in);
		case ParserAsm::XTailCall:
		case ParserAsm::XTailCallSymbol:
		case ParserAsm::XTailCallThis:
		case ParserAsm::XTailCallThisModeCall:
		case ParserAsm::XTailCallThisModeApply:
		case ParserAsm::XTailCallWithThisReference: {
			fallThrough = false;
			CompileCppWalk walk = walkReturn(current);
			if (walk.kind == CompileCppWalk::FunctionReturn && !region.isCoroutine && opcode != ParserAsm::XTailCallThisModeApply) {
				emitSelfTailCall(in);
			};
			if (!emitCall(in)) {
				return false;
			};
			CompileCppValue value = pop();
			String vValue = asVariable(value);
			return emitWalk(current, walk, vValue);
		};

		// --- functions

		case ParserAsm::XPushFunction: {
			std::map<AsmLabel, size_t>::iterator it = owner.regionByBegin.find(in.target);
			if (it == owner.regionByBegin.end()) {
				return fail(label, "Invalid function");
			};
			CompileCppRegion &child = regions[it->second];
			String localVariables = "nullptr";
			if (region.heapLocals) {
				localVariables = localsArray();
			};
			if (child.isCoroutine) {
				pushVariable(String("newFunctionWithYield(module, function->functionParent.value(), ") << localVariables << ", arguments, " << child.name << ", " << toStringSize(child.contextCount) << ", " << toStringSize(child.contextCount + 1) << ")");
				return true;
			};
			pushVariable(String("newFunction(module, function->functionParent.value(), ") << localVariables << ", arguments, " << child.name << ")");
			return true;
		};

		default:
			break;
		};

		if (withInline) {
			if (emitInline(in)) {
				return true;
			};
			if (error.length()) {
				return false;
			};
		};
		if (emitRuntime(in)) {
			return true;
		};
		if (error.length() == 0) {
			fail(label, "Unsupported instruction");
		};
		return false;
	};

	// simple instructions as C++ code
	bool CompileCppFunction::emitInline(CodeInstruction &in) {
		switch (in.asmType) {
		case ParserAsm::PushBoolean: {
			bool value = ((VariableBoolean *)in.operand.value())->value;
			CompileCppValue &item = pushBorrowed(value ? "module->valueTrue" : "module->valueFalse");
			item.hasBoolean = true;
			item.boolean = value;
			return true;
		};
		case ParserAsm::PushNumber: {
			Number value = ((VariableNumber *)in.operand.value())->value;
			CompileCppValue &item = pushBorrowed(String("module->number[") << toStringSize(constant.numberIndex(value)) << "]");
			item.hasNumber = true;
			item.number = value;
			return true;
		};
		case ParserAsm::LocalVariablesPushSymbol:
			if (isCppLocals()) {
				CompileCppValue value;
				value.type = CompileCppValue::TypeLocal;
				value.index = operandIndex(in);
				local(value.index);
				stack.push_back(value);
				return true;
			};
			return false;
		case ParserAsm::Reference: {
			CompileCppValue a = pop();
			String va = asVariable(a);
			pushVariable(String("(") << va << ")->getPropertyBySymbol(" << symbol(operandSymbol(in)) << ")");
			return true;
		};
		case ParserAsm::ReferenceObjectReference: {
			CompileCppValue a = pop();
			String object = asVariable(a);
			pushReference(CompileCppValue::ReferenceSymbol, object, symbol(operandSymbol(in)));
			return true;
		};
		case ParserAsm::OperatorReferenceReference: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String va = asVariable(a);
			String vb = asVariable(b);
			pushReference(CompileCppValue::ReferenceVariable, va, vb);
			return true;
		};
		case ParserAsm::LocalVariablesLevelPushObjectReference:
		case ParserAsm::ArgumentsLevelPushObjectReference: {
			VariableArgumentLevel *operand = (VariableArgumentLevel *)in.operand.value();
			String object = newTemp("o");
			line(String("VariableArray *") << object << " = " << ((in.asmType == ParserAsm::LocalVariablesLevelPushObjectReference) ? "localVariablesLevelReference" : "argumentsLevelReference") << "(function->functionParent.value(), " << toStringInt(operand->level) << ");");
			pushReference(CompileCppValue::ReferenceIndex, object, toStringInt(operand->value));
			return true;
		};

		// --- arithmetic

		case ParserAsm::OperatorPlus: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			if (a.isNumber() && b.isNumber()) {
				pushNumber(String("(") << asNumber(a) << ") + (" << asNumber(b) << ")");
				return true;
			};
			String conditionA;
			String conditionB;
			String numberA;
			String numberB;
			if (asNumberPath(a, conditionA, numberA) && asNumberPath(b, conditionB, numberB)) {
				// numbers without Variable::operatorPlus, at least one condition
				String condition = conditionA;
				if (conditionB.length()) {
					if (condition.length()) {
						condition << " && ";
					};
					condition << conditionB;
				};
				String result = newTemp("t");
				line(String("TPointer<Variable> ") << result << ";");
				line(String("if (") << condition << ") {");
				line(String("\t") << result << " = VariableNumber::newVariable((" << numberA << ") + (" << numberB << "));");
				line("} else {");
				String va = asVariableLocal(a);
				String vb = asVariableLocal(b);
				line(String("\t") << result << " = (" << va << ")->operatorPlus(" << vb << ");");
				line("};");
				CompileCppValue value;
				value.type = CompileCppValue::TypeVariable;
				value.name = result;
				stack.push_back(value);
				return true;
			};
			String va = asVariable(a);
			String vb = asVariable(b);
			pushVariable(String("(") << va << ")->operatorPlus(" << vb << ")");
			return true;
		};
		case ParserAsm::OperatorMinus:
		case ParserAsm::OperatorMul:
		case ParserAsm::OperatorDiv:
		case ParserAsm::OperatorMod: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String na = newTemp("n");
			line(String("Number ") << na << " = " << asNumber(a) << ";");
			String nb = asNumber(b);
			switch (in.asmType) {
			case ParserAsm::OperatorMinus:
				pushNumber(na + " - (" + nb + ")");
				break;
			case ParserAsm::OperatorMul:
				pushNumber(na + " * (" + nb + ")");
				break;
			case ParserAsm::OperatorDiv:
				pushNumber(na + " / (" + nb + ")");
				break;
			default:
				pushNumber(String("fmod(") << na << ", " << nb << ")");
				break;
			};
			return true;
		};
		case ParserAsm::OperatorUnaryPlus: {
			CompileCppValue a = pop();
			pushNumber(asNumber(a));
			return true;
		};
		case ParserAsm::OperatorBitwiseOr:
		case ParserAsm::OperatorBitwiseAnd:
		case ParserAsm::OperatorBitwiseXor:
		case ParserAsm::OperatorBitwiseLeftShift:
		case ParserAsm::OperatorBitwiseRightShift:
		case ParserAsm::OperatorBitwiseRightShiftZero: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String na = newTemp("n");
			line(String("Number ") << na << " = " << asNumber(a) << ";");
			String nb = asNumber(b);
			const char *procedure = "bitwiseOr";
			switch (in.asmType) {
			case ParserAsm::OperatorBitwiseAnd:
				procedure = "bitwiseAnd";
				break;
			case ParserAsm::OperatorBitwiseXor:
				procedure = "bitwiseXor";
				break;
			case ParserAsm::OperatorBitwiseLeftShift:
				procedure = "bitwiseLeftShift";
				break;
			case ParserAsm::OperatorBitwiseRightShift:
				procedure = "bitwiseRightShift";
				break;
			case ParserAsm::OperatorBitwiseRightShiftZero:
				procedure = "bitwiseRightShiftZero";
				break;
			default:
				break;
			};
			pushNumber(String(procedure) << "(" << na << ", " << nb << ")");
			return true;
		};
		case ParserAsm::OperatorBitwiseNot: {
			CompileCppValue a = pop();
			pushNumber(String("bitwiseNot(") << asNumber(a) << ")");
			return true;
		};

		// --- compare

		case ParserAsm::OperatorLess:
		case ParserAsm::OperatorLessOrEqual:
		case ParserAsm::OperatorGreater:
		case ParserAsm::OperatorGreaterOrEqual:
		case ParserAsm::OperatorEqual:
		case ParserAsm::OperatorNotEqual:
		case ParserAsm::OperatorEqualStrict:
		case ParserAsm::OperatorNotEqualStrict: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			const char *operatorCpp = "";
			const char *method = "";
			bool negate = false;
			switch (in.asmType) {
			case ParserAsm::OperatorLess:
				operatorCpp = "<";
				method = "isLess";
				break;
			case ParserAsm::OperatorLessOrEqual:
				operatorCpp = "<=";
				method = "isLessOrEqual";
				break;
			case ParserAsm::OperatorGreater:
				operatorCpp = ">";
				method = "isGreater";
				break;
			case ParserAsm::OperatorGreaterOrEqual:
				operatorCpp = ">=";
				method = "isGreaterOrEqual";
				break;
			case ParserAsm::OperatorEqual:
				operatorCpp = "==";
				method = "isEqual";
				break;
			case ParserAsm::OperatorNotEqual:
				operatorCpp = "==";
				method = "isEqual";
				negate = true;
				break;
			case ParserAsm::OperatorEqualStrict:
				operatorCpp = "==";
				method = "isEqualStrict";
				break;
			default:
				operatorCpp = "==";
				method = "isEqualStrict";
				negate = true;
				break;
			};
			String result = emitCompare(a, b, operatorCpp, method);
			CompileCppValue value;
			value.type = CompileCppValue::TypeBoolean;
			value.name = result;
			if (negate) {
				value.name = newTemp("b");
				line(String("bool ") << value.name << " = !" << result << ";");
			};
			stack.push_back(value);
			return true;
		};
		case ParserAsm::OperatorNot: {
			CompileCppValue a = pop();
			pushBoolean(String("!") << asBoolean(a));
			return true;
		};
		case ParserAsm::OperatorLogicalOr:
		case ParserAsm::OperatorLogicalAnd: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String ba = newTemp("b");
			line(String("bool ") << ba << " = " << asBoolean(a) << ";");
			String bb = newTemp("b");
			line(String("bool ") << bb << " = " << asBoolean(b) << ";");
			pushBoolean(String("(") << ba << ((in.asmType == ParserAsm::OperatorLogicalOr) ? " || " : " && ") << bb << ")");
			return true;
		};

		// --- stack

		case ParserAsm::Pop1:
			stack.pop_back();
			return true;
		case ParserAsm::Duplicate: {
			CompileCppValue top = stack.back();
			if (top.type == CompileCppValue::TypeSlot) {
				top.type = CompileCppValue::TypeBorrowed;
				top.name = slot(top.index) + ".value()";
			};
			stack.push_back(top);
			return true;
		};

		// --- assign

		case ParserAsm::Assign: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String vb = asVariable(b);
			referenceSet(a, vb);
			return true;
		};
		case ParserAsm::AssignReverse: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String va = asVariable(a);
			referenceSet(b, va);
			return true;
		};
		case ParserAsm::OperatorAssign: {
			CompileCppValue value = pop();
			CompileCppValue reference = pop();
			if (value.type == CompileCppValue::TypeLocal || value.type == CompileCppValue::TypeSlot) {
				String name = newTemp("t");
				line(String("TPointer<Variable> ") << name << "(" << asVariable(value) << ");");
				value.type = CompileCppValue::TypeVariable;
				value.name = name;
			};
			String vValue = asVariable(value);
			referenceSet(reference, vValue);
			stack.push_back(value);
			return true;
		};
		case ParserAsm::OperatorAssignPlus: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String vb = asVariable(b);
			if (a.type == CompileCppValue::TypeReference || a.type == CompileCppValue::TypeStatic) {
				pushVariable(String("(") << referenceGet(a) << ")->operatorPlus(" << vb << ")");
				referenceSet(a, stack.back().name + ".value()");
				return true;
			};
			String va = asVariable(a);
			pushVariable(String("(") << va << ")->referenceOperatorPlus(" << vb << ")");
			return true;
		};
		case ParserAsm::OperatorAssignMinus:
		case ParserAsm::OperatorAssignMul:
		case ParserAsm::OperatorAssignDiv:
		case ParserAsm::OperatorAssignMod:
		case ParserAsm::OperatorAssignBitwiseOr:
		case ParserAsm::OperatorAssignBitwiseAnd:
		case ParserAsm::OperatorAssignBitwiseXor:
		case ParserAsm::OperatorAssignBitwiseLeftShift:
		case ParserAsm::OperatorAssignBitwiseRightShift:
		case ParserAsm::OperatorAssignBitwiseRightShiftZero: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			bool isReference = (a.type == CompileCppValue::TypeReference) || (a.type == CompileCppValue::TypeStatic);
			String va = isReference ? String() : asVariable(a);
			String na = newTemp("n");
			line(String("Number ") << na << " = " << (isReference ? referenceToNumber(a) : String("(") << va << ")->referenceToNumber()") << ";");
			String nb = newTemp("n");
			line(String("Number ") << nb << " = " << asNumber(b) << ";");
			String result;
			switch (in.asmType) {
			case ParserAsm::OperatorAssignMinus:
				result = na + " - " + nb;
				break;
			case ParserAsm::OperatorAssignMul:
				result = na + " * " + nb;
				break;
			case ParserAsm::OperatorAssignDiv:
				result = na + " / " + nb;
				break;
			case ParserAsm::OperatorAssignMod:
				result = String("fmod(") << na << ", " << nb << ")";
				break;
			case ParserAsm::OperatorAssignBitwiseOr:
				result = String("bitwiseOr(") << na << ", " << nb << ")";
				break;
			case ParserAsm::OperatorAssignBitwiseAnd:
				result = String("bitwiseAnd(") << na << ", " << nb << ")";
				break;
			case ParserAsm::OperatorAssignBitwiseXor:
				result = String("bitwiseXor(") << na << ", " << nb << ")";
				break;
			case ParserAsm::OperatorAssignBitwiseLeftShift:
				result = String("bitwiseLeftShift(") << na << ", " << nb << ")";
				break;
			case ParserAsm::OperatorAssignBitwiseRightShift:
				result = String("bitwiseRightShift(") << na << ", " << nb << ")";
				break;
			default:
				result = String("bitwiseRightShiftZero(") << na << ", " << nb << ")";
				break;
			};
			if (isReference) {
				pushVariable(String("VariableNumber::newVariable(") << result << ")");
				referenceSet(a, stack.back().name + ".value()");
				return true;
			};
			pushVariable(String("(") << va << ")->referenceSet(VariableNumber::newVariable(" << result << "))");
			return true;
		};
		case ParserAsm::OperatorPlusPlusLeft:
		case ParserAsm::OperatorMinusMinusLeft: {
			CompileCppValue a = pop();
			const char *operatorCpp = (in.asmType == ParserAsm::OperatorPlusPlusLeft) ? "+" : "-";
			if (a.type == CompileCppValue::TypeReference || a.type == CompileCppValue::TypeStatic) {
				pushVariable(String("VariableNumber::newVariable((") << referenceToNumber(a) << ") " << operatorCpp << " 1)");
				referenceSet(a, stack.back().name + ".value()");
				return true;
			};
			String va = asVariable(a);
			pushVariable(String("(") << va << ")->referenceSet(VariableNumber::newVariable((" << va << ")->referenceToNumber() " << operatorCpp << " 1))");
			return true;
		};
		case ParserAsm::OperatorPlusPlusRight:
		case ParserAsm::OperatorMinusMinusRight: {
			CompileCppValue a = pop();
			const char *operatorCpp = (in.asmType == ParserAsm::OperatorPlusPlusRight) ? "+" : "-";
			if (a.type == CompileCppValue::TypeReference || a.type == CompileCppValue::TypeStatic) {
				pushVariable(referenceGet(a));
				String value = newTemp("t");
				line(String("TPointer<Variable> ") << value << "(VariableNumber::newVariable((" << stack.back().name << "->toNumber()) " << operatorCpp << " 1));");
				referenceSet(a, value + ".value()");
				return true;
			};
			String va = asVariable(a);
			pushVariable(String("(") << va << ")->referenceGet()");
			line(String("(") << va << ")->referenceSet(VariableNumber::newVariable((" << va << ")->referenceToNumber() " << operatorCpp << " 1));");
			return true;
		};

		// --- optimized instructions of the parser

		case ParserAsm::SymbolPlusPlus: {
			String value = newTemp("t");
			String symbol_ = symbol(operandSymbol(in));
			line(String("TPointer<Variable> ") << value << "(" << global() << "->getPropertyBySymbol(" << symbol_ << "));");
			line(String(global()) << "->setPropertyBySymbol(" << symbol_ << ", VariableNumber::newVariable((" << value << "->toNumber()) + 1));");
			return true;
		};
		case ParserAsm::LocalVariablesPlusPlus: {
			uint32_t index = operandIndex(in);
			if (isCppLocals()) {
				spillLocal(index);
				line(local(index) << " = VariableNumber::newVariable((" << local(index) << "->toNumber()) + 1);");
				return true;
			};
			String value = newTemp("r");
			line(String("TPointerX<Variable> &") << value << " = " << localsArray() << "->index(" << toStringSize(index) << ");");
			line(String(value) << " = VariableNumber::newVariable((" << value << "->toNumber()) + 1);");
			return true;
		};
		case ParserAsm::OperatorMinusArgumentsSymbolXNumber: {
			VariableOperator21 *operand = (VariableOperator21 *)in.operand.value();
			pushNumber(String("(arguments->index(") << toStringSize((uint32_t)operand->symbol) << ")->toNumber()) - " << toStringNumber(operand->value));
			return true;
		};
		case ParserAsm::OperatorMulLocalVariablesSymbol2:
		case ParserAsm::OperatorMinusLocalVariablesSymbol2:
		case ParserAsm::OperatorPlusLocalVariablesSymbol2: {
			VariableOperator22 *operand = (VariableOperator22 *)in.operand.value();
			String operand2 = newTemp("v");
			String operand1 = newTemp("v");
			if (isCppLocals()) {
				line(String("Variable *") << operand2 << " = " << local((uint32_t)operand->symbol1) << ".value();");
				line(String("Variable *") << operand1 << " = " << local((uint32_t)operand->symbol2) << ".value();");
			} else {
				line(String("Variable *") << operand2 << " = " << localsArray() << "->index(" << toStringSize((uint32_t)operand->symbol1) << ").value();");
				line(String("Variable *") << operand1 << " = " << localsArray() << "->index(" << toStringSize((uint32_t)operand->symbol2) << ").value();");
			};
			switch (in.asmType) {
			case ParserAsm::OperatorMulLocalVariablesSymbol2:
				pushNumber(String("(") << operand1 << "->toNumber()) * (" << operand2 << "->toNumber())");
				break;
			case ParserAsm::OperatorMinusLocalVariablesSymbol2:
				pushNumber(String("(") << operand1 << "->toNumber()) - (" << operand2 << "->toNumber())");
				break;
			default:
				pushVariable(String(operand1) << "->operatorPlus(" << operand2 << ")");
				break;
			};
			return true;
		};
		case ParserAsm::OperatorPow2LocalVariablesSymbol: {
			String operandX = newTemp("v");
			uint32_t index = operandIndex(in);
			if (isCppLocals()) {
				line(String("Variable *") << operandX << " = " << local(index) << ".value();");
			} else {
				line(String("Variable *") << operandX << " = " << localsArray() << "->index(" << toStringSize(index) << ").value();");
			};
			pushNumber(String("(") << operandX << "->toNumber()) * (" << operandX << "->toNumber())");
			return true;
		};
		case ParserAsm::AssignLocalVariablesObjectReferencePow2LocalVariablesSymbol: {
			VariableOperator22 *operand = (VariableOperator22 *)in.operand.value();
			String value = newTemp("n");
			if (isCppLocals()) {
				line(String("Number ") << value << " = " << local((uint32_t)operand->symbol2) << "->toNumber();");
				spillLocal((uint32_t)operand->symbol1);
				line(local((uint32_t)operand->symbol1) << " = VariableNumber::newVariable(" << value << " * " << value << ");");
				return true;
			};
			line(String("Number ") << value << " = (" << localsArray() << "->index(" << toStringSize((uint32_t)operand->symbol2) << "))->toNumber();");
			line(String("(") << localsArray() << "->index(" << toStringSize((uint32_t)operand->symbol1) << ")) = VariableNumber::newVariable(" << value << " * " << value << ");");
			return true;
		};
		default:
			break;
		};
		return false;
	};

	// every value is a Variable, the instructions of the native runtime
	bool CompileCppFunction::emitRuntime(CodeInstruction &in) {
		switch (in.asmType) {
		case ParserAsm::PushUndefined:
			pushBorrowed("module->undefined");
			return true;
		case ParserAsm::PushNull:
			pushVariable("VariableNull::newVariable()");
			return true;
		case ParserAsm::PushBoolean:
			pushBorrowed(((VariableBoolean *)in.operand.value())->value ? "module->valueTrue" : "module->valueFalse");
			return true;
		case ParserAsm::PushNumber:
			pushBorrowed(String("module->number[") << toStringSize(constant.numberIndex(((VariableNumber *)in.operand.value())->value)) << "]");
			return true;
		case ParserAsm::PushString:
			pushBorrowed(String("module->string[") << toStringSize(constant.stringIndex(((VariableString *)in.operand.value())->value)) << "]");
			return true;
		case ParserAsm::PushSymbol:
			pushVariable(String(global()) << "->getPropertyBySymbol(" << symbol(operandSymbol(in)) << ")");
			return true;
		case ParserAsm::PushGlobal:
			pushBorrowed(global());
			return true;
		case ParserAsm::ContextPushThis:
			pushBorrowed("this_");
			return true;
		case ParserAsm::ContextPushSelf:
			pushBorrowed("function");
			return true;
		case ParserAsm::PushArguments:
			pushBorrowed("arguments");
			return true;
		case ParserAsm::PushNewObject:
			pushVariable("VariableObject::newVariable()");
			return true;
		case ParserAsm::PushNewArray:
		case ParserAsm::XPushNewArray:
			pushVariable("VariableArray::newVariable()");
			return true;
		case ParserAsm::LocalVariablesPushSymbol:
			if (isCppLocals()) {
				pushVariable(local(operandIndex(in)));
				return true;
			};
			pushVariable(String(localsArray()) << "->index(" << toStringSize(operandIndex(in)) << ")");
			return true;
		case ParserAsm::ArgumentsPushSymbol:
			pushVariable(String("arguments->index(") << toStringSize(operandIndex(in)) << ")");
			return true;
		case ParserAsm::LocalVariablesLevelPushSymbol:
		case ParserAsm::ArgumentsLevelPushSymbol: {
			VariableArgumentLevel *operand = (VariableArgumentLevel *)in.operand.value();
			pushVariable(String((in.asmType == ParserAsm::LocalVariablesLevelPushSymbol) ? "localVariablesLevelGet" : "argumentsLevelGet") << "(function->functionParent.value(), " << toStringInt(operand->level) << ", " << toStringInt(operand->value) << ")");
			return true;
		};
		case ParserAsm::PushObjectReference:
			pushStatic(CompileCppEntry::ReferenceGlobal, (uint32_t)constant.symbolIndex(Context::getSymbolMirror(operandSymbol(in))));
			useSymbol = true;
			return true;
		case ParserAsm::LocalVariablesPushObjectReference:
			pushStatic(CompileCppEntry::ReferenceLocal, operandIndex(in));
			return true;
		case ParserAsm::ArgumentsPushObjectReference:
			pushStatic(CompileCppEntry::ReferenceArgument, operandIndex(in));
			return true;
		case ParserAsm::LocalVariablesLevelPushObjectReference:
		case ParserAsm::ArgumentsLevelPushObjectReference: {
			VariableArgumentLevel *operand = (VariableArgumentLevel *)in.operand.value();
			pushVariable(String("VariableReferenceIndex::newVariable(") << ((in.asmType == ParserAsm::LocalVariablesLevelPushObjectReference) ? "localVariablesLevelReference" : "argumentsLevelReference") << "(function->functionParent.value(), " << toStringInt(operand->level) << "), " << toStringInt(operand->value) << ")");
			return true;
		};
		case ParserAsm::Reference: {
			CompileCppValue a = pop();
			String va = asVariable(a);
			pushVariable(String("(") << va << ")->getPropertyBySymbol(" << symbol(operandSymbol(in)) << ")");
			return true;
		};
		case ParserAsm::ReferenceObjectReference: {
			CompileCppValue a = pop();
			String va = asVariable(a);
			pushVariable(String("VariableReferenceSymbol::newVariable(") << va << ", " << symbol(operandSymbol(in)) << ")");
			return true;
		};
		case ParserAsm::OperatorReference: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String va = asVariable(a);
			String vb = asVariable(b);
			pushVariable(String("(") << va << ")->getPropertyByVariable(" << vb << ")");
			return true;
		};
		case ParserAsm::OperatorReferenceReference: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String va = asVariable(a);
			String vb = asVariable(b);
			pushVariable(String("VariableReferenceVariable::newVariable(") << va << ", " << vb << ")");
			return true;
		};
		case ParserAsm::OperatorReferenceDeleteIndex: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String va = asVariable(a);
			String vb = asVariable(b);
			String name = newTemp("c");
			line(String("Variable *") << name << " = boolean(module, (" << va << ")->deletePropertyByVariable(" << vb << "));");
			pushBorrowed(name);
			return true;
		};
		case ParserAsm::OperatorReferenceDeleteReference: {
			CompileCppValue a = pop();
			String va = asVariable(a);
			String name = newTemp("c");
			line(String("Variable *") << name << " = boolean(module, (" << va << ")->deletePropertyBySymbol(" << symbol(operandSymbol(in)) << "));");
			pushBorrowed(name);
			return true;
		};
		case ParserAsm::OperatorTypeOf: {
			CompileCppValue a = pop();
			String va = asVariable(a);
			pushVariable(String("operatorTypeOf(") << va << ")");
			return true;
		};
		case ParserAsm::OperatorInstanceOf:
		case ParserAsm::OperatorIn:
		case ParserAsm::OperatorEqual:
		case ParserAsm::OperatorNotEqual:
		case ParserAsm::OperatorEqualStrict:
		case ParserAsm::OperatorNotEqualStrict:
		case ParserAsm::OperatorLess:
		case ParserAsm::OperatorLessOrEqual:
		case ParserAsm::OperatorGreater:
		case ParserAsm::OperatorGreaterOrEqual:
		case ParserAsm::OperatorLogicalOr:
		case ParserAsm::OperatorLogicalAnd: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String va = asVariable(a);
			String vb = asVariable(b);
			const char *procedure = "";
			switch (in.asmType) {
			case ParserAsm::OperatorInstanceOf:
				procedure = "operatorInstanceOf";
				break;
			case ParserAsm::OperatorIn:
				procedure = "operatorIn";
				break;
			case ParserAsm::OperatorEqual:
				procedure = "operatorEqual";
				break;
			case ParserAsm::OperatorNotEqual:
				procedure = "operatorNotEqual";
				break;
			case ParserAsm::OperatorEqualStrict:
				procedure = "operatorEqualStrict";
				break;
			case ParserAsm::OperatorNotEqualStrict:
				procedure = "operatorNotEqualStrict";
				break;
			case ParserAsm::OperatorLess:
				procedure = "operatorLess";
				break;
			case ParserAsm::OperatorLessOrEqual:
				procedure = "operatorLessOrEqual";
				break;
			case ParserAsm::OperatorGreater:
				procedure = "operatorGreater";
				break;
			case ParserAsm::OperatorGreaterOrEqual:
				procedure = "operatorGreaterOrEqual";
				break;
			case ParserAsm::OperatorLogicalOr:
				procedure = "operatorLogicalOr";
				break;
			default:
				procedure = "operatorLogicalAnd";
				break;
			};
			String name = newTemp("c");
			line(String("Variable *") << name << " = " << procedure << "(module, " << va << ", " << vb << ");");
			pushBorrowed(name);
			return true;
		};
		case ParserAsm::OperatorNot: {
			CompileCppValue a = pop();
			String va = asVariable(a);
			String name = newTemp("c");
			line(String("Variable *") << name << " = operatorNot(module, " << va << ");");
			pushBorrowed(name);
			return true;
		};
		case ParserAsm::OperatorPlus:
		case ParserAsm::OperatorMinus:
		case ParserAsm::OperatorMul:
		case ParserAsm::OperatorDiv:
		case ParserAsm::OperatorMod:
		case ParserAsm::OperatorBitwiseOr:
		case ParserAsm::OperatorBitwiseAnd:
		case ParserAsm::OperatorBitwiseXor:
		case ParserAsm::OperatorBitwiseLeftShift:
		case ParserAsm::OperatorBitwiseRightShift:
		case ParserAsm::OperatorBitwiseRightShiftZero: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String va = asVariable(a);
			String vb = asVariable(b);
			const char *procedure = "";
			switch (in.asmType) {
			case ParserAsm::OperatorPlus:
				procedure = "operatorPlus";
				break;
			case ParserAsm::OperatorMinus:
				procedure = "operatorMinus";
				break;
			case ParserAsm::OperatorMul:
				procedure = "operatorMul";
				break;
			case ParserAsm::OperatorDiv:
				procedure = "operatorDiv";
				break;
			case ParserAsm::OperatorMod:
				procedure = "operatorMod";
				break;
			case ParserAsm::OperatorBitwiseOr:
				procedure = "operatorBitwiseOr";
				break;
			case ParserAsm::OperatorBitwiseAnd:
				procedure = "operatorBitwiseAnd";
				break;
			case ParserAsm::OperatorBitwiseXor:
				procedure = "operatorBitwiseXor";
				break;
			case ParserAsm::OperatorBitwiseLeftShift:
				procedure = "operatorBitwiseLeftShift";
				break;
			case ParserAsm::OperatorBitwiseRightShift:
				procedure = "operatorBitwiseRightShift";
				break;
			default:
				procedure = "operatorBitwiseRightShiftZero";
				break;
			};
			pushVariable(String(procedure) << "(" << va << ", " << vb << ")");
			return true;
		};
		case ParserAsm::OperatorUnaryPlus:
		case ParserAsm::OperatorBitwiseNot: {
			CompileCppValue a = pop();
			String va = asVariable(a);
			pushVariable(String((in.asmType == ParserAsm::OperatorUnaryPlus) ? "operatorUnaryPlus" : "operatorBitwiseNot") << "(" << va << ")");
			return true;
		};
		case ParserAsm::OperatorEqual1: {
			CompileCppValue b = pop();
			String vb = asVariable(b);
			String va = asVariable(stack.back());
			String name = newTemp("c");
			line(String("Variable *") << name << " = operatorEqual(module, " << va << ", " << vb << ");");
			pushBorrowed(name);
			return true;
		};

		// --- stack

		case ParserAsm::Pop1:
			stack.pop_back();
			return true;
		case ParserAsm::Duplicate: {
			CompileCppValue top = stack.back();
			if (top.type == CompileCppValue::TypeSlot) {
				top.type = CompileCppValue::TypeBorrowed;
				top.name = slot(top.index) + ".value()";
			};
			stack.push_back(top);
			return true;
		};

		// --- assign, the reference is a Variable

		case ParserAsm::Assign: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String vb = asVariable(b);
			String va = asVariable(a);
			line(String("(") << va << ")->referenceSetA1(" << vb << ");");
			return true;
		};
		case ParserAsm::AssignReverse: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String va = asVariable(a);
			String vb = asVariable(b);
			line(String("(") << vb << ")->referenceSetA1(" << va << ");");
			return true;
		};
		case ParserAsm::OperatorAssign: {
			CompileCppValue value = pop();
			CompileCppValue reference = pop();
			String vReference = asVariable(reference);
			String name = newTemp("t");
			line(String("TPointer<Variable> ") << name << "(" << asVariable(value) << ");");
			line(String("(") << vReference << ")->referenceSetA1(" << name << ");");
			CompileCppValue item;
			item.type = CompileCppValue::TypeVariable;
			item.name = name;
			stack.push_back(item);
			return true;
		};
		case ParserAsm::OperatorAssignPlus:
		case ParserAsm::OperatorAssignMinus:
		case ParserAsm::OperatorAssignMul:
		case ParserAsm::OperatorAssignDiv:
		case ParserAsm::OperatorAssignMod:
		case ParserAsm::OperatorAssignBitwiseOr:
		case ParserAsm::OperatorAssignBitwiseAnd:
		case ParserAsm::OperatorAssignBitwiseXor:
		case ParserAsm::OperatorAssignBitwiseLeftShift:
		case ParserAsm::OperatorAssignBitwiseRightShift:
		case ParserAsm::OperatorAssignBitwiseRightShiftZero: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String va = asVariable(a);
			String vb = asVariable(b);
			const char *procedure = "";
			switch (in.asmType) {
			case ParserAsm::OperatorAssignPlus:
				procedure = "assignPlus";
				break;
			case ParserAsm::OperatorAssignMinus:
				procedure = "assignMinus";
				break;
			case ParserAsm::OperatorAssignMul:
				procedure = "assignMul";
				break;
			case ParserAsm::OperatorAssignDiv:
				procedure = "assignDiv";
				break;
			case ParserAsm::OperatorAssignMod:
				procedure = "assignMod";
				break;
			case ParserAsm::OperatorAssignBitwiseOr:
				procedure = "assignBitwiseOr";
				break;
			case ParserAsm::OperatorAssignBitwiseAnd:
				procedure = "assignBitwiseAnd";
				break;
			case ParserAsm::OperatorAssignBitwiseXor:
				procedure = "assignBitwiseXor";
				break;
			case ParserAsm::OperatorAssignBitwiseLeftShift:
				procedure = "assignBitwiseLeftShift";
				break;
			case ParserAsm::OperatorAssignBitwiseRightShift:
				procedure = "assignBitwiseRightShift";
				break;
			default:
				procedure = "assignBitwiseRightShiftZero";
				break;
			};
			pushVariable(String(procedure) << "(" << va << ", " << vb << ")");
			return true;
		};
		case ParserAsm::OperatorPlusPlusLeft:
		case ParserAsm::OperatorPlusPlusRight:
		case ParserAsm::OperatorMinusMinusLeft:
		case ParserAsm::OperatorMinusMinusRight: {
			CompileCppValue a = pop();
			String va = asVariable(a);
			const char *procedure = "plusPlusLeft";
			switch (in.asmType) {
			case ParserAsm::OperatorPlusPlusRight:
				procedure = "plusPlusRight";
				break;
			case ParserAsm::OperatorMinusMinusLeft:
				procedure = "minusMinusLeft";
				break;
			case ParserAsm::OperatorMinusMinusRight:
				procedure = "minusMinusRight";
				break;
			default:
				break;
			};
			pushVariable(String(procedure) << "(" << va << ")");
			return true;
		};

		// --- arrays, objects

		case ParserAsm::ArrayPush:
		case ParserAsm::XArrayPushWithTransfer: {
			CompileCppValue b = pop();
			String vb = asVariable(b);
			String va = asVariable(stack.back());
			String array = newTemp("a");
			line(String("VariableArray *") << array << " = (VariableArray *)" << va << ";");
			line(String("(*(") << array << "->value))[" << array << "->value->length()] = " << vb << ";");
			return true;
		};
		case ParserAsm::OperatorArrayPush: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String vb = asVariable(b);
			String va = asVariable(a);
			String array = newTemp("a");
			line(String("VariableArray *") << array << " = (VariableArray *)" << va << ";");
			line(String("(*(") << array << "->value))[" << array << "->value->length()] = " << vb << ";");
			pushVariable(vb);
			return true;
		};
		case ParserAsm::OperatorAssignXPrototype: {
			CompileCppValue b = pop();
			String vb = asVariable(b);
			String va = asVariable(stack.back());
			line(String("assignXPrototype(") << va << ", " << vb << ");");
			pushVariable(vb);
			return true;
		};
		case ParserAsm::AssignNewObject: {
			CompileCppValue b = pop();
			CompileCppValue a = pop();
			String vb = asVariable(b);
			String va = asVariable(a);
			pushVariable(String("assignNewObject(") << va << ", " << vb << ")");
			return true;
		};

		// --- optimized instructions of the parser

		case ParserAsm::SymbolPlusPlus:
		case ParserAsm::LocalVariablesPlusPlus:
		case ParserAsm::OperatorMinusArgumentsSymbolXNumber:
		case ParserAsm::OperatorMulLocalVariablesSymbol2:
		case ParserAsm::OperatorMinusLocalVariablesSymbol2:
		case ParserAsm::OperatorPlusLocalVariablesSymbol2:
		case ParserAsm::OperatorPow2LocalVariablesSymbol:
		case ParserAsm::AssignLocalVariablesObjectReferencePow2LocalVariablesSymbol:
			// heap local variables in this mode, the inline code is the runtime code
			return emitInline(in);

		default:
			break;
		};
		return false;
	};

	bool CompileCppFunction::generate(String &output) {
		size_t k;
		out = "";
		stack.clear();
		hasSource = false;
		for (AsmLabel label : region.code) {
			if (unit.at(label).sourceSymbol && unit.at(label).asmType != ParserAsm::XPushFunction) {
				hasSource = true;
				break;
			};
		};

		// --- body

		bool previousFallThrough = false;
		AsmLabel previous = 0;
		for (AsmLabel label : region.code) {
			if (!state[label].isSet) {
				// not reachable
				if (previousFallThrough) {
					return fail(previous, "Unsupported code, no next instruction");
				};
				continue;
			};
			if (blockStart[label] || !blockOpen) {
				if (previousFallThrough) {
					if (!transfer(stack, state[label])) {
						return false;
					};
				};
				closeBlock();
				openBlock(label);
				setStack(state[label]);
				setTryState(state[label]);
			};
			CodeInstruction &in = unit.at(label);
			line(String("// ") << toStringSize(label) << " " << AsmOpcode::getInfo(in.asmType)->name);
			bool fallThrough = true;
			if (!emitInstruction(label, fallThrough)) {
				if (error.length() == 0) {
					fail(label, "Unsupported instruction");
				};
				return false;
			};
			if (needHeapLocals && !region.heapLocals) {
				error = "Local variables";
				return false;
			};
			if (fallThrough && !next(label)) {
				if (regionIndex == 0 && (size_t)label == unit.length()) {
					// the end of the top level code
					line(returnValue("module->undefined"));
					fallThrough = false;
				} else {
					return fail(label, "Unsupported code, no next instruction");
				};
			};
			previousFallThrough = fallThrough;
			previous = label;
		};
		if (previousFallThrough) {
			return fail(previous, "Unsupported code, no next instruction");
		};
		closeBlock();

		// handlers of the try, the exception value to the catch / finally
		String handlers;
		for (k = 0; k < handlerWalk.size(); ++k) {
			CompileCppWalk &walk = handlerWalk[k];
			out << "\t\t\t\tH" << toStringSize(k + 1) << ":;\n";
			out << "\t\t\t\t{\n";
			// the stack of the try is empty at the handler: values from slots
			const CompileCppState &target = state[walk.target];
			if (target.stack.size() != ((walk.kind == CompileCppWalk::Catch) ? 1 : 0)) {
				error = "Unsupported try, operand stack";
				return false;
			};
			if (walk.kind == CompileCppWalk::Catch) {
				line(slot(target.stack.size() - 1) << " = exceptionValue;");
			} else {
				line(contextVariable(usePending, "pd", walk.contextIndex) << " = exceptionValue;");
				line(contextVariable(usePc, "pc", walk.contextIndex) << " = " << toStringSize(walk.pc) << ";");
			};
			line("exceptionValue = nullptr;");
			line(String("goto L") << toStringSize(walk.target) << ";");
			out << "\t\t\t\t};\n";
		};

		if (needHeapLocals && !region.heapLocals) {
			error = "Local variables";
			return false;
		};

		// --- function

		bool hasHandlers = !handlerWalk.empty();
		bool isCoroutine = region.isCoroutine;
		bool needsDispatch = hasHandlers || isCoroutine;
		bool needsCatch = hasHandlers || hasSource || isCoroutine;
		size_t maxDepth = 0;
		for (k = 0; k < useRegister.size(); ++k) {
			maxDepth = std::max(maxDepth, k + 1);
		};

		if (isCoroutine) {
			output << "\tstatic TPointer<Variable> " << region.name << "(VariableFunctionWithYield *function, Variable *this_, VariableArray *arguments) {\n";
		} else {
			output << "\tstatic TPointer<Variable> " << region.name << "(VariableFunction *function, Variable *this_, VariableArray *arguments) {\n";
		};
		output << "\t\tModule *module = (Module *)function->valueSuper;\n";
		if (useGlobal) {
			output << "\t\tVariable *global = module->global;\n";
		};
		if (useSymbol) {
			output << "\t\tconst Symbol *S = module->symbol;\n";
		};
		output << "\t\tTPointer<VariableArray> arguments_;\n";
		output << "\t\tif (!arguments) {\n";
		output << "\t\t\targuments_ = VariableArray::newArray();\n";
		output << "\t\t\targuments = arguments_;\n";
		output << "\t\t};\n";
		output << "\t\tif (!this_) {\n";
		output << "\t\t\tthis_ = module->undefined;\n";
		output << "\t\t};\n";

		// variables saved by a coroutine
		std::vector<String> saveVariable;
		std::vector<String> saveIterator;
		std::vector<String> saveValue;

		if (region.heapLocals) {
			output << "\t\tTPointer<VariableArray> locals;\n";
			saveVariable.push_back("locals");
		} else {
			for (k = 0; k < useLocal.size(); ++k) {
				if (useLocal[k]) {
					output << "\t\tTPointer<Variable> l" << toStringSize(k) << "(module->undefined);\n";
					saveVariable.push_back(String("l") << toStringSize(k));
				};
			};
		};
		for (k = 0; k < useSlot.size(); ++k) {
			if (useSlot[k]) {
				output << "\t\tTPointer<Variable> s" << toStringSize(k) << ";\n";
				saveVariable.push_back(String("s") << toStringSize(k));
			};
		};
		for (k = 0; k < useRegister.size(); ++k) {
			if (useRegister[k]) {
				output << "\t\tTPointer<Variable> rv" << toStringSize(k) << ";\n";
				saveVariable.push_back(String("rv") << toStringSize(k));
			};
		};
		for (k = 0; k < useReference.size(); ++k) {
			if (useReference[k]) {
				output << "\t\tTPointer<Variable> rf" << toStringSize(k) << ";\n";
				saveVariable.push_back(String("rf") << toStringSize(k));
			};
		};
		for (k = 0; k < usePending.size(); ++k) {
			if (usePending[k]) {
				output << "\t\tTPointer<Variable> pd" << toStringSize(k) << ";\n";
				saveVariable.push_back(String("pd") << toStringSize(k));
			};
		};
		for (k = 0; k < useIterator.size(); ++k) {
			if (useIterator[k]) {
				output << "\t\tTPointer<Iterator> it" << toStringSize(k) << ";\n";
				saveIterator.push_back(String("it") << toStringSize(k));
			};
		};
		for (k = 0; k < usePc.size(); ++k) {
			if (usePc[k]) {
				output << "\t\tint pc" << toStringSize(k) << " = 0;\n";
				saveValue.push_back(String("pc") << toStringSize(k));
			};
		};
		if (needsDispatch) {
			output << "\t\tint tryState = 0;\n";
			saveValue.push_back("tryState");
		};
		if (hasHandlers || useExceptionTrace) {
			output << "\t\tTPointer<Variable> exceptionValue;\n";
			output << "\t\tTPointer<StackTrace> exceptionTrace;\n";
		};
		if (hasSource) {
			output << "\t\tsize_t source = 0;\n";
			output << "\t\tuint32_t line = 0;\n";
		};
		if (useSelfTailCall) {
			output << "\t\tTPointer<Variable> thisValue;\n";
			output << "\t\tauto selfTailCall = [&]() {\n";
			if (region.heapLocals) {
				output << "\t\t\tlocals = VariableArray::newArray();\n";
			} else {
				for (k = 0; k < useLocal.size(); ++k) {
					if (useLocal[k]) {
						output << "\t\t\tl" << toStringSize(k) << " = module->undefined;\n";
					};
				};
			};
			output << "\t\t};\n";
		};

		if (isCoroutine) {
			output << "\t\tCoroutineFrame *frame = getCoroutineFrame(function);\n";
			output << "\t\tauto yieldSave = [&]() {\n";
			for (k = 0; k < saveVariable.size(); ++k) {
				output << "\t\t\tframe->variable->index(" << toStringSize(k) << ") = " << saveVariable[k] << ".value();\n";
			};
			for (k = 0; k < saveIterator.size(); ++k) {
				output << "\t\t\tframe->iterator[" << toStringSize(k) << "] = " << saveIterator[k] << ";\n";
			};
			for (k = 0; k < saveValue.size(); ++k) {
				output << "\t\t\tframe->value[" << toStringSize(k) << "] = " << saveValue[k] << ";\n";
			};
			output << "\t\t};\n";
		};
		if (needsDispatch) {
			output << "\t\tint entry = 0;\n";
		};
		if (isCoroutine) {
			output << "\t\tif (function->yieldStep) {\n";
			output << "\t\t\tentry = (int)function->yieldStep;\n";
			for (k = 0; k < saveVariable.size(); ++k) {
				if (saveVariable[k] == "locals") {
					output << "\t\t\tlocals = (VariableArray *)(frame->variable->index(" << toStringSize(k) << ").value());\n";
					continue;
				};
				output << "\t\t\t" << saveVariable[k] << " = frame->variable->index(" << toStringSize(k) << ");\n";
			};
			for (k = 0; k < saveIterator.size(); ++k) {
				output << "\t\t\t" << saveIterator[k] << " = frame->iterator[" << toStringSize(k) << "];\n";
			};
			for (k = 0; k < saveValue.size(); ++k) {
				output << "\t\t\t" << saveValue[k] << " = frame->value[" << toStringSize(k) << "];\n";
			};
			if (region.heapLocals) {
				output << "\t\t} else {\n";
				output << "\t\t\tlocals = VariableArray::newArray();\n";
			};
			output << "\t\t};\n";
		} else if (region.heapLocals) {
			output << "\t\tlocals = VariableArray::newArray();\n";
		};
		if (region.isCoroutine && saveIterator.size() > region.contextCount) {
			error = "Coroutine iterators";
			return false;
		};
		if (region.isCoroutine && saveValue.size() > region.contextCount + 1) {
			error = "Coroutine values";
			return false;
		};

		if (needsDispatch) {
			output << "\t\tfor (;;) {\n";
		} else {
			output << "\t\t{\n";
		};
		if (needsCatch) {
			output << "\t\t\ttry {\n";
		} else {
			output << "\t\t\t{\n";
		};
		if (needsDispatch) {
			output << "\t\t\t\tswitch (entry) {\n";
			output << "\t\t\t\t\tcase 0:\n";
			output << "\t\t\t\t\t\tgoto L" << toStringSize(region.code[0]) << ";\n";
			for (k = 0; k < handlerWalk.size(); ++k) {
				output << "\t\t\t\t\tcase " << toStringSize(k + 1) << ":\n";
				output << "\t\t\t\t\t\tgoto H" << toStringSize(k + 1) << ";\n";
			};
			for (k = 0; k < resume.size(); ++k) {
				output << "\t\t\t\t\tcase " << toStringSize(handlerWalk.size() + k + 1) << ":\n";
				output << "\t\t\t\t\t\tgoto L" << toStringSize(resume[k]) << ";\n";
			};
			output << "\t\t\t\t\tdefault:\n";
			output << "\t\t\t\t\t\tbreak;\n";
			output << "\t\t\t\t};\n";
			output << "\t\t\t\tthrow Error(\"Invalid entry\");\n";
		};
		output << out;
		if (needsCatch) {
			String reset;
			if (isCoroutine) {
				reset = "\t\t\t\tcoroutineReset(function);\n";
			};
			output << "\t\t\t} catch (Exception &e) {\n";
			if (hasSource) {
				output << "\t\t\t\te.trace(module->source[source], line);\n";
			};
			if (hasHandlers) {
				output << "\t\t\t\tif (!tryState) {\n";
				output << "\t" << reset;
				output << "\t\t\t\t\tthrow;\n";
				output << "\t\t\t\t};\n";
				output << "\t\t\t\texceptionValue = e.value;\n";
				output << "\t\t\t\texceptionTrace = e.stackTrace;\n";
			} else {
				output << reset;
				output << "\t\t\t\tthrow;\n";
			};
			if (hasSource || hasHandlers) {
				output << "\t\t\t} catch (Error &e) {\n";
				output << "\t\t\t\tException exception(newException(module, e));\n";
				if (hasSource) {
					output << "\t\t\t\texception.trace(module->source[source], line);\n";
				};
				if (hasHandlers) {
					output << "\t\t\t\tif (!tryState) {\n";
					output << "\t" << reset;
					output << "\t\t\t\t\tthrow exception;\n";
					output << "\t\t\t\t};\n";
					output << "\t\t\t\texceptionValue = exception.value;\n";
					output << "\t\t\t\texceptionTrace = exception.stackTrace;\n";
				} else {
					output << reset;
					output << "\t\t\t\tthrow exception;\n";
				};
			};
			if (isCoroutine) {
				output << "\t\t\t} catch (...) {\n";
				output << reset;
				output << "\t\t\t\tthrow;\n";
			};
			output << "\t\t\t};\n";
		} else {
			output << "\t\t\t};\n";
		};
		if (needsDispatch) {
			output << "\t\t\tentry = tryState;\n";
		};
		output << "\t\t};\n";
		if (!needsDispatch) {
			output << "\t\treturn module->undefined;\n";
		};
		output << "\t};\n\n";
		return true;
	};

	bool CompileCpp::generate(const uint8_t *bytecode, size_t size, const String &sourceName, const String &namespaceName, bool withInline, String &output, String &error) {
		CodeUnit unit;
		CodeUnitMap units;
		CompileCppConstant constant;
		size_t k;

		if (!Bytecode::load(bytecode, size, unit, error, &units)) {
			return false;
		};

		// the code of each unit
		std::vector<CompileCppUnit *> compileUnit;
		std::vector<String> unitName;
		std::vector<String> code;
		bool isOk = true;
		compileUnit.push_back(new CompileCppUnit(unit, constant, "u0", withInline));
		unitName.push_back("");
		CodeUnitMap::Node *scan;
		for (scan = units.begin(); scan; scan = scan->successor()) {
			String prefix("u");
			prefix << toStringSize(compileUnit.size());
			compileUnit.push_back(new CompileCppUnit(*(scan->value.value()), constant, prefix, withInline));
			unitName.push_back(scan->key);
		};
		code.resize(compileUnit.size());
		for (k = 0; k < compileUnit.size(); ++k) {
			if (!compileUnit[k]->generate(code[k])) {
				error = compileUnit[k]->error;
				if (k > 0) {
					error << " in unit " << unitName[k];
				};
				isOk = false;
				break;
			};
		};

		if (isOk) {
			output = "";
			output << "// Generated by quantum-script --compile-cpp from " << sourceName << "\n";
			output << "// Link with quantum-script.native-runtime, quantum-script and quantum-script--console\n\n";
			output << "#include <math.h>\n"
			          "#include <stdio.h>\n\n"
			          "#include <XYO/QuantumScript.NativeRuntime.hpp>\n\n";
			output << "#if XYO_QUANTUMSCRIPT_NATIVERUNTIME_ABI != " << toStringInt(XYO_QUANTUMSCRIPT_COMPILECPP_ABI) << "\n"
			       << "#	error \"Generated for another native runtime, generate it again with quantum-script --compile-cpp\"\n"
			       << "#endif\n\n";
			output << "namespace " << namespaceName << " {\n\n";
			output << "\tusing namespace XYO::QuantumScript;\n";
			output << "\tusing namespace XYO::QuantumScript::NativeRuntime;\n\n";

			for (k = 0; k < compileUnit.size(); ++k) {
				compileUnit[k]->declare(output);
			};
			output << "\n";
			for (k = 0; k < compileUnit.size(); ++k) {
				output << code[k];
			};

			// constants
			if (constant.symbol.empty()) {
				output << "\tstatic const char *const *symbolList = nullptr;\n\n";
			} else {
				output << "\tstatic const char *const symbolList[] = {\n";
				for (k = 0; k < constant.symbol.size(); ++k) {
					output << "\t    " << toStringC(constant.symbol[k]) << ",\n";
				};
				output << "\t};\n\n";
			};
			if (constant.string.empty()) {
				output << "\tstatic const ModuleString *stringList = nullptr;\n\n";
			} else {
				output << "\tstatic const ModuleString stringList[] = {\n";
				for (k = 0; k < constant.string.size(); ++k) {
					output << "\t    {" << toStringC(constant.string[k]) << ", " << toStringSize(constant.string[k].length()) << "},\n";
				};
				output << "\t};\n\n";
			};
			if (constant.number.empty()) {
				output << "\tstatic const Number *numberList = nullptr;\n\n";
			} else {
				output << "\tstatic const Number numberList[] = {\n";
				for (k = 0; k < constant.number.size(); ++k) {
					output << "\t    " << toStringNumber(constant.number[k]) << ",\n";
				};
				output << "\t};\n\n";
			};
			if (constant.source.empty()) {
				output << "\tstatic const char *const *sourceList = nullptr;\n\n";
			} else {
				output << "\tstatic const char *const sourceList[] = {\n";
				for (k = 0; k < constant.source.size(); ++k) {
					output << "\t    " << toStringC(constant.source[k]) << ",\n";
				};
				output << "\t};\n\n";
			};
			output << "\tstatic const ModuleUnit unitList[] = {\n";
			for (k = 0; k < compileUnit.size(); ++k) {
				output << "\t    {" << ((k == 0) ? String("nullptr") : toStringC(unitName[k])) << ", " << compileUnit[k]->regions[0].name << "},\n";
			};
			output << "\t};\n\n";
			output << "\tstatic const ModuleInfo moduleInfo = {\n"
			          "\t    XYO_QUANTUMSCRIPT_NATIVERUNTIME_ABI,\n"
			       << "\t    symbolList, " << toStringSize(constant.symbol.size()) << ",\n"
			       << "\t    stringList, " << toStringSize(constant.string.size()) << ",\n"
			       << "\t    numberList, " << toStringSize(constant.number.size()) << ",\n"
			       << "\t    sourceList, " << toStringSize(constant.source.size()) << ",\n"
			       << "\t    unitList, " << toStringSize(compileUnit.size()) << "};\n\n";

			output << "\t// the description of the compiled code, for NativeRuntime::load / execute / run\n"
			          "\tconst ModuleInfo &getModuleInfo() {\n"
			          "\t\treturn moduleInfo;\n"
			          "\t};\n\n";
			output << "\t// init the engine, run, end the engine, returns the exit code\n"
			          "\tint run(int cmdN, char *cmdS[], QuantumScriptInitExecutiveProc initExecutive) {\n"
			          "\t\treturn NativeRuntime::run(moduleInfo, cmdN, cmdS, initExecutive);\n"
			          "\t};\n\n";
			output << "};\n\n";

			// the application, as the interpreter
			output << "#ifndef XYO_QUANTUMSCRIPT_COMPILED_NO_MAIN\n\n"
			          "#	include <XYO/QuantumScript.Extension/Console.hpp>\n\n"
			          "namespace "
			       << namespaceName << " {\n\n"
			                           "\tstatic void initExecutive(Executive *executive) {\n"
			                           "\t\tExtension::Console::registerInternalExtension(executive);\n"
			                           "\t};\n\n"
			                           "\tclass Application : public virtual IApplication {\n"
			                           "\t\t\tXYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Application);\n\n"
			                           "\t\tpublic:\n"
			                           "\t\t\tinline Application(){};\n\n"
			                           "\t\t\tinline int main(int cmdN, char *cmdS[]) {\n"
			                           "\t\t\t\treturn run(cmdN, cmdS, initExecutive);\n"
			                           "\t\t\t};\n"
			                           "\t};\n\n"
			                           "};\n\n"
			                           "XYO_APPLICATION_MAIN("
			       << namespaceName << "::Application);\n\n"
			                           "#endif\n";
		};

		for (k = 0; k < compileUnit.size(); ++k) {
			delete compileUnit[k];
		};
		return isOk;
	};

};
