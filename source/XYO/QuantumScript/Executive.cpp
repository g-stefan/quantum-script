// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>

#include <stdexcept>

#include <XYO/QuantumScript/Executive.hpp>
#include <XYO/QuantumScript/ExecutiveX.hpp>
#include <XYO/QuantumScript/ParserAsm.hpp>
#include <XYO/QuantumScript/LibStd.hpp>
#include <XYO/QuantumScript/Variable.hpp>
#include <XYO/QuantumScript/ExecutiveContext.hpp>
#include <XYO/QuantumScript/InstructionX.hpp>

#include <XYO/QuantumScript/VariableNull.hpp>
#include <XYO/QuantumScript/VariableBoolean.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>
#include <XYO/QuantumScript/VariableString.hpp>

#include <XYO/QuantumScript/VariableSymbol.hpp>
#include <XYO/QuantumScript/VariableAssociativeArray.hpp>
#include <XYO/QuantumScript/VariableResource.hpp>

#include <XYO/QuantumScript/VariableVmFunction.hpp>
#include <XYO/QuantumScript/InstructionContext.hpp>
#include <XYO/QuantumScript/VariableStackTrace.hpp>
#include <XYO/QuantumScript/VariableArgumentLevel.hpp>
#include <XYO/QuantumScript/VariableReferenceSymbol.hpp>
#include <XYO/QuantumScript/VariableReferenceIndex.hpp>
#include <XYO/QuantumScript/VariableReferenceVariable.hpp>
#include <XYO/QuantumScript/VariableOperator21.hpp>
#include <XYO/QuantumScript/VariableOperator22.hpp>
#include <XYO/QuantumScript/VariableOperator23.hpp>
#include <XYO/QuantumScript/VariableOperator31.hpp>
#include <XYO/QuantumScript/VariableNativeVmFunction.hpp>
#include <XYO/QuantumScript/VariableVmProgramCounter.hpp>

#include <XYO/QuantumScript/ArrayIteratorKey.hpp>
#include <XYO/QuantumScript/ArrayIteratorValue.hpp>
#include <XYO/QuantumScript/ObjectIteratorKey.hpp>
#include <XYO/QuantumScript/ObjectIteratorValue.hpp>

#include <XYO/QuantumScript/ExecutiveX.hpp>

namespace XYO::QuantumScript {

	Executive::Executive() {
		//
		// Init context singleton
		//
		applicationInitExecutive = nullptr;

		Context::newContext();

		configPrintStackTraceLimit = XYO_QUANTUMSCRIPT_DEFAULT_STACK_TRACE_LEVEL;
		currentContextExit = nullptr;
		currentContextReturn = nullptr;
		currentContextThrow = nullptr;
		currentContextContinue = nullptr;
		currentContextBreak = nullptr;

		includePath.newMemory();
		extensionList.newMemory();
		internalExtensionList.newMemory();
		includeSource.newMemory();
		includeUnit.newMemory();
		includeFunction.newMemory();
		errorInfoInit();

		// for sys extension
		mainCmdN = 0;
		mainCmdS = nullptr;

		isExit = false;
		exitCode = 0;

		callContextPoolSize = 0;
	};

	Executive::~Executive() {
		callContextPool.empty();
		callContextPoolSize = 0;
		instructionContext.deleteMemory();

		//
		// Release extension context
		//

		if (extensionList) {
			TDoubleEndedQueue<Extension_>::Node *scan;
			for (scan = extensionList->head; scan; scan = scan->next) {
				if (scan->value.deleteContext) {
					(*scan->value.deleteContext)();
				};
			};
		};

		//
		// Release context singleton
		//
		Context::deleteContext();
	};

	void Executive::errorInfoInit() {
		errorInfo.compileError = 0;
		errorInfo.compileLineNumber = 0;
		errorInfo.compileFileName = "";
		errorInfo.bytecodeError = "";
	};

