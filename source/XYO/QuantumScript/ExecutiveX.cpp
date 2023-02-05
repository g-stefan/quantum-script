// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript/ExecutiveX.hpp>
#include <XYO/QuantumScript/ParserAsm.hpp>
#include <XYO/QuantumScript/LibStd.hpp>
#include <XYO/QuantumScript/Variable.hpp>

#include <XYO/QuantumScript/VariableString.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>

#include <XYO/QuantumScript/ExecutiveContext.hpp>
#include <XYO/QuantumScript/InstructionX.hpp>
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

namespace XYO::QuantumScript {

	class ExecutiveX_ {
		public:
			Executive executive;
			String error;
			String stackTrace;
			TPointer<Variable> returnValue;

			ExecutiveX_();
			~ExecutiveX_();

			static void initMemory();

			void endProcessing();
	};

	ExecutiveX_::ExecutiveX_() {
		returnValue = Context::getValueUndefined();
	};

	ExecutiveX_::~ExecutiveX_() {
		executive.endProcessing();
		returnValue.deleteMemory();
	};

	void ExecutiveX_::initMemory() {
		String::initMemory();
		Error::initMemory();
		Executive::initMemory();
	};

	void ExecutiveX_::endProcessing() {
		executive.endProcessing();
		returnValue.deleteMemory();
	};

	bool ExecutiveX::initExecutive(int cmdN, char **cmdS, QuantumScriptInitExecutiveProc applicationInitExecutive) {
		Executive &executive = (TSingletonThread<ExecutiveX_>::getValue())->executive;
		String &error = (TSingletonThread<ExecutiveX_>::getValue())->error;
		String pathApplication;

		pathApplication = Shell::getExecutablePath();
		executive.includePath->push(pathApplication);

		executive.mainCmdN = cmdN;
		executive.mainCmdS = cmdS;
		if (!Shell::realPath(pathApplication, executive.pathExecutable)) {
			return false;
		};
		executive.applicationInitExecutive = applicationInitExecutive;

		try {

			executive.initExecutive();
			executive.compileEnd();

			int errorExecute = executive.execute();			
			if (errorExecute == InstructionError::None) {				
				return true;
			};
			if (errorExecute == InstructionError::Error) {						
				return false;
			};
			if (errorExecute == InstructionError::Throw) {				
				TPointer<Variable> throwValue;
				executive.instructionContext->pop(throwValue);
				TPointer<VariableStackTrace> stackTrace((VariableStackTrace *)VariableStackTrace::newVariable(executive.instructionContext->stackTrace, executive.instructionContext));
				stackTrace->configPrintStackTraceLimit = executive.instructionContext->configPrintStackTraceLimit;						
				ExecutiveX::setStackTrace(stackTrace->toString());
				stackTrace.deleteMemory();
				executive.instructionContext->stackTrace.deleteMemory();

				error = "Error: ";
				error += (throwValue->getPropertyBySymbol(Context::getSymbol("message")))->toString();
				return false;				
			};
			
			error = "Error: ";
			error += executive.instructionContext->errorInfo;
			return false;			

		} catch (const Error &e) {
			error = "Error: ";
			error << (const_cast<Error &>(e)).getMessage();
		} catch (const std::exception &e) {
			error = "Error: ";
			error << e.what();
		} catch (...) {
			error = "Error: Unknown\n";
		};

		return false;
	};

	String ExecutiveX::getError() {
		return (TSingletonThread<ExecutiveX_>::getValue())->error;
	};

	void ExecutiveX::setStackTrace(String stackTrace) {
		(TSingletonThread<ExecutiveX_>::getValue())->stackTrace = stackTrace;
	};

	String ExecutiveX::getStackTrace() {
		return (TSingletonThread<ExecutiveX_>::getValue())->stackTrace;
	};

	Executive &ExecutiveX::getExecutive() {
		return (TSingletonThread<ExecutiveX_>::getValue())->executive;
	};

	bool ExecutiveX::executeFile(const char *fileName) {
		String &error = (TSingletonThread<ExecutiveX_>::getValue())->error;

		try {
			TPointer<VariableArray> arguments(VariableArray::newArray());
			(arguments->index(0)) = VariableString::newVariable(fileName);
			TPointer<Variable> script_((Context::getGlobalObject())->getPropertyBySymbol(Context::getSymbol("Script")));
			(TSingletonThread<ExecutiveX_>::getValue())->returnValue = (script_->getPropertyBySymbol(Context::getSymbol("include")))->functionApply(script_, arguments);
			return true;
		} catch (const Error &e) {
			error = "Error: ";
			error << (const_cast<Error &>(e)).getMessage();
		} catch (const std::exception &e) {
			error = "Error: ";
			error << e.what();
		} catch (...) {
			error = "Error: Unknown\n";
		};

		return false;
	};

