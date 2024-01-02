// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Application/Application.hpp>
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
		       "    --execution-time-cmd    --execution-time + --cmd\n");
		printf("\n");
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

		for (i = 1; i < cmdN; ++i) {
			if (StringCore::beginWith(cmdS[i], "@")) {
				String content;
				if (Shell::fileGetContents(&cmdS[i][1], content)) {
					int cmdNX;
					char **cmdSX;
					int m;
					Shell::mainArgsSet(content, cmdNX, cmdSX);
					for (m = 0; m < cmdNX; ++m) {
						cmdLine.push(cmdSX[m]);
					};
					Shell::mainArgsDelete(cmdNX, cmdSX);
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

		if (executionTime) {
			beginTimestampInMilliseconds = DateTime::timestampInMilliseconds();
		};

		if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
			if (runCode) {
				if (ExecutiveX::executeString(code)) {
					ExecutiveX::endProcessing();
					if (executionTime) {
						endTimestampInMilliseconds = DateTime::timestampInMilliseconds();
						intervalTimestampInMilliseconds = endTimestampInMilliseconds - beginTimestampInMilliseconds;
						printf("Execution time: " XYO_FORMAT_SIZET " ms\n", (size_t)intervalTimestampInMilliseconds);
					};
					return 0;
				};
			} else {
				ExecutiveX::includePath(Shell::getFilePath(fileIn));
				if (isCmd) {
					if (ExecutiveX::executeFileSkipLines(fileIn, 2)) {
						ExecutiveX::endProcessing();
						if (executionTime) {
							endTimestampInMilliseconds = DateTime::timestampInMilliseconds();
							intervalTimestampInMilliseconds = endTimestampInMilliseconds - beginTimestampInMilliseconds;
							printf("Execution time: " XYO_FORMAT_SIZET " ms\n", (size_t)intervalTimestampInMilliseconds);
						};
						return 0;
					};

					fflush(stdout);
					printf("%s\n", (ExecutiveX::getError()).value());
					printf("%s", (ExecutiveX::getStackTrace()).value());
					fflush(stdout);

					ExecutiveX::endProcessing();
					return 1;
				};

				if (ExecutiveX::executeFile(fileIn)) {
					ExecutiveX::endProcessing();
					if (executionTime) {
						endTimestampInMilliseconds = DateTime::timestampInMilliseconds();
						intervalTimestampInMilliseconds = endTimestampInMilliseconds - beginTimestampInMilliseconds;
						printf("Execution time: " XYO_FORMAT_SIZET " ms\n", (size_t)intervalTimestampInMilliseconds);
					};
					return 0;
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