	void Executive::initExecutive() {
		isExit = false;
		exitCode = 0;

		assembler.newMemory();

		instructionContext.newMemory();
		instructionContext->configPrintStackTraceLimit = configPrintStackTraceLimit;
		instructionContext->init();
		instructionContext->executiveSuper = this;
		instructionContext->contextStack.newMemory();
		instructionContext->contextStack->enterMaster(instructionContext->functionContext);
		instructionContext->pcContext = instructionContext->functionContext;
		instructionContext->functionContext->this_.newMemory(); // undefined

		instructionList.newMemory();

		AsmLabel skipFiberExit = assembler->assembleProgramCounter(ParserAsm::Goto, 0, 0, 0);
		AsmLabel linkExit = assembler->assemble(ParserAsm::CurrentContextExit, "", 0, 0);
		AsmLabel linkReturn = assembler->assemble(ParserAsm::Return, "", 0, 0);
		AsmLabel linkThrow = assembler->assemble(ParserAsm::Throw, "", 0, 0);
		AsmLabel linkContinue = assembler->assemble(ParserAsm::Continue, "", 0, 0);
		AsmLabel linkBreak = assembler->assemble(ParserAsm::Break, "", 0, 0);
		assembler->linkProgramCounter(skipFiberExit, assembler->assemble(ParserAsm::Mark, "", 0, 0));

		// runtime generated code links to these
		Linker linker;
		assembler->link(linker, *(instructionList.value()));
		currentContextExit = linker.getProgramCounter(linkExit);
		currentContextReturn = linker.getProgramCounter(linkReturn);
		currentContextThrow = linker.getProgramCounter(linkThrow);
		currentContextContinue = linker.getProgramCounter(linkContinue);
		currentContextBreak = linker.getProgramCounter(linkBreak);

		initExtension(LibStd::initExecutive);
		if (applicationInitExecutive) {
			(*applicationInitExecutive)(this);
		};

		instructionContext->configPrintStackTraceLimit = configPrintStackTraceLimit;
	};

	bool Executive::isExtensionLoaded(String extensionName) {
		TDoubleEndedQueue<Extension_>::Node *scan;
		extensionName = extensionName.toLowerCaseASCII();
		for (scan = extensionList->head; scan; scan = scan->next) {
			if (extensionName == scan->value.name.toLowerCaseASCII()) {
				return true;
			};
		};
		return false;
	};

	void Executive::setExtensionInfo(void *extensionId, String info) {
		((TDoubleEndedQueue<Extension_>::Node *)extensionId)->value.info = info;
	};

	void Executive::setExtensionVersion(void *extensionId, String version) {
		((TDoubleEndedQueue<Extension_>::Node *)extensionId)->value.version = version;
	};

	void Executive::setExtensionName(void *extensionId, String name) {
		((TDoubleEndedQueue<Extension_>::Node *)extensionId)->value.name = name;
	};

	void Executive::setExtensionPublic(void *extensionId, bool isPublic) {
		((TDoubleEndedQueue<Extension_>::Node *)extensionId)->value.isPublic = isPublic;
	};

	void Executive::setExtensionDeleteContext(void *extensionId, QuantumScriptExtensionDeleteContextProc deleteContext) {
		((TDoubleEndedQueue<Extension_>::Node *)extensionId)->value.deleteContext = deleteContext;
	};

	void Executive::initExtension(QuantumScriptExtensionInitProc extensionProc) {
		extensionList->push();
		(*extensionProc)(this, extensionList->head);
	};

	void Executive::setIncludeSource(String fileName, String source) {
		includeSource->set(fileName, source);
	};

	void Executive::removeIncludeSource(String fileName) {
		includeSource->remove(fileName);
	};

	bool Executive::hasIncludeSource(String fileName) {
		String dummy;
		return includeSource->get(fileName, dummy);
	};

	void Executive::setIncludeUnit(String name, CodeUnit *unit) {
		includeUnit->set(name, TPointer<CodeUnit>(unit));
	};

	void Executive::removeIncludeUnit(String name) {
		includeUnit->remove(name);
	};

	bool Executive::hasIncludeUnit(String name) {
		return includeUnit->find(name) != nullptr;
	};

	void Executive::setIncludeFunction(String name, Variable *function) {
		includeFunction->set(name, TPointer<Variable>(function));
	};

	void Executive::removeIncludeFunction(String name) {
		includeFunction->remove(name);
	};

	bool Executive::hasIncludeFunction(String name) {
		return includeFunction->find(name) != nullptr;
	};

	void Executive::registerInternalExtension(String extensionName, QuantumScriptExtensionInitProc extensionProc) {
		TDoubleEndedQueue<InternalExtension_>::Node *scan;
		String extensionName_ = extensionName.toLowerCaseASCII();
		for (scan = internalExtensionList->head; scan; scan = scan->next) {
			if (extensionName_ == scan->value.name.toLowerCaseASCII()) {
				scan->value.extensionProc = extensionProc;
				return;
			};
		};
		internalExtensionList->push();
		scan = internalExtensionList->head;
		scan->value.name = extensionName;
		scan->value.extensionProc = extensionProc;
	};

	Variable *Executive::cloneVariable(SymbolList &inSymbolList, Variable *in) {
		if (in == nullptr) {
			return VariableUndefined::newVariable();
		};
		return in->clone(inSymbolList);
	};

	int Executive::execute() {
		instructionContext->error = InstructionError::None;
		instructionContext->contextStack->enter(instructionContext->pcContext);
		Linker linker;
		assembler->link(linker, *(instructionList.value()));

		instructionContext->instructionListExecutive = instructionList.value();
		instructionContext->currentProgramCounter = reinterpret_cast<ProgramCounter *>(instructionList->head);

		return execute_(instructionContext.value());
	};

