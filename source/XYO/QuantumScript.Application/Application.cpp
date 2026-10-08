// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Application/Application.hpp>
#include <XYO/QuantumScript.Application/CompileCpp.hpp>
#include <XYO/QuantumScript.Application/Copyright.hpp>
#include <XYO/QuantumScript.Application/License.hpp>
#include <XYO/QuantumScript.Application/Version.hpp>

namespace XYO::QuantumScript::Application {

	void Application::showUsage() {
		printf("Quantum Script\n");
		showVersion();
		printf("%s\n\n", QuantumScript::Application::Copyright::copyright());

		printf("%s",
		       "options:\n"
		       "    --license               show license\n"
		       "    script.js               execute script\n"
		       "    --cmd                   execute script, skip first 2 lines\n"
		       "    --run=\"code\"          run code\n"
		       "    --execution-time        show execution time\n"
		       "    --execution-time-cmd    --execution-time + --cmd\n"
		       "    --compile=file.qsbc     compile script to bytecode, do not execute\n"
		       "    --strip                 with --compile, without source file names, lines and function source\n"
		       "    --add=file              with --compile, add file as a unit included by name\n"
		       "    --add=name=file         with --compile, add file as a unit included as name\n"
		       "    --verify=file.qsbc      check a bytecode file, do not execute\n"
		       "    --compile-cpp=file.cpp  translate script or bytecode to C++, do not execute\n"
		       "    --cpp-no-inline         with --compile-cpp, every instruction calls the runtime\n"
		       "    script.qsbc             execute bytecode\n");
		printf("\n");
	};

	static int compileToBuffer(const String &fileIn, Buffer &output, bool strip, TDynamicArray<String> &unitList, size_t skipLines) {
		Executive &executive = ExecutiveX::getExecutive();
		CodeUnit unit;
		CodeUnitMap units;
		String error;
		size_t k;

		auto compile = [&](const String &fileName, CodeUnit &out, size_t skip) -> bool {
			int retV = executive.compileFileToUnit(fileName, out, skip);
			if (retV == VmParserError::None) {
				return true;
			};
			if (retV == VmParserError::FileNotFound) {
				printf("Error: file not found - %s\n", fileName.value());
				return false;
			};
			printf("Error: compile error in %s line %u\n", fileName.value(), executive.errorInfo.compileLineNumber);
			return false;
		};

		if (!compile(fileIn, unit, skipLines)) {
			return 1;
		};

		for (k = 0; k < unitList.length(); ++k) {
			String name = unitList[k];
			String fileName = unitList[k];
			size_t index;
			if (name.indexOf("=", 0, index)) {
				fileName = name.substring(index + 1);
				name = name.substring(0, index);
			};
			TPointer<CodeUnit> namedUnit;
			if (units.get(name, namedUnit)) {
				printf("Error: unit added twice - %s\n", name.value());
				return 1;
			};
			namedUnit.newMemory();
			if (!compile(fileName, *(namedUnit.value()), 0)) {
				return 1;
			};
			units.set(name, namedUnit);
		};

		if (!Bytecode::save(unit, output, error, !strip, &units)) {
			printf("Error: %s - %s\n", error.value(), fileIn.value());
			return 1;
		};
		return 0;
	};

	static int compileToBytecode(const String &fileIn, const String &fileOut, bool strip, TDynamicArray<String> &unitList, size_t skipLines) {
		Buffer output;
		int retV = compileToBuffer(fileIn, output, strip, unitList, skipLines);
		if (retV != 0) {
			return retV;
		};
		if (!Shell::filePutContents(fileOut, output)) {
			printf("Error: unable to write - %s\n", fileOut.value());
			return 1;
		};
		return 0;
	};