	bool ExecutiveX::executeString(const char *strSource) {
		String &error = (TSingletonThread<ExecutiveX_>::getValue())->error;

		if (strSource == nullptr) {
			error = "Error: Invalid source\n";
			return false;
		};

		try {
			TPointer<VariableArray> arguments(VariableArray::newArray());
			(arguments->index(0)) = VariableString::newVariable(strSource);
			TPointer<Variable> script_((Context::getGlobalObject())->getPropertyBySymbol(Context::getSymbol("Script")));
			(TSingletonThread<ExecutiveX_>::getValue())->returnValue = (script_->getPropertyBySymbol(Context::getSymbol("execute")))->functionApply(script_, arguments);
			return true;

		} catch (const Error &e) {
			error = "Error: ";
			error << (const_cast<Error &>(e)).getMessage();
		} catch (const std::exception &e) {
			error = "Error: ";
			error << e.what();
		} catch (...) {
			error = "Error: Unknown\n";
		};

		return false;
	};

	TPointer<Variable> ExecutiveX::returnValue() {
		return (TSingletonThread<ExecutiveX_>::getValue())->returnValue;
	};

	void ExecutiveX::endProcessing() {
		(TSingletonThread<ExecutiveX_>::getValue())->endProcessing();
	};

	void ExecutiveX::setIncludeSource(String fileName, String source) {
		Executive &executive = (TSingletonThread<ExecutiveX_>::getValue())->executive;
		executive.setIncludeSource(fileName, source);
	};

	void ExecutiveX::removeIncludeSource(String fileName) {
		Executive &executive = (TSingletonThread<ExecutiveX_>::getValue())->executive;
		executive.removeIncludeSource(fileName);
	};

	bool ExecutiveX::executeFileSkipLines(const char *fileName, size_t skipLines) {
		String &error = (TSingletonThread<ExecutiveX_>::getValue())->error;

		try {
			TPointer<VariableArray> arguments(VariableArray::newArray());
			(arguments->index(0)) = VariableString::newVariable(fileName);
			(arguments->index(1)) = Context::getValueUndefined();
			(arguments->index(2)) = VariableNumber::newVariable(skipLines);
			TPointer<Variable> script_((Context::getGlobalObject())->getPropertyBySymbol(Context::getSymbol("Script")));
			(TSingletonThread<ExecutiveX_>::getValue())->returnValue = (script_->getPropertyBySymbol(Context::getSymbol("includeSkipLines")))->functionApply(script_, arguments);
			return true;
		} catch (const Error &e) {
			error = "Error: ";
			error << (const_cast<Error &>(e)).getMessage();
		} catch (const std::exception &e) {
			error = "Error: ";
			error << e.what();
		} catch (...) {
			error = "Error: Unknown\n";
		};

		return false;
	};

	bool ExecutiveX::executeStringSkipLines(const char *strSource, size_t skipLines) {
		String &error = (TSingletonThread<ExecutiveX_>::getValue())->error;

		if (strSource == nullptr) {
			error = "Error: Invalid source\n";
			return false;
		};

		try {
			TPointer<VariableArray> arguments(VariableArray::newArray());
			(arguments->index(0)) = VariableString::newVariable(strSource);
			(arguments->index(1)) = Context::getValueUndefined();
			(arguments->index(2)) = VariableNumber::newVariable(skipLines);
			TPointer<Variable> script_((Context::getGlobalObject())->getPropertyBySymbol(Context::getSymbol("Script")));
			(TSingletonThread<ExecutiveX_>::getValue())->returnValue = (script_->getPropertyBySymbol(Context::getSymbol("executeSkipLines")))->functionApply(script_, arguments);
			return true;

		} catch (const Error &e) {
			error = "Error: ";
			error << (const_cast<Error &>(e)).getMessage();
		} catch (const std::exception &e) {
			error = "Error: ";
			error << e.what();
		} catch (...) {
			error = "Error: Unknown\n";
		};

		return false;
	};

	void ExecutiveX::includePath(const char *includePath_) {
		(TSingletonThread<ExecutiveX_>::getValue())->executive.includePath->push(includePath_);
	};

	bool ExecutiveX::isExit() {
		return (TSingletonThread<ExecutiveX_>::getValue())->executive.isExit;
	};

	int ExecutiveX::getExitCode() {
		return (TSingletonThread<ExecutiveX_>::getValue())->executive.exitCode;
	};

};