	int Executive::execute_(InstructionContext *context) {

		while (context->currentProgramCounter) {

			try {
				while (context->currentProgramCounter) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
					fflush(stdout);
#endif

					context->nextProgramCounter = reinterpret_cast<ProgramCounter *>(reinterpret_cast<InstructionList::Node *>(context->currentProgramCounter)->next);
					(*reinterpret_cast<InstructionList::Node *>(context->currentProgramCounter)->value.procedure)(
					    context,
					    reinterpret_cast<InstructionList::Node *>(context->currentProgramCounter)->value.operand);

					context->currentProgramCounter = context->nextProgramCounter;
				};
				break;

			} catch (const Error &e) {
				context->push(context->newError((const_cast<Error &>(e)).getMessage()));
				InstructionVmThrow(context, nullptr);
			};
			context->currentProgramCounter = context->nextProgramCounter;
		};

		return context->error;
	};

	int Executive::compileFile(const char *fileName) {
		return compileFile_(assembler, fileName, 0);
	};

	int Executive::compileFileSkipLines(const char *fileName, size_t skipLines) {
		return compileFile_(assembler, fileName, skipLines);
	};

	int Executive::compileFileToUnit(const char *fileName, CodeUnit &unit, size_t skipLines) {
		TPointer<Asm> unitAssembler;
		unitAssembler.newMemory();
		int retV = compileFile_(unitAssembler, fileName, skipLines);
		if (retV == VmParserError::None) {
			unit.append(*(unitAssembler->code.value()));
		};
		return retV;
	};

	int Executive::compileFile_(Asm *target, const char *fileName, size_t skipLines) {
		Parser parser;
		Input input;
		File in;
		int retV;
		char fullFile[4096];

		retV = VmParserError::Compile;

		if (Shell::realpath(fileName, fullFile, 4096)) {

			if (in.openRead(fullFile)) {
				if (input.init(&in)) {

					// nothing to skip, do not underflow skipLines
					if (skipLines > 0) {
						uint32_t lineNumber = input.lineNumber;
						while (input.read()) {
							if (input.lineNumber > lineNumber) {
								lineNumber = input.lineNumber;
								--skipLines;
								if (skipLines == 0) {
									break;
								};
							};
						};
					};

					String strSymbol("#");
					strSymbol << fullFile;
					Symbol symbolSource = Context::getSymbol(strSymbol);
					if (parser.init(&input, target, symbolSource)) {
						retV = parser.parse();
					};
				};
				in.close();
			} else {
				retV = VmParserError::FileNotFound;
			};

		} else {
			retV = VmParserError::FileNotFound;
		};

		errorInfo.compileError = retV;
		errorInfo.compileLineNumber = input.lineNumber;
		errorInfo.compileFileName = fileName;
		return retV;
	};

	// Code included at runtime runs in its own context, is linked after the current
	// instruction and is removed from the instruction list after execution
	void Executive::includeBegin(ExecutiveInclude &include) {
		include.linkBegin = assembler->assembleProgramCounter(ParserAsm::Goto, 0, 0, 0);
		include.linkStart = assembler->assemble(ParserAsm::Nop, "", 0, 0); // don't use mark, next instruction can be removed

		assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
		assembler->assemble(ParserAsm::ContextSetStack, "", 0, 0);
		include.linkFinally = assembler->assembleProgramCounter(ParserAsm::ContextSetFinally, 0, 0, 0);
		assembler->assembleProgramCounterExternal(ParserAsm::ContextSetTryReturn, currentContextReturn, 0, 0);
		assembler->assembleProgramCounterExternal(ParserAsm::ContextSetTryThrow, currentContextThrow, 0, 0);
		assembler->assembleProgramCounterExternal(ParserAsm::ContextSetTryContinue, currentContextContinue, 0, 0);
		assembler->assembleProgramCounterExternal(ParserAsm::ContextSetTryBreak, currentContextBreak, 0, 0);
	};

	void Executive::includeEnd(InstructionContext *context, ExecutiveInclude &include, bool clearIncludedFile) {
		AsmLabel linkEnd;
		AsmLabel linkCallPC;
		AsmLabel linkPC;
		AsmLabel linkEntry;

		assembler->assemble(ParserAsm::LeaveContext, "", 0, 0);
		linkEntry = assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
		linkCallPC = assembler->assembleProgramCounter(ParserAsm::ContextSetPC, 0, 0, 0);

		linkPC = assembler->assemble(ParserAsm::Nop, "", 0, 0);
		assembler->linkProgramCounter(include.linkFinally, linkPC);

		if (clearIncludedFile) {
			assembler->assemble(ParserAsm::ClearIncludedFile, "", 0, 0);
		};

		linkEnd = assembler->assembleProgramCounter(ParserAsm::InstructionListExtractAndDelete, include.linkBegin, 0, 0);
		assembler->linkProgramCounter(include.linkBegin, linkEntry);
		assembler->linkProgramCounterExternal(linkEnd, currentContextReturn);
		assembler->linkProgramCounterExternal(linkCallPC, context->nextProgramCounter);

		Linker linker;
		assembler->link(linker, *(instructionList.value()));
		context->nextProgramCounter = linker.getProgramCounter(include.linkStart);
	};

	// Remove the code assembled since includeBegin
	void Executive::includeAbort(ExecutiveInclude &include) {
		assembler->discard(include.linkBegin);
	};

	int Executive::includeAndExecuteFile(InstructionContext *context, const char *fileName) {
		if (Bytecode::isBytecodeFile(fileName)) {
			return includeAndExecuteBytecodeFile_(context, fileName);
		};
		ExecutiveInclude include;
		includeBegin(include);
		int retV = compileFile(fileName);
		if (retV == VmParserError::None) {
			context->includedFile.push(fileName);
			includeEnd(context, include, true);
			return 0;
		};
		includeAbort(include);
		return retV;
	};

	int Executive::includeAndExecuteFileSkipLines(InstructionContext *context, const char *fileName, size_t skipLines) {
		// a bytecode file has no lines
		if (Bytecode::isBytecodeFile(fileName)) {
			return includeAndExecuteBytecodeFile_(context, fileName);
		};
		ExecutiveInclude include;
		includeBegin(include);
		int retV = compileFileSkipLines(fileName, skipLines);
		if (retV == VmParserError::None) {
			context->includedFile.push(fileName);
			includeEnd(context, include, true);
			return 0;
		};
		includeAbort(include);
		return retV;
	};

	int Executive::executeExtension(InstructionContext *context, QuantumScriptExtensionProc extensionProc, void *extensionId) {
		ExecutiveInclude include;
		includeBegin(include);
		try {
			extensionProc(this, extensionId);
			includeEnd(context, include, false);
			return 0;
		} catch (...) {
		};
		includeAbort(include);
		return VmParserError::Compile;
	};

	int Executive::compileString(const char *data_, const char *tag) {
		return compileString_(assembler, data_, 0, tag);
	};

	int Executive::compileStringSkipLines(const char *data_, size_t skipLines, const char *tag) {
		return compileString_(assembler, data_, skipLines, tag);
	};

	int Executive::compileStringToUnit(const char *data_, CodeUnit &unit, const char *tag) {
		TPointer<Asm> unitAssembler;
		unitAssembler.newMemory();
		int retV = compileString_(unitAssembler, data_, 0, tag);
		if (retV == VmParserError::None) {
			unit.append(*(unitAssembler->code.value()));
		};
		return retV;
	};

	int Executive::compileString_(Asm *target, const char *data_, size_t skipLines, const char *tag) {
		Parser parser;
		Input input;
		MemoryRead in;
		int retV;

		retV = VmParserError::Compile;

		if (in.open(data_, StringCore::length(data_))) {
			if (input.init(&in)) {

				// nothing to skip, do not underflow skipLines
				if (skipLines > 0) {
					uint32_t lineNumber = input.lineNumber;
					while (input.read()) {
						if (input.lineNumber > lineNumber) {
							lineNumber = input.lineNumber;
							--skipLines;
							if (skipLines == 0) {
								break;
							};
						};
					};
				};

				String strSymbol;
				if (tag) {
					strSymbol = "!";
					strSymbol << tag;
				} else {
#ifdef XYO_QUANTUMSCRIPT_STACK_TRACE_SHOW_MEMORY_SOURCE
					strSymbol = "@";
					strSymbol << data_;
#endif
				};

				Symbol symbolSource = Context::getSymbol(strSymbol);
				if (parser.init(&input, target, symbolSource)) {
					retV = parser.parse();
				};
			};
			in.close();
		};

		errorInfo.compileError = retV;
		errorInfo.compileLineNumber = input.lineNumber;
		if (tag) {
			errorInfo.compileFileName = tag;
		} else {
			errorInfo.compileFileName = "";
		};

		return retV;
	};

	int Executive::includeAndExecuteString(InstructionContext *context, const char *data_, const char *tag) {
		ExecutiveInclude include;
		includeBegin(include);
		int retV = compileString(data_, tag);
		if (retV == VmParserError::None) {
			includeEnd(context, include, false);
			return 0;
		};
		includeAbort(include);
		return retV;
	};

	int Executive::includeAndExecuteStringSkipLines(InstructionContext *context, const char *data_, size_t skipLines, const char *tag) {
		ExecutiveInclude include;
		includeBegin(include);
		int retV = compileStringSkipLines(data_, skipLines, tag);
		if (retV == VmParserError::None) {
			includeEnd(context, include, false);
			return 0;
		};
		includeAbort(include);
		return retV;
	};

	// The named units of a bundle are added to includeUnit
	int Executive::includeAndExecuteBytecodeFile_(InstructionContext *context, const char *fileName) {
		CodeUnit unit;
		CodeUnitMap units;
		String error;

		errorInfo.compileFileName = fileName;
		errorInfo.compileLineNumber = 0;
		if (!Bytecode::loadFile(fileName, unit, error, &units)) {
			errorInfo.compileError = VmParserError::Bytecode;
			errorInfo.bytecodeError = error;
			return VmParserError::Bytecode;
		};
		errorInfo.compileError = VmParserError::None;

		CodeUnitMap::Node *scan;
		for (scan = units.begin(); scan; scan = scan->successor()) {
			includeUnit->set(scan->key, scan->value);
		};

		context->includedFile.push(fileName);
		ExecutiveInclude include;
		includeBegin(include);
		assembler->append(unit);
		includeEnd(context, include, true);
		return 0;
	};

	int Executive::includeAndExecuteUnit(InstructionContext *context, CodeUnit &unit) {
		ExecutiveInclude include;
		includeBegin(include);
		assembler->append(unit);
		includeEnd(context, include, false);
		return 0;
	};

	TPointer<Variable> Executive::newFunctionFromUnit(CodeUnit &unit, int functionHint) {
		TPointer<Asm> unitAssembler;
		unitAssembler.newMemory();

		// the code is linked at the end of the instruction list, never executed in sequence
		AsmLabel linkSkip = unitAssembler->assembleProgramCounter(ParserAsm::Goto, 0, 0, 0);
		AsmLabel linkBegin = unitAssembler->assemble(ParserAsm::Nop, "", 0, 0);
		unitAssembler->append(unit);
		unitAssembler->assemble(ParserAsm::PushUndefined, "", 0, 0);
		AsmLabel linkEnd = unitAssembler->assemble(ParserAsm::Return, "", 0, 0);
		unitAssembler->linkProgramCounter(linkSkip, unitAssembler->assemble(ParserAsm::Nop, "", 0, 0));

		Linker linker;
		unitAssembler->link(linker, *(instructionList.value()));

		TPointer<Variable> function(VariableVmFunction::newVariable(linker.getProgramCounter(linkBegin)));
		((VariableVmFunction *)function.value())->valueEnd = linker.getProgramCounter(linkEnd);
		((VariableVmFunction *)function.value())->functionHint = functionHint;
#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		((VariableVmFunction *)function.value())->functionParent.newMemory();
#endif
		return function;
	};

	// Assemble the reference to "a.b.c(x,y)" for assign
	// returns the index of '(' or of the end of name
	size_t Executive::assembleNameReference(const char *name) {
		String buf;
		char bufx[2];
		bool isFirst = true;
		size_t k;

		bufx[1] = 0;
		for (k = 0; name[k] != 0; ++k) {
			if (name[k] == '(') {
				break;
			};
			if (name[k] == '.') {
				if (isFirst) {
					isFirst = false;
					assembler->assemble(ParserAsm::PushSymbol, buf, 0, 0);
				} else {
					assembler->assemble(ParserAsm::Reference, buf, 0, 0);
				};
				buf = "";
				continue;
			};
			bufx[0] = name[k];
			buf << bufx;
		};

		if (isFirst) {
			assembler->assemble(ParserAsm::PushObjectReference, buf, 0, 0);
		} else {
			assembler->assemble(ParserAsm::ReferenceObjectReference, buf, 0, 0);
		};
		return k;
	};

	// "function(x,y)" from the arguments of "a.b.c(x,y)", k is the index of '('
	static String functionSourceHeader(const char *name, size_t k) {
		String fnSource = "function(";
		bool isEmpty = true;
		if (name[k] == '(') {
			++k;
			isEmpty = (name[k] == 0) || (name[k] == ')');
		};
		if (isEmpty) {
			fnSource << ")";
			return fnSource;
		};
		for (; name[k] != 0; ++k) {
			fnSource << name[k];
			if (name[k] == ')') {
				break;
			};
		};
		return fnSource;
	};

	void Executive::setVmFunction(const char *name, InstructionProcedure procedure, Variable *operand) {
		AsmLabel linkFunctionBegin;
		AsmLabel linkFunctionEnd;

		assembleNameReference(name);

		linkFunctionBegin = assembler->assembleProgramCounter(ParserAsm::XPushFunction, 0, 0, 0);
		linkFunctionEnd = assembler->assembleProgramCounter(ParserAsm::Goto, 0, 0, 0);

		assembler->linkProgramCounter(linkFunctionBegin, assembler->assemble(ParserAsm::Mark, "", 0, 0));

		assembler->assembleNative(ParserAsm::CallNative, VariableNativeVmFunction::newVariable(procedure, operand));

		assembler->assemble(ParserAsm::PushUndefined, "", 0, 0);
		assembler->linkProgramCounterEnd(linkFunctionBegin, assembler->assemble(ParserAsm::Return, "", 0, 0));

		assembler->linkProgramCounter(linkFunctionEnd, assembler->assemble(ParserAsm::Assign, "", 0, 0));

	};

	int Executive::setVmFunctionFromFile(const char *name, const char *fileName) {
		AsmLabel linkFunctionBegin;
		AsmLabel linkFunctionEnd;
		int retV;

		size_t k = assembleNameReference(name);

		linkFunctionBegin = assembler->assembleProgramCounter(ParserAsm::XPushFunction, 0, 0, 0);
		linkFunctionEnd = assembler->assembleProgramCounter(ParserAsm::Goto, 0, 0, 0);

		assembler->linkProgramCounter(linkFunctionBegin, assembler->assemble(ParserAsm::Mark, "", 0, 0));

		String fnSource = functionSourceHeader(name, k);

		String content;
		if (!Shell::fileGetContents(fileName, content)) {
			return VmParserError::FileNotFound;
		};

		fnSource << "{" << content << "}";

		retV = compileFile(fileName);

		uint32_t fnSourceSymbol = Context::getSymbol(fnSource);
		assembler->linkProgramCounterSource(linkFunctionBegin, fnSourceSymbol, 0);

		assembler->assemble(ParserAsm::PushUndefined, "", 0, 0);
		assembler->linkProgramCounterEnd(linkFunctionBegin, assembler->assemble(ParserAsm::Return, "", 0, 0));

		assembler->linkProgramCounter(linkFunctionEnd, assembler->assemble(ParserAsm::Assign, "", 0, 0));

		return retV;
	};

	int Executive::setVmFunctionFromString(const char *name, const char *data, const char *tag) {
		AsmLabel linkFunctionBegin;
		AsmLabel linkFunctionEnd;
		int retV;

		size_t k = assembleNameReference(name);

		linkFunctionBegin = assembler->assembleProgramCounter(ParserAsm::XPushFunction, 0, 0, 0);
		linkFunctionEnd = assembler->assembleProgramCounter(ParserAsm::Goto, 0, 0, 0);

		assembler->linkProgramCounter(linkFunctionBegin, assembler->assemble(ParserAsm::Mark, "", 0, 0));

		String fnSource = functionSourceHeader(name, k);
		fnSource << "{" << data << "}";

		retV = compileString(data, tag);

		uint32_t fnSourceSymbol = Context::getSymbol(fnSource);
		assembler->linkProgramCounterSource(linkFunctionBegin, fnSourceSymbol, 0);

		assembler->assemble(ParserAsm::PushUndefined, "", 0, 0);
		assembler->linkProgramCounterEnd(linkFunctionBegin, assembler->assemble(ParserAsm::Return, "", 0, 0));

		assembler->linkProgramCounter(linkFunctionEnd, assembler->assemble(ParserAsm::Assign, "", 0, 0));

		return retV;
	};

	int Executive::setVmFunctionFromFileX(InstructionContext *context, const char *name, const char *fileName) {
		ExecutiveInclude include;
		includeBegin(include);
		int retV = setVmFunctionFromFile(name, fileName);
		if (retV == VmParserError::None) {
			includeEnd(context, include, false);
			return 0;
		};
		includeAbort(include);
		return retV;
	};

	int Executive::setVmFunctionFromStringX(InstructionContext *context, const char *name, const char *data, const char *tag) {
		ExecutiveInclude include;
		includeBegin(include);
		int retV = setVmFunctionFromString(name, data, tag);
		if (retV == VmParserError::None) {
			includeEnd(context, include, false);
			return 0;
		};
		includeAbort(include);
		return retV;
	};

	// reset and keep for reuse, allocation and init of a context is expensive
	void Executive::callContextRelease(TPointer<InstructionContext> &fnContext) {
		if (callContextPoolSize >= XYO_QUANTUMSCRIPT_CALL_CONTEXT_POOL_SIZE) {
			return;
		};
		fnContext->empty();
		fnContext->contextStack->empty();
		fnContext->stackTrace.deleteMemory();
		fnContext->returnValue = Context::getValueUndefined();
		fnContext->error = InstructionError::None;
		fnContext->errorInfo = "";
		fnContext->includedFile.empty();
		fnContext->listIncludeOnce.empty();
		fnContext->pcContext = nullptr;
		fnContext->functionContext = nullptr;
		fnContext->currentProgramCounter = nullptr;
		fnContext->nextProgramCounter = nullptr;
		callContextPool.push(fnContext);
		++callContextPoolSize;
	};

	TPointer<Variable> Executive::callVmFunction(Variable *fnProcedure, Variable *fnThis, Variable *fnParameters) {
		if (currentContextExit == nullptr) {
			return nullptr;
		};
		if (fnProcedure) {
			if (TIsType<VariableVmFunction>(fnProcedure)) {
				TPointer<Variable> fnParametersX(fnParameters);
				if (fnParameters) {
					if (TIsTypeExact<VariableUndefined>(fnParameters)) {
						fnParametersX = VariableArray::newVariable();
					};
				} else {
					fnParametersX = VariableArray::newVariable();
				};
				if (TIsType<VariableArray>(fnParametersX)) {
					TPointer<InstructionContext> fnContext;
					if (!callContextPool.pop(fnContext)) {
						fnContext.newMemory();
						fnContext->executiveSuper = this;
						fnContext->init();
						fnContext->contextStack.newMemory();
					} else {
						--callContextPoolSize;
					};

					fnContext->configPrintStackTraceLimit = configPrintStackTraceLimit;
					fnContext->returnValue = Context::getValueUndefined();
					fnContext->contextStack->enterMaster(fnContext->functionContext);
					fnContext->functionContext->this_ = Context::getValueUndefined();
					fnContext->pcContext = fnContext->functionContext;
					fnContext->pcContext->pc_ = currentContextExit;
					fnContext->instructionListExecutive = instructionList.value();

					fnContext->error = InstructionError::None;
					fnContext->currentProgramCounter = nullptr;
					fnContext->nextProgramCounter = currentContextExit;

					TPointer<Variable> functionArguments;
					functionArguments = VariableArray::newVariable();
					(*(((VariableArray *)(functionArguments.value()))->value))[0] = fnParametersX;

					TPointer<Variable> fnThisX(fnThis);

					if (fnThis) {
					} else {
						fnThisX = VariableUndefined::newVariable();
					};
					// apply
					fnContext->push(fnProcedure);
					fnContext->push(fnThisX);
					fnContext->push(functionArguments);
					InstructionVmXCallThisModeApply(fnContext, nullptr);
					fnContext->currentProgramCounter = fnContext->nextProgramCounter;

					// -- executor
					int error = Executive::execute_(fnContext.value());
					if (error == InstructionError::None) {
						TPointer<Variable> retV(fnContext->returnValue);
						callContextRelease(fnContext);
						return retV;
					};
					if (error == InstructionError::Throw) {
						TPointer<Variable> throwValue;
						fnContext->pop(throwValue);
						TPointer<VariableStackTrace> stackTrace((VariableStackTrace *)VariableStackTrace::newVariable(fnContext->stackTrace, fnContext));
						stackTrace->configPrintStackTraceLimit = fnContext->configPrintStackTraceLimit;
						ExecutiveX::setStackTrace(stackTrace->toString());
						stackTrace.deleteMemory();
						fnContext->stackTrace.deleteMemory();
						callContextRelease(fnContext);
						throw Error((throwValue->getPropertyBySymbol(Context::getSymbol("message")))->toString());
					};
					String errorInfo(fnContext->errorInfo);
					callContextRelease(fnContext);
					throw Error(errorInfo);
				};
			};
		};
		return nullptr;
	};

	void Executive::setFunction(const char *name, Variable *nativeFunction) {
		if (TIsType<VariableVmFunction>(nativeFunction)) {
			throw Error("setFunction require native function");
		};

		assembleNameReference(name);
		assembler->assembleNative(ParserAsm::SetFunction, nativeFunction);
	};

	void Executive::compileStringX(const char *source, const char *tag) {
		if (compileString(source, tag) != 0) {
			char buf[2048];
			if (strlen(errorInfo.compileFileName) == 0) {
				snprintf(buf, sizeof(buf), "Compile error on line %u",
				        errorInfo.compileLineNumber);
			} else {
				snprintf(buf, sizeof(buf), "Compile error in %s line %u",
				        errorInfo.compileFileName,
				        errorInfo.compileLineNumber);
			};
			throw Error(buf);
		};
	};

	void Executive::setFunction2(const char *name, FunctionProcedure functionProcedure) {
		TPointer<Variable> functionV(VariableFunction::newVariable(nullptr, nullptr, nullptr, functionProcedure, nullptr, nullptr));
		setFunction(name, functionV);
	};

	void Executive::setFunction3(const char *name, FunctionProcedure functionProcedure, Object *super) {
		TPointer<Variable> functionV(VariableFunction::newVariable(nullptr, nullptr, nullptr, functionProcedure, super, nullptr));
		setFunction(name, functionV);
	};

	void Executive::setFunction4(const char *name, FunctionProcedure functionProcedure, void *valueSuper) {
		TPointer<Variable> functionV(VariableFunction::newVariable(nullptr, nullptr, nullptr, functionProcedure, nullptr, valueSuper));
		setFunction(name, functionV);
	};

	void Executive::setFunction5(const char *name, FunctionProcedure functionProcedure, Object *super, void *valueSuper) {
		TPointer<Variable> functionV(VariableFunction::newVariable(nullptr, nullptr, nullptr, functionProcedure, super, valueSuper));
		setFunction(name, functionV);
	};

	void Executive::setFunctionWithYield2(const char *name, FunctionProcedureWithYield functionProcedureWithYield) {
		TPointer<Variable> functionV(VariableFunctionWithYield::newVariable(nullptr, nullptr, nullptr, functionProcedureWithYield, nullptr, nullptr));
		setFunction(name, functionV);
	};

	void Executive::setFunctionWithYield3(const char *name, FunctionProcedureWithYield functionProcedureWithYield, Object *super) {
		TPointer<Variable> functionV(VariableFunctionWithYield::newVariable(nullptr, nullptr, nullptr, functionProcedureWithYield, super, nullptr));
		setFunction(name, functionV);
	};

	void Executive::setFunctionWithYield4(const char *name, FunctionProcedureWithYield functionProcedureWithYield, void *valueSuper) {
		TPointer<Variable> functionV(VariableFunctionWithYield::newVariable(nullptr, nullptr, nullptr, functionProcedureWithYield, nullptr, valueSuper));
		setFunction(name, functionV);
	};

	void Executive::setFunctionWithYield5(const char *name, FunctionProcedureWithYield functionProcedureWithYield, Object *super, void *valueSuper) {
		TPointer<Variable> functionV(VariableFunctionWithYield::newVariable(nullptr, nullptr, nullptr, functionProcedureWithYield, super, valueSuper));
		setFunction(name, functionV);
	};

	void Executive::compileEnd() {
		assembler->assemble(ParserAsm::PushUndefined, "", 0, 0);
		assembler->assemble(ParserAsm::CurrentContextExit, "", 0, 0);
	};

	void Executive::initMemory() {
		//
		// not really required
		//
		Context::initMemory();
		TMemory<TDoubleEndedQueue<String>>::initMemory();
		TMemory<InstructionContext>::initMemory();
		TMemory<TDoubleEndedQueue<TPointer<InstructionContext>>>::initMemory();
		TMemory<InstructionList>::initMemory();
		Parser::initMemory();
		TMemory<ArrayIteratorKey>::initMemory();
		TMemory<ArrayIteratorValue>::initMemory();
		TMemory<ObjectIteratorKey>::initMemory();
		TMemory<ObjectIteratorValue>::initMemory();
		TMemory<Variable>::initMemory();
		TMemory<VariableNull>::initMemory();
		TMemory<VariableBoolean>::initMemory();
		TMemory<VariableNumber>::initMemory();
		TMemory<VariableString>::initMemory();
		TMemory<VariableSymbol>::initMemory();
		TMemory<VariableArray>::initMemory();
		TMemory<VariableAssociativeArray>::initMemory();
		TMemory<VariableObject>::initMemory();
		TMemory<VariableFunction>::initMemory();
		TMemory<VariableFunctionWithYield>::initMemory();
		TMemory<VariableResource>::initMemory();
		TMemory<VariableArgumentLevel>::initMemory();
		TMemory<VariableNativeVmFunction>::initMemory();
		TMemory<VariableOperator21>::initMemory();
		TMemory<VariableOperator22>::initMemory();
		TMemory<VariableOperator23>::initMemory();
		TMemory<VariableOperator31>::initMemory();
		TMemory<VariableReferenceSymbol>::initMemory();
		TMemory<VariableReferenceVariable>::initMemory();
		TMemory<VariableStackTrace>::initMemory();
		TMemory<VariableVmProgramCounter>::initMemory();
		TMemory<VariableVmFunction>::initMemory();
		TMemory<Array>::initMemory();
		ExecutiveContext::initMemory();
	};

	void Executive::endProcessing() {
		if (assembler) {
			assembler->endProcessing();
		};
		assembler.deleteMemory();
		includeUnit.deleteMemory();
		includeFunction.deleteMemory();
		if (instructionList) {
			instructionList->empty();
		};
		instructionList.deleteMemory();
		includePath.deleteMemory();
		//
		//
		//
		if (extensionList) {
			TDoubleEndedQueue<Extension_>::Node *scan;
			for (scan = extensionList->head; scan; scan = scan->next) {
				if (scan->value.deleteContext) {
					(*scan->value.deleteContext)();
				};
			};
		};
		//
		//
		//

		extensionList.deleteMemory();
		internalExtensionList.deleteMemory();

		callContextPool.empty();
		callContextPoolSize = 0;
		instructionContext.deleteMemory();
		Context::deleteContext();
	};

};