	// The C++ namespace of the generated code, from the name of the output file
	static String namespaceFromFileName(const String &fileName) {
		String name = Shell::getFileBasename(Shell::getFileName(fileName));
		String retV("QuantumScriptCompiled_");
		size_t k;
		for (k = 0; k < name.length(); ++k) {
			char ch = name[k];
			if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')) {
				retV << ch;
				continue;
			};
			retV << '_';
		};
		return retV;
	};

	// Script or bytecode to C++
	static int compileToCpp(const String &fileIn, const String &fileOut, bool strip, TDynamicArray<String> &unitList, size_t skipLines, bool withInline) {
		Buffer bytecode;
		String output;
		String error;
		if (Bytecode::isBytecodeFile(fileIn)) {
			if (!Shell::fileGetContents(fileIn, bytecode)) {
				printf("Error: file not found - %s\n", fileIn.value());
				return 1;
			};
		} else {
			int retV = compileToBuffer(fileIn, bytecode, strip, unitList, skipLines);
			if (retV != 0) {
				return retV;
			};
		};
		if (!CompileCpp::generate(bytecode.buffer, bytecode.length, Shell::getFileName(fileIn), namespaceFromFileName(fileOut), withInline, output, error)) {
			printf("Error: %s - %s\n", error.value(), fileIn.value());
			return 1;
		};
		if (!Shell::filePutContents(fileOut, output)) {
			printf("Error: unable to write - %s\n", fileOut.value());
			return 1;
		};
		return 0;
	};

	static int verifyBytecode(const String &fileName) {
		CodeUnit unit;
		CodeUnitMap units;
		String error;
		if (!Bytecode::loadFile(fileName, unit, error, &units)) {
			printf("Error: %s - %s\n", error.value(), fileName.value());
			return 1;
		};
		size_t unitCount = 0;
		CodeUnitMap::Node *scan;
		for (scan = units.begin(); scan; scan = scan->successor()) {
			++unitCount;
		};
		printf("Ok: %s - " XYO_PLATFORM_FORMAT_SIZET " instructions, " XYO_PLATFORM_FORMAT_SIZET " units\n", fileName.value(), unit.length(), unitCount);
		return 0;
	};

	void Application::showLicense() {
		printf("%s", QuantumScript::Application::License::license().c_str());
	};

	void Application::showVersion() {
		printf("version %s build %s [%s]\n", QuantumScript::Application::Version::version(), QuantumScript::Application::Version::build(), QuantumScript::Application::Version::datetime());
	};

	void Application::initMemory() {
		String::initMemory();
		TDynamicArray<String>::initMemory();
	};

	void Application::initExecutive(Executive *executive) {
		Extension::Console::registerInternalExtension(executive);
	};

	int Application::main(int cmdN, char *cmdS[]) {
		int i;
		String opt;
		size_t optIndex;
		String optValue;
		TDynamicArray<String> cmdLine;

		bool executionTime = false;
		uint64_t beginTimestampInMilliseconds;
		uint64_t endTimestampInMilliseconds;
		uint64_t intervalTimestampInMilliseconds;
		String fileIn;
		bool isCmd = false;
		bool runCode = false;
		String code;
		String compileOut;
		bool compileStrip = false;
		TDynamicArray<String> compileUnits;
		String compileCppOut;
		bool compileCppInline = true;

		for (i = 1; i < cmdN; ++i) {
			if (StringCore::beginWith(cmdS[i], "@")) {
				String content;
				if (Shell::fileGetContents(&cmdS[i][1], content)) {
					XYO::System::ShellArguments shellArguments;
					int m;
					shellArguments.set(content);
					for (m = 0; m < shellArguments.cmdN; ++m) {
						cmdLine.push(shellArguments.cmdS[m]);
					};
					continue;
				};
				printf("Error: file not found - %s\n", &cmdS[i][1]);
				return 1;
			};
			cmdLine.push(cmdS[i]);
		};

		for (i = 0; i < cmdLine.length(); ++i) {
			if (StringCore::beginWith(cmdLine[i], "--")) {
				opt = cmdLine[i].index(2);
				optValue = "";
				if (opt.indexOf("=", 0, optIndex)) {
					optValue = opt.substring(optIndex + 1);
					opt = opt.substring(0, optIndex);
				};
				if (opt == "usage") {
					showUsage();
					return 0;
				};
				if (opt == "license") {
					showLicense();
					return 0;
				};
				if (opt == "version") {
					showVersion();
					return 0;
				};
				if (opt == "execution-time") {
					executionTime = true;
					continue;
				};
				if (opt == "cmd") {
					isCmd = true;
					continue;
				};
				if (opt == "execution-time-cmd") {
					executionTime = true;
					isCmd = true;
					continue;
				};
				if (opt == "run") {
					runCode = true;
					code = optValue;
					continue;
				};
				if (opt == "compile") {
					compileOut = optValue;
					if (compileOut.length() == 0) {
						printf("Error: no bytecode file specified\n");
						return 1;
					};
					continue;
				};
				if (opt == "verify") {
					if (optValue.length() == 0) {
						printf("Error: no bytecode file specified\n");
						return 1;
					};
					if (!ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
						printf("%s\n", (ExecutiveX::getError()).value());
						ExecutiveX::endProcessing();
						return 1;
					};
					int retV = verifyBytecode(optValue);
					ExecutiveX::endProcessing();
					return retV;
				};
				if (opt == "compile-cpp") {
					compileCppOut = optValue;
					if (compileCppOut.length() == 0) {
						printf("Error: no C++ file specified\n");
						return 1;
					};
					continue;
				};
				if (opt == "cpp-no-inline") {
					compileCppInline = false;
					continue;
				};
				if (opt == "strip") {
					compileStrip = true;
					continue;
				};
				if (opt == "add") {
					compileUnits.push(optValue);
					continue;
				};
				continue;
			};
			if (fileIn.length() == 0) {
				fileIn = cmdLine[i];
			};
		};

		// ---

		if (!runCode) {
			if (fileIn.length() == 0) {
				showUsage();
				return 0;
			};
		} else {
			if (code.length() == 0) {
				printf("Error: No code specified!");
				return 1;
			};
		};

		if (compileOut.length() > 0) {
			if (fileIn.length() == 0) {
				printf("Error: no script specified\n");
				return 1;
			};
			if (!ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
				printf("%s\n", (ExecutiveX::getError()).value());
				ExecutiveX::endProcessing();
				return 1;
			};
			int retV = compileToBytecode(fileIn, compileOut, compileStrip, compileUnits, isCmd ? 2 : 0);
			ExecutiveX::endProcessing();
			return retV;
		};

		if (compileCppOut.length() > 0) {
			if (fileIn.length() == 0) {
				printf("Error: no script specified\n");
				return 1;
			};
			if (!ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
				printf("%s\n", (ExecutiveX::getError()).value());
				ExecutiveX::endProcessing();
				return 1;
			};
			int retV = compileToCpp(fileIn, compileCppOut, compileStrip, compileUnits, isCmd ? 2 : 0, compileCppInline);
			ExecutiveX::endProcessing();
			return retV;
		};

		if (executionTime) {
			beginTimestampInMilliseconds = DateTime::timestampInMilliseconds();
		};

		if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
			if (runCode) {
				if (ExecutiveX::executeString(code)) {
					int exitCode = ExecutiveX::getExitCode();
					ExecutiveX::endProcessing();
					if (executionTime) {
						endTimestampInMilliseconds = DateTime::timestampInMilliseconds();
						intervalTimestampInMilliseconds = endTimestampInMilliseconds - beginTimestampInMilliseconds;
						printf("Execution time: " XYO_PLATFORM_FORMAT_SIZET " ms\n", (size_t)intervalTimestampInMilliseconds);
					};
					return exitCode;
				};
			} else {
				ExecutiveX::includePath(Shell::getFilePath(fileIn));
				if (isCmd) {
					if (ExecutiveX::executeFileSkipLines(fileIn, 2)) {
						int exitCode = ExecutiveX::getExitCode();
						ExecutiveX::endProcessing();
						if (executionTime) {
							endTimestampInMilliseconds = DateTime::timestampInMilliseconds();
							intervalTimestampInMilliseconds = endTimestampInMilliseconds - beginTimestampInMilliseconds;
							printf("Execution time: " XYO_PLATFORM_FORMAT_SIZET " ms\n", (size_t)intervalTimestampInMilliseconds);
						};
						return exitCode;
					};

					fflush(stdout);
					printf("%s\n", (ExecutiveX::getError()).value());
					printf("%s", (ExecutiveX::getStackTrace()).value());
					fflush(stdout);

					ExecutiveX::endProcessing();
					return 1;
				};

				if (ExecutiveX::executeFile(fileIn)) {
					int exitCode = ExecutiveX::getExitCode();
					ExecutiveX::endProcessing();
					if (executionTime) {
						endTimestampInMilliseconds = DateTime::timestampInMilliseconds();
						intervalTimestampInMilliseconds = endTimestampInMilliseconds - beginTimestampInMilliseconds;
						printf("Execution time: " XYO_PLATFORM_FORMAT_SIZET " ms\n", (size_t)intervalTimestampInMilliseconds);
					};
					return exitCode;
				};
			};
		};

		fflush(stdout);
		printf("%s\n", (ExecutiveX::getError()).value());
		printf("%s", (ExecutiveX::getStackTrace()).value());
		fflush(stdout);

		ExecutiveX::endProcessing();
		return 1;
	};
};

#ifndef XYO_QUANTUMSCRIPT_APPLICATION_LIBRARY
XYO_APPLICATION_MAIN(XYO::QuantumScript::Application::Application);
#endif
