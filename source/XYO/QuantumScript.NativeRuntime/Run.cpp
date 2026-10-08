// Quantum Script Native Runtime
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include <XYO/QuantumScript.NativeRuntime/Run.hpp>

namespace XYO::QuantumScript::NativeRuntime {

	TPointer<Variable> load(const ModuleInfo &info) {
		if (info.abi != XYO_QUANTUMSCRIPT_NATIVERUNTIME_ABI) {
			throw Error("Compiled code does not match the native runtime, generate it again");
		};
		if (info.unitCount == 0) {
			throw Error("Compiled code without units");
		};
		TPointer<Module> module(Module::newModule(info));
		size_t k;
		for (k = 1; k < info.unitCount; ++k) {
			module->executive->setIncludeFunction(info.unit[k].name, newFunction(module, nullptr, nullptr, nullptr, info.unit[k].procedure));
		};
		return newFunction(module, nullptr, nullptr, nullptr, info.unit[0].procedure);
	};

	bool execute(const ModuleInfo &info, String &error, String &stackTrace) {
		error = "";
		stackTrace = "";
		try {
			TPointer<Variable> main(load(info));
			TPointer<VariableArray> arguments(VariableArray::newArray());
			TPointer<Variable> script((Context::getGlobalObject())->getPropertyBySymbol(Context::getSymbol("Script")));
			main->functionApply(script, arguments);
			return true;
		} catch (Exit &) {
			return true;
		} catch (Exception &e) {
			error = "Error: ";
			error << e.getMessage();
			if (e.stackTrace) {
				TPointer<VariableStackTrace> stackTrace_((VariableStackTrace *)VariableStackTrace::newVariable(e.stackTrace, nullptr));
				stackTrace_->configPrintStackTraceLimit = (ExecutiveX::getExecutive()).configPrintStackTraceLimit;
				stackTrace = stackTrace_->toString();
			};
		} catch (Error &e) {
			error = "Error: ";
			error << e.getMessage();
		} catch (const std::exception &e) {
			error = "Error: ";
			error << e.what();
		} catch (...) {
			error = "Error: Unknown";
		};
		return false;
	};

	int run(const ModuleInfo &info, int cmdN, char *cmdS[], QuantumScriptInitExecutiveProc initExecutive) {
		if (!ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
			printf("%s\n", (ExecutiveX::getError()).value());
			ExecutiveX::endProcessing();
			return 1;
		};
		String error;
		String stackTrace;
		if (execute(info, error, stackTrace)) {
			int exitCode = ExecutiveX::getExitCode();
			ExecutiveX::endProcessing();
			return exitCode;
		};
		fflush(stdout);
		printf("%s\n", error.value());
		printf("%s", stackTrace.value());
		fflush(stdout);
		ExecutiveX::endProcessing();
		return 1;
	};

};
