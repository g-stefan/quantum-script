// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
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
		errorInfoInit();

		// for sys extension
		mainCmdN = 0;
		mainCmdS = nullptr;

		isExit = false;
		exitCode = 0;
	};

	Executive::~Executive() {
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

		assembler->instructionList.newMemory();
		ProgramCounter *skipFiberExit;

		skipFiberExit = assembler->assembleProgramCounter(ParserAsm::Goto, nullptr, 0, 0);
		currentContextExit = assembler->assemble(ParserAsm::CurrentContextExit, "", 0, 0);
		currentContextReturn = assembler->assemble(ParserAsm::Return, "", 0, 0);
		currentContextThrow = assembler->assemble(ParserAsm::Throw, "", 0, 0);
		currentContextContinue = assembler->assemble(ParserAsm::Continue, "", 0, 0);
		currentContextBreak = assembler->assemble(ParserAsm::Break, "", 0, 0);
		assembler->linkProgramCounter(skipFiberExit, assembler->assemble(ParserAsm::Mark, "", 0, 0));

		initExtension(LibStd::initExecutive);
		if (applicationInitExecutive) {
			(*applicationInitExecutive)(this);
		};

		instructionContext->configPrintStackTraceLimit = configPrintStackTraceLimit;
	};

	bool Executive::isExtensionLoaded(String extensionName) {
		TDoubleEndedQueue<Extension_>::Node *scan;
		extensionName = extensionName.toLowerCaseAscii();
		for (scan = extensionList->head; scan; scan = scan->next) {
			if (extensionName == scan->value.name.toLowerCaseAscii()) {
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

	void Executive::registerInternalExtension(String extensionName, QuantumScriptExtensionInitProc extensionProc) {
		TDoubleEndedQueue<InternalExtension_>::Node *scan;
		String extensionName_ = extensionName.toLowerCaseAscii();
		for (scan = internalExtensionList->head; scan; scan = scan->next) {
			if (extensionName_ == scan->value.name.toLowerCaseAscii()) {
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
		instructionContext->instructionListExecutive = assembler->instructionList.value();
		instructionContext->currentProgramCounter = reinterpret_cast<ProgramCounter *>(assembler->instructionList->head);

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
		Parser parser;
		Input input;
		File in;
		int retV;
		char fullFile[4096];

		retV = VmParserError::Compile;

		if (Shell::realpath(fileName, fullFile, 4096)) {

			if (in.openRead(fullFile)) {
				if (input.init(&in)) {
					String strSymbol("#");
					strSymbol << fullFile;
					Symbol symbolSource = Context::getSymbol(strSymbol);
					if (parser.init(&input, assembler, symbolSource)) {
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

		assembler->resetLinks();

		errorInfo.compileError = retV;
		errorInfo.compileLineNumber = input.lineNumber;
		errorInfo.compileFileName = fileName;
		return retV;
	};

	int Executive::compileFileSkipLines(const char *fileName, size_t skipLines) {
		Parser parser;
		Input input;
		File in;
		int retV;
		char fullFile[4096];

		retV = VmParserError::Compile;

		if (Shell::realpath(fileName, fullFile, 4096)) {

			if (in.openRead(fullFile)) {
				if (input.init(&in)) {

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

					String strSymbol("#");
					strSymbol << fullFile;
					Symbol symbolSource = Context::getSymbol(strSymbol);
					if (parser.init(&input, assembler, symbolSource)) {
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

		assembler->resetLinks();

		errorInfo.compileError = retV;
		errorInfo.compileLineNumber = input.lineNumber;
		errorInfo.compileFileName = fileName;
		return retV;
	};

	int Executive::includeAndExecuteFile(InstructionContext *context, const char *fileName) {
		ProgramCounter *linkBegin;
		ProgramCounter *linkStart;
		ProgramCounter *linkEnd;
		ProgramCounter *nextProgramCounter_;
		ProgramCounter *currentProgramCounter_;
		ProgramCounter *linkFinally;
		ProgramCounter *linkCallPC;
		ProgramCounter *linkPC;
		ProgramCounter *linkEntry;
		int retV;

		linkBegin = assembler->assembleProgramCounter(ParserAsm::Goto, nullptr, 0, 0);
		linkStart = assembler->assemble(ParserAsm::Nop, "", 0, 0); // don't use mark, next instruction can be removed

		assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
		assembler->assemble(ParserAsm::ContextSetStack, "", 0, 0);
		linkFinally = assembler->assembleProgramCounter(ParserAsm::ContextSetFinally, nullptr, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryReturn, currentContextReturn, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryThrow, currentContextThrow, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryContinue, currentContextContinue, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryBreak, currentContextBreak, 0, 0);

		retV = compileFile(fileName);
		if (retV == VmParserError::None) {
			context->includedFile.push(fileName);

			assembler->assemble(ParserAsm::LeaveContext, "", 0, 0);
			linkEntry = assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
			linkCallPC = assembler->assembleProgramCounter(ParserAsm::ContextSetPC, nullptr, 0, 0);

			linkPC = assembler->assemble(ParserAsm::Nop, "", 0, 0);
			assembler->linkProgramCounter(linkFinally, linkPC);

			assembler->assemble(ParserAsm::ClearIncludedFile, "", 0, 0);

			linkEnd = assembler->assembleProgramCounter(ParserAsm::InstructionListExtractAndDelete, linkBegin, 0, 0);
			assembler->linkProgramCounter(linkBegin, linkEntry);
			assembler->linkProgramCounter(linkEnd, currentContextReturn);
			assembler->linkProgramCounter(linkCallPC, context->nextProgramCounter);
			context->nextProgramCounter = linkStart;

			assembler->resetLinks();
			return 0;
		};

		InstructionList instructionList;
		TDoubleEndedQueue<InstructionX>::Node *pcBegin = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(linkBegin);
		TDoubleEndedQueue<InstructionX>::Node *pcEnd = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(assembler->assemble(ParserAsm::Nop, "", 0, 0));
		context->instructionListExecutive->extractList(pcBegin, pcEnd);
		instructionList.setList(pcBegin, pcEnd);
		return retV;
	};

	int Executive::includeAndExecuteFileSkipLines(InstructionContext *context, const char *fileName, size_t skipLines) {
		ProgramCounter *linkBegin;
		ProgramCounter *linkStart;
		ProgramCounter *linkEnd;
		ProgramCounter *nextProgramCounter_;
		ProgramCounter *currentProgramCounter_;
		ProgramCounter *linkFinally;
		ProgramCounter *linkCallPC;
		ProgramCounter *linkPC;
		ProgramCounter *linkEntry;
		int retV;

		linkBegin = assembler->assembleProgramCounter(ParserAsm::Goto, nullptr, 0, 0);
		linkStart = assembler->assemble(ParserAsm::Nop, "", 0, 0); // don't use mark, next instruction can be removed

		assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
		assembler->assemble(ParserAsm::ContextSetStack, "", 0, 0);
		linkFinally = assembler->assembleProgramCounter(ParserAsm::ContextSetFinally, nullptr, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryReturn, currentContextReturn, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryThrow, currentContextThrow, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryContinue, currentContextContinue, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryBreak, currentContextBreak, 0, 0);

		retV = compileFileSkipLines(fileName, skipLines);
		if (retV == VmParserError::None) {
			context->includedFile.push(fileName);

			assembler->assemble(ParserAsm::LeaveContext, "", 0, 0);
			linkEntry = assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
			linkCallPC = assembler->assembleProgramCounter(ParserAsm::ContextSetPC, nullptr, 0, 0);

			linkPC = assembler->assemble(ParserAsm::Nop, "", 0, 0);
			assembler->linkProgramCounter(linkFinally, linkPC);

			assembler->assemble(ParserAsm::ClearIncludedFile, "", 0, 0);

			linkEnd = assembler->assembleProgramCounter(ParserAsm::InstructionListExtractAndDelete, linkBegin, 0, 0);
			assembler->linkProgramCounter(linkBegin, linkEntry);
			assembler->linkProgramCounter(linkEnd, currentContextReturn);
			assembler->linkProgramCounter(linkCallPC, context->nextProgramCounter);
			context->nextProgramCounter = linkStart;

			assembler->resetLinks();
			return 0;
		};

		InstructionList instructionList;
		TDoubleEndedQueue<InstructionX>::Node *pcBegin = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(linkBegin);
		TDoubleEndedQueue<InstructionX>::Node *pcEnd = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(assembler->assemble(ParserAsm::Nop, "", 0, 0));
		context->instructionListExecutive->extractList(pcBegin, pcEnd);
		instructionList.setList(pcBegin, pcEnd);
		return retV;
	};

	int Executive::executeExtension(InstructionContext *context, QuantumScriptExtensionProc extensionProc, void *extensionId) {
		ProgramCounter *linkBegin;
		ProgramCounter *linkStart;
		ProgramCounter *linkEnd;
		ProgramCounter *nextProgramCounter_;
		ProgramCounter *currentProgramCounter_;
		ProgramCounter *linkFinally;
		ProgramCounter *linkCallPC;
		ProgramCounter *linkPC;
		ProgramCounter *linkEntry;
		int retV;

		linkBegin = assembler->assembleProgramCounter(ParserAsm::Goto, nullptr, 0, 0);
		linkStart = assembler->assemble(ParserAsm::Nop, "", 0, 0); // don't use mark, next instruction can be removed

		assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
		assembler->assemble(ParserAsm::ContextSetStack, "", 0, 0);
		linkFinally = assembler->assembleProgramCounter(ParserAsm::ContextSetFinally, nullptr, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryReturn, currentContextReturn, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryThrow, currentContextThrow, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryContinue, currentContextContinue, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryBreak, currentContextBreak, 0, 0);

		try {
			extensionProc(this, extensionId);

			assembler->assemble(ParserAsm::LeaveContext, "", 0, 0);
			linkEntry = assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
			linkCallPC = assembler->assembleProgramCounter(ParserAsm::ContextSetPC, nullptr, 0, 0);

			linkPC = assembler->assemble(ParserAsm::Nop, "", 0, 0);
			assembler->linkProgramCounter(linkFinally, linkPC);

			linkEnd = assembler->assembleProgramCounter(ParserAsm::InstructionListExtractAndDelete, linkBegin, 0, 0);
			assembler->linkProgramCounter(linkBegin, linkEntry);
			assembler->linkProgramCounter(linkEnd, currentContextReturn);
			assembler->linkProgramCounter(linkCallPC, context->nextProgramCounter);
			context->nextProgramCounter = linkStart;

			assembler->resetLinks();
			return 0;

		} catch (...) {
		};

		InstructionList instructionList;
		TDoubleEndedQueue<InstructionX>::Node *pcBegin = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(linkBegin);
		TDoubleEndedQueue<InstructionX>::Node *pcEnd = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(assembler->assemble(ParserAsm::Nop, "", 0, 0));
		context->instructionListExecutive->extractList(pcBegin, pcEnd);
		instructionList.setList(pcBegin, pcEnd);
		return VmParserError::Compile;
	};

	int Executive::compileString(const char *data_, const char *tag) {
		Parser parser;
		Input input;
		MemoryRead in;
		int retV;

		retV = VmParserError::Compile;

		if (in.open(data_, StringCore::length(data_))) {
			if (input.init(&in)) {

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
				if (parser.init(&input, assembler, symbolSource)) {
					retV = parser.parse();
				};
			};
			in.close();
		};
		assembler->resetLinks();

		errorInfo.compileError = retV;
		errorInfo.compileLineNumber = input.lineNumber;
		if (tag) {
			errorInfo.compileFileName = tag;
		} else {
			errorInfo.compileFileName = "";
		};

		return retV;
	};

	int Executive::compileStringSkipLines(const char *data_, size_t skipLines, const char *tag) {
		Parser parser;
		Input input;
		MemoryRead in;
		int retV;

		retV = VmParserError::Compile;

		if (in.open(data_, StringCore::length(data_))) {
			if (input.init(&in)) {

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
				if (parser.init(&input, assembler, symbolSource)) {
					retV = parser.parse();
				};
			};
			in.close();
		};
		assembler->resetLinks();

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
		ProgramCounter *linkBegin;
		ProgramCounter *linkStart;
		ProgramCounter *linkEnd;
		ProgramCounter *nextProgramCounter_;
		ProgramCounter *currentProgramCounter_;
		ProgramCounter *linkFinally;
		ProgramCounter *linkCallPC;
		ProgramCounter *linkPC;
		ProgramCounter *linkEntry;
		int retV;

		linkBegin = assembler->assembleProgramCounter(ParserAsm::Goto, nullptr, 0, 0);
		linkStart = assembler->assemble(ParserAsm::Nop, "", 0, 0); // don't use mark, next instruction can be removed

		assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
		assembler->assemble(ParserAsm::ContextSetStack, "", 0, 0);
		linkFinally = assembler->assembleProgramCounter(ParserAsm::ContextSetFinally, nullptr, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryReturn, currentContextReturn, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryThrow, currentContextThrow, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryContinue, currentContextContinue, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryBreak, currentContextBreak, 0, 0);

		retV = compileString(data_, tag);
		if (retV == VmParserError::None) {
			assembler->assemble(ParserAsm::LeaveContext, "", 0, 0);
			linkEntry = assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
			linkCallPC = assembler->assembleProgramCounter(ParserAsm::ContextSetPC, nullptr, 0, 0);

			linkPC = assembler->assemble(ParserAsm::Nop, "", 0, 0);
			assembler->linkProgramCounter(linkFinally, linkPC);

			linkEnd = assembler->assembleProgramCounter(ParserAsm::InstructionListExtractAndDelete, linkBegin, 0, 0);
			assembler->linkProgramCounter(linkBegin, linkEntry);
			assembler->linkProgramCounter(linkEnd, currentContextReturn);
			assembler->linkProgramCounter(linkCallPC, context->nextProgramCounter);
			context->nextProgramCounter = linkStart;

			assembler->resetLinks();
			return 0;
		};

		InstructionList instructionList;
		TDoubleEndedQueue<InstructionX>::Node *pcBegin = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(linkBegin);
		TDoubleEndedQueue<InstructionX>::Node *pcEnd = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(assembler->assemble(ParserAsm::Nop, "", 0, 0));
		context->instructionListExecutive->extractList(pcBegin, pcEnd);
		instructionList.setList(pcBegin, pcEnd);
		return retV;
	};

	int Executive::includeAndExecuteStringSkipLines(InstructionContext *context, const char *data_, size_t skipLines, const char *tag) {
		ProgramCounter *linkBegin;
		ProgramCounter *linkStart;
		ProgramCounter *linkEnd;
		ProgramCounter *nextProgramCounter_;
		ProgramCounter *currentProgramCounter_;
		ProgramCounter *linkFinally;
		ProgramCounter *linkCallPC;
		ProgramCounter *linkPC;
		ProgramCounter *linkEntry;
		int retV;

		linkBegin = assembler->assembleProgramCounter(ParserAsm::Goto, nullptr, 0, 0);
		linkStart = assembler->assemble(ParserAsm::Nop, "", 0, 0); // don't use mark, next instruction can be removed

		assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
		assembler->assemble(ParserAsm::ContextSetStack, "", 0, 0);
		linkFinally = assembler->assembleProgramCounter(ParserAsm::ContextSetFinally, nullptr, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryReturn, currentContextReturn, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryThrow, currentContextThrow, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryContinue, currentContextContinue, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryBreak, currentContextBreak, 0, 0);

		retV = compileStringSkipLines(data_, skipLines, tag);
		if (retV == VmParserError::None) {
			assembler->assemble(ParserAsm::LeaveContext, "", 0, 0);
			linkEntry = assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
			linkCallPC = assembler->assembleProgramCounter(ParserAsm::ContextSetPC, nullptr, 0, 0);

			linkPC = assembler->assemble(ParserAsm::Nop, "", 0, 0);
			assembler->linkProgramCounter(linkFinally, linkPC);

			linkEnd = assembler->assembleProgramCounter(ParserAsm::InstructionListExtractAndDelete, linkBegin, 0, 0);
			assembler->linkProgramCounter(linkBegin, linkEntry);
			assembler->linkProgramCounter(linkEnd, currentContextReturn);
			assembler->linkProgramCounter(linkCallPC, context->nextProgramCounter);
			context->nextProgramCounter = linkStart;

			assembler->resetLinks();
			return 0;
		};

		InstructionList instructionList;
		TDoubleEndedQueue<InstructionX>::Node *pcBegin = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(linkBegin);
		TDoubleEndedQueue<InstructionX>::Node *pcEnd = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(assembler->assemble(ParserAsm::Nop, "", 0, 0));
		context->instructionListExecutive->extractList(pcBegin, pcEnd);
		instructionList.setList(pcBegin, pcEnd);
		return retV;
	};

	void Executive::setVmFunction(const char *name, InstructionProcedure procedure, Variable *operand) {
		ProgramCounter *linkFunctionBegin;
		ProgramCounter *linkFunctionEnd;
		ProgramCounter *linkFunctionEnter;

		String buf;
		char bufx[2];
		int lk;
		int k;
		int isFirst;

		int isEmpty;
		int retV;
		int level;
		char sLevel[32];

		retV = 0;
		isFirst = 1;
		bufx[1] = 0;

		for (k = 0; name[k] != 0; ++k) {
			if (name[k] == '(') {
				break;
			};
			if (name[k] == '.') {

				if (isFirst) {
					isFirst = 0;
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

		linkFunctionBegin = assembler->assembleProgramCounter(ParserAsm::XPushFunction, nullptr, 0, 0);
		linkFunctionEnd = assembler->assembleProgramCounter(ParserAsm::Goto, nullptr, 0, 0);

		assembler->linkProgramCounter(linkFunctionBegin, assembler->assemble(ParserAsm::Mark, "", 0, 0));

		isEmpty = 0;
		if (name[k] == '(') {
			++k;
			if (name[k] == 0) {
				isEmpty = 1;
			} else if (name[k] == ')') {
				isEmpty = 1;
			};
		} else {
			isEmpty = 1;
		};

		level = 0;

		if (isEmpty) {
		} else {
			buf = "";
			for (; name[k] != 0; ++k) {
				if (name[k] == ')') {

					/* - no need - */

					break;
				};
				if (name[k] == ',') {

					/* - no need - */

					buf = "";
					continue;
				};
				bufx[0] = name[k];
				buf << bufx;
			};
		};

		assembler->assembleDirect(InstructionVmCallNative, VariableNativeVmFunction::newVariable(procedure, operand));

		assembler->assemble(ParserAsm::PushUndefined, "", 0, 0);
		assembler->linkProgramCounterEnd(linkFunctionBegin, assembler->assemble(ParserAsm::Return, "", 0, 0));

		assembler->linkProgramCounter(linkFunctionEnd, assembler->assemble(ParserAsm::Assign, "", 0, 0));

		assembler->resetLinks();
	};

	int Executive::setVmFunctionFromFile(const char *name, const char *fileName) {
		ProgramCounter *linkFunctionBegin;
		ProgramCounter *linkFunctionEnd;
		ProgramCounter *linkFunctionEnter;

		String buf;
		char bufx[2];
		int lk;
		int k;
		int isFirst;

		int isEmpty;
		int retV;
		int level;
		char sLevel[32];

		retV = 0;
		isFirst = 1;

		bufx[1] = 0;

		for (k = 0; name[k] != 0; ++k) {
			if (name[k] == '(') {
				break;
			};
			if (name[k] == '.') {

				if (isFirst) {
					isFirst = 0;
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

		linkFunctionBegin = assembler->assembleProgramCounter(ParserAsm::XPushFunction, nullptr, 0, 0);

		linkFunctionEnd = assembler->assembleProgramCounter(ParserAsm::Goto, nullptr, 0, 0);

		assembler->linkProgramCounter(linkFunctionBegin, assembler->assemble(ParserAsm::Mark, "", 0, 0));

		String fnSource = "function(";

		isEmpty = 0;
		if (name[k] == '(') {
			++k;
			if (name[k] == 0) {
				isEmpty = 1;
			} else if (name[k] == ')') {
				isEmpty = 1;
			};
		} else {
			isEmpty = 1;
		};

		level = 0;

		if (isEmpty) {
			fnSource << ")";
		} else {
			buf = "";
			for (; name[k] != 0; ++k) {
				fnSource << name[k];

				if (name[k] == ')') {

					/* - no need - */

					break;
				};
				if (name[k] == ',') {

					/* - no need - */

					buf = "";
					continue;
				};
				bufx[0] = name[k];
				buf << bufx;
			};
		};

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

		assembler->resetLinks();
		return retV;
	};

	int Executive::setVmFunctionFromString(const char *name, const char *data, const char *tag) {
		ProgramCounter *linkFunctionBegin;
		ProgramCounter *linkFunctionEnd;
		ProgramCounter *linkFunctionEnter;

		String buf;
		char bufx[2];
		int lk;
		int k;
		int isFirst;

		int isEmpty;
		int retV;
		int level;
		char sLevel[32];

		isFirst = 1;
		bufx[1] = 0;

		for (k = 0; name[k] != 0; ++k) {
			if (name[k] == '(') {
				break;
			};
			if (name[k] == '.') {

				if (isFirst) {
					isFirst = 0;
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

		String fnSource = "function(";

		linkFunctionBegin = assembler->assembleProgramCounter(ParserAsm::XPushFunction, nullptr, 0, 0);

		linkFunctionEnd = assembler->assembleProgramCounter(ParserAsm::Goto, nullptr, 0, 0);

		assembler->linkProgramCounter(linkFunctionBegin, assembler->assemble(ParserAsm::Mark, "", 0, 0));

		isEmpty = 0;
		if (name[k] == '(') {
			++k;
			if (name[k] == 0) {
				isEmpty = 1;
			} else if (name[k] == ')') {
				isEmpty = 1;
			};
		} else {
			isEmpty = 1;
		};

		level = 0;

		if (isEmpty) {
			fnSource << ")";
		} else {
			buf = "";
			for (; name[k] != 0; ++k) {
				fnSource << name[k];
				if (name[k] == ')') {

					/* - no need - */

					break;
				};
				if (name[k] == ',') {

					/* - no need - */

					buf = "";
					continue;
				};
				bufx[0] = name[k];
				buf << bufx;
			};
		};

		fnSource << "{" << data << "}";

		retV = compileString(data, tag);

		uint32_t fnSourceSymbol = Context::getSymbol(fnSource);
		assembler->linkProgramCounterSource(linkFunctionBegin, fnSourceSymbol, 0);

		assembler->assemble(ParserAsm::PushUndefined, "", 0, 0);
		assembler->linkProgramCounterEnd(linkFunctionBegin, assembler->assemble(ParserAsm::Return, "", 0, 0));

		assembler->linkProgramCounter(linkFunctionEnd, assembler->assemble(ParserAsm::Assign, "", 0, 0));

		assembler->resetLinks();
		return retV;
	};

	int Executive::setVmFunctionFromFileX(InstructionContext *context, const char *name, const char *fileName) {
		ProgramCounter *linkBegin;
		ProgramCounter *linkStart;
		ProgramCounter *linkEnd;
		ProgramCounter *nextProgramCounter_;
		ProgramCounter *currentProgramCounter_;
		ProgramCounter *linkFinally;
		ProgramCounter *linkCallPC;
		ProgramCounter *linkPC;
		ProgramCounter *linkEntry;
		int retV;

		linkBegin = assembler->assembleProgramCounter(ParserAsm::Goto, nullptr, 0, 0);
		linkStart = assembler->assemble(ParserAsm::Nop, "", 0, 0); // don't use mark, next instruction can be removed

		assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
		assembler->assemble(ParserAsm::ContextSetStack, "", 0, 0);
		linkFinally = assembler->assembleProgramCounter(ParserAsm::ContextSetFinally, nullptr, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryReturn, currentContextReturn, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryThrow, currentContextThrow, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryContinue, currentContextContinue, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryBreak, currentContextBreak, 0, 0);

		retV = setVmFunctionFromFile(name, fileName);
		if (retV == VmParserError::None) {
			assembler->assemble(ParserAsm::LeaveContext, "", 0, 0);
			linkEntry = assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
			linkCallPC = assembler->assembleProgramCounter(ParserAsm::ContextSetPC, nullptr, 0, 0);

			linkPC = assembler->assemble(ParserAsm::Nop, "", 0, 0);
			assembler->linkProgramCounter(linkFinally, linkPC);

			linkEnd = assembler->assembleProgramCounter(ParserAsm::InstructionListExtractAndDelete, linkBegin, 0, 0);
			assembler->linkProgramCounter(linkBegin, linkEntry);
			assembler->linkProgramCounter(linkEnd, currentContextReturn);
			assembler->linkProgramCounter(linkCallPC, context->nextProgramCounter);
			context->nextProgramCounter = linkStart;

			assembler->resetLinks();
			return 0;
		};

		InstructionList instructionList;
		TDoubleEndedQueue<InstructionX>::Node *pcBegin = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(linkBegin);
		TDoubleEndedQueue<InstructionX>::Node *pcEnd = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(assembler->assemble(ParserAsm::Nop, "", 0, 0));
		context->instructionListExecutive->extractList(pcBegin, pcEnd);
		instructionList.setList(pcBegin, pcEnd);
		return retV;
	};

	int Executive::setVmFunctionFromStringX(InstructionContext *context, const char *name, const char *data, const char *tag) {
		ProgramCounter *linkBegin;
		ProgramCounter *linkStart;
		ProgramCounter *linkEnd;
		ProgramCounter *nextProgramCounter_;
		ProgramCounter *currentProgramCounter_;
		ProgramCounter *linkFinally;
		ProgramCounter *linkCallPC;
		ProgramCounter *linkPC;
		ProgramCounter *linkEntry;
		int retV;

		linkBegin = assembler->assembleProgramCounter(ParserAsm::Goto, nullptr, 0, 0);
		linkStart = assembler->assemble(ParserAsm::Nop, "", 0, 0); // don't use mark, next instruction can be removed

		assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
		assembler->assemble(ParserAsm::ContextSetStack, "", 0, 0);
		linkFinally = assembler->assembleProgramCounter(ParserAsm::ContextSetFinally, nullptr, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryReturn, currentContextReturn, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryThrow, currentContextThrow, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryContinue, currentContextContinue, 0, 0);
		assembler->assembleProgramCounter(ParserAsm::ContextSetTryBreak, currentContextBreak, 0, 0);

		retV = setVmFunctionFromString(name, data, tag);
		if (retV == VmParserError::None) {
			assembler->assemble(ParserAsm::LeaveContext, "", 0, 0);
			linkEntry = assembler->assemble(ParserAsm::EnterContext, "", 0, 0);
			linkCallPC = assembler->assembleProgramCounter(ParserAsm::ContextSetPC, nullptr, 0, 0);

			linkPC = assembler->assemble(ParserAsm::Nop, "", 0, 0);
			assembler->linkProgramCounter(linkFinally, linkPC);

			linkEnd = assembler->assembleProgramCounter(ParserAsm::InstructionListExtractAndDelete, linkBegin, 0, 0);
			assembler->linkProgramCounter(linkBegin, linkEntry);
			assembler->linkProgramCounter(linkEnd, currentContextReturn);
			assembler->linkProgramCounter(linkCallPC, context->nextProgramCounter);
			context->nextProgramCounter = linkStart;

			assembler->resetLinks();
			return 0;
		};

		InstructionList instructionList;
		TDoubleEndedQueue<InstructionX>::Node *pcBegin = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(linkBegin);
		TDoubleEndedQueue<InstructionX>::Node *pcEnd = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(assembler->assemble(ParserAsm::Nop, "", 0, 0));
		context->instructionListExecutive->extractList(pcBegin, pcEnd);
		instructionList.setList(pcBegin, pcEnd);
		return retV;
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
					fnContext.newMemory();

					fnContext->configPrintStackTraceLimit = configPrintStackTraceLimit;
					fnContext->executiveSuper = this;
					fnContext->init();
					fnContext->contextStack.newMemory();
					fnContext->contextStack->enterMaster(fnContext->functionContext);
					fnContext->functionContext->this_ = Context::getValueUndefined();
					fnContext->pcContext = fnContext->functionContext;
					fnContext->pcContext->pc_ = currentContextExit;
					fnContext->instructionListExecutive = assembler->instructionList.value();

					fnContext->error = InstructionError::None;
					fnContext->currentProgramCounter = nullptr;
					fnContext->nextProgramCounter = currentContextExit;

					TPointer<Variable> functionArguments;
					functionArguments = VariableArray::newVariable();
					(*(((VariableArray *)(functionArguments.value()))->value))[0] = fnParameters;

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
						return fnContext->returnValue;
					};
					if (error == InstructionError::Throw) {
						TPointer<Variable> throwValue;
						fnContext->pop(throwValue);
						TPointer<VariableStackTrace> stackTrace((VariableStackTrace *)VariableStackTrace::newVariable(fnContext->stackTrace, fnContext));
						stackTrace->configPrintStackTraceLimit = fnContext->configPrintStackTraceLimit;
						ExecutiveX::setStackTrace(stackTrace->toString());
						stackTrace.deleteMemory();
						fnContext->stackTrace.deleteMemory();
						throw Error((throwValue->getPropertyBySymbol(Context::getSymbol("message")))->toString());
					};
					throw Error(fnContext->errorInfo);
				};
			};
		};
		return nullptr;
	};

	void Executive::setFunction(const char *name, Variable *nativeFunction) {
		if (TIsType<VariableVmFunction>(nativeFunction)) {
			throw Error("setFunction require native function");
		};

		String buf;
		char bufx[2];
		int lk;
		int k;
		int isFirst;

		int isEmpty;
		int retV;
		int level;
		char sLevel[32];

		retV = 0;
		isFirst = 1;

		bufx[1] = 0;

		for (k = 0; name[k] != 0; ++k) {
			if (name[k] == '(') {
				break;
			};
			if (name[k] == '.') {

				if (isFirst) {
					isFirst = 0;
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

		assembler->assembleDirect(InstructionVmSetFunction, nativeFunction);
	};

	void Executive::compileStringX(const char *source, const char *tag) {
		if (compileString(source, tag) != 0) {
			char buf[2048];
			if (strlen(errorInfo.compileFileName)==0) {
				sprintf(buf, "Compile error on line %u",
				        errorInfo.compileLineNumber);
			} else {
				sprintf(buf, "Compile error in %s line %u",
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
		TMemory<Stack>::initMemory();
		TMemory<Array>::initMemory();
		ExecutiveContext::initMemory();
	};

	void Executive::endProcessing() {
		if (assembler) {
			assembler->endProcessing();
		};
		assembler.deleteMemory();
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

		instructionContext.deleteMemory();
		Context::deleteContext();
	};

};
