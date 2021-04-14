//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/timeb.h>

#include <stdexcept>

#include "quantum-script-executivex.hpp"
#include "quantum-script-parserasm.hpp"
#include "quantum-script-libstd.hpp"
#include "quantum-script-variable.hpp"

#include "quantum-script-variablestring.hpp"
#include "quantum-script-variablenumber.hpp"

#include "quantum-script-executivecontext.hpp"
#include "quantum-script-instructionx.hpp"
#include "quantum-script-variablevmfunction.hpp"
#include "quantum-script-instructioncontext.hpp"
#include "quantum-script-variablestacktrace.hpp"
#include "quantum-script-variableargumentlevel.hpp"
#include "quantum-script-variablereferenceobject.hpp"
#include "quantum-script-variableoperator21.hpp"
#include "quantum-script-variableoperator22.hpp"
#include "quantum-script-variableoperator23.hpp"
#include "quantum-script-variableoperator31.hpp"
#include "quantum-script-variablenativevmfunction.hpp"
#include "quantum-script-variablevmprogramcounter.hpp"

#include "quantum-script-arrayiteratorkey.hpp"
#include "quantum-script-arrayiteratorvalue.hpp"
#include "quantum-script-objectiteratorkey.hpp"
#include "quantum-script-objectiteratorvalue.hpp"

namespace Quantum {
	namespace Script {


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
			String pathMain;

			pathMain = Shell::getExecutablePath();
			executive.includePath->push(pathMain);

			executive.mainCmdN = cmdN;
			executive.mainCmdS = cmdS;
			if(!Shell::realPath(pathMain, executive.pathExecutable)) {
				return false;
			};
			executive.applicationInitExecutive = applicationInitExecutive;

			try {

				executive.initExecutive();
				executive.compileEnd();
				if (executive.execute() == 0) {
					return true;
				} else {
					error = "Error: Internal virtual machine execution error.";
					return false;
				};

			} catch(const Error &e) {
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
				(arguments->index(0))=VariableString::newVariable(fileName);
				Variable &script_((Context::getGlobalObject())->operatorReference(Context::getSymbol("Script")));
				(TSingletonThread<ExecutiveX_>::getValue())->returnValue = (script_.operatorReference(Context::getSymbol("include"))).functionApply(&script_, arguments);
				return true;
			} catch(const Error &e) {
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

			if(strSource == nullptr) {
				error = "Error: Invalid source\n";
				return false;
			};

			try {
				TPointer<VariableArray> arguments(VariableArray::newArray());
				(arguments->index(0))=VariableString::newVariable(strSource);
				Variable &script_((Context::getGlobalObject())->operatorReference(Context::getSymbol("Script")));
				(TSingletonThread<ExecutiveX_>::getValue())->returnValue = (script_.operatorReference(Context::getSymbol("execute"))).functionApply(&script_, arguments);
				return true;

			} catch(const Error &e) {
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
				(arguments->index(0))=VariableString::newVariable(fileName);
				(arguments->index(1))=Context::getValueUndefined();
				(arguments->index(2))=VariableNumber::newVariable(skipLines);
				Variable &script_((Context::getGlobalObject())->operatorReference(Context::getSymbol("Script")));
				(TSingletonThread<ExecutiveX_>::getValue())->returnValue = (script_.operatorReference(Context::getSymbol("includeSkipLines"))).functionApply(&script_, arguments);
				return true;
			} catch(const Error &e) {
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

			if(strSource == nullptr) {
				error = "Error: Invalid source\n";
				return false;
			};

			try {
				TPointer<VariableArray> arguments(VariableArray::newArray());
				(arguments->index(0))=VariableString::newVariable(strSource);
				(arguments->index(1))=Context::getValueUndefined();
				(arguments->index(2))=VariableNumber::newVariable(skipLines);
				Variable &script_((Context::getGlobalObject())->operatorReference(Context::getSymbol("Script")));
				(TSingletonThread<ExecutiveX_>::getValue())->returnValue = (script_.operatorReference(Context::getSymbol("executeSkipLines"))).functionApply(&script_, arguments);
				return true;

			} catch(const Error &e) {
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

	};
};



