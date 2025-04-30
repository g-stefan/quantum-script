// Quantum Script Extension Console
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/Console/Library.hpp>
#include <XYO/QuantumScript.Extension/Console/Copyright.hpp>
#include <XYO/QuantumScript.Extension/Console/License.hpp>
#include <XYO/QuantumScript.Extension/Console/Version.hpp>

namespace XYO::QuantumScript::Extension::Console {

	static TPointer<Variable> write(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- console-write\n");
#endif

		printf("%s", ((arguments->index(0))->toString()).value());

		return Context::getValueUndefined();
	};

	static TPointer<Variable> writeLn(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- console-write-ln\n");
#endif

		printf("%s\r\n", ((arguments->index(0))->toString()).value());

		return Context::getValueUndefined();
	};

	static TPointer<Variable> readLn(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- console-read-ln\n");
#endif
		String retV;

		Number ln;
		size_t readLn;
		size_t readToLn;
		size_t readTotal;
		size_t k;
		char buffer[2];

		buffer[1] = 0;

		ln = (arguments->index(0))->toNumber();
		if (isnan(ln) || isinf(ln) || signbit(ln)) {
			return Context::getValueUndefined();
		};

		readToLn = (size_t)(ln);
		readTotal = 0;
		if (readToLn < 1) {
			return VariableString::newVariable("");
		};
		for (;;) {
			readLn = fread(buffer, sizeof(uint8_t), 1, stdin);
			if (readLn > 0) {

				if (buffer[0] == '\r') {
					if (readTotal + 1 >= readToLn) {
						retV.concatenate("\r", 1);
						return VariableString::newVariable(retV);
					};
					readLn = fread(buffer, sizeof(uint8_t), 1, stdin);
					if (readLn > 0) {
						if (buffer[0] == '\n') {
							retV.concatenate("\r", 1);
							retV.concatenate("\n", 1);
							return VariableString::newVariable(retV);
							break;
						};
						retV.concatenate(buffer, 1);
						readTotal += 2;
						if (readTotal >= readToLn) {
							return VariableString::newVariable(retV);
						};
						continue;
					};

					retV.concatenate("\r", 1);
					// end of file
					return VariableString::newVariable(retV);
				};

				if (buffer[0] == '\n') {
					retV.concatenate("\n", 1);
					return VariableString::newVariable(retV);
				};

				retV.concatenate(buffer, 1);
				readTotal++;
				if (readTotal >= readToLn) {
					return VariableString::newVariable(retV);
				};
				continue;
			};
			// connection interrupted - 0 to read ...
			if (readTotal == 0) {
				break;
			};
			// end of file
			return VariableString::newVariable(retV);
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> keyHit(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- console-key-hit\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Console::keyHit());
	};

	static TPointer<Variable> getChar(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- console-get-char\n");
#endif
		char buf[2];
		buf[0] = XYO::System::Console::getChar();
		buf[1] = 0;
		return VariableString::newVariable((&buf[0]));
	};

	static TPointer<Variable> getKey(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- console-get-key\n");
#endif
		if (XYO::System::Console::keyHit()) {
			char buf[2];
			buf[0] = XYO::System::Console::getChar();
			buf[1] = 0;
			return VariableString::newVariable((&buf[0]));
		};

		return VariableString::newVariable("");
	};

	void registerInternalExtension(Executive *executive) {
		executive->registerInternalExtension("Console", initExecutive);
	};

	void initExecutive(Executive *executive, void *extensionId) {
#ifdef XYO_PLATFORM_OS_LINUX
		setbuf(stdout, nullptr);
#endif

		String info = "Console\r\n";
		info << License::shortLicense().c_str();

		executive->setExtensionName(extensionId, "Console");
		executive->setExtensionInfo(extensionId, info);
		executive->setExtensionVersion(extensionId, Extension::Console::Version::versionWithBuild());
		executive->setExtensionPublic(extensionId, true);

		executive->compileStringX("var Console={};");

		executive->setFunction2("Console.write(str)", write);
		executive->setFunction2("Console.writeLn(str)", writeLn);
		executive->setFunction2("Console.readLn(max)", readLn);
		executive->setFunction2("Console.keyHit()", keyHit);
		executive->setFunction2("Console.getChar()", getChar);
		executive->setFunction2("Console.getKey()", getKey);
	};

};

#ifdef XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY
extern "C" XYO_QUANTUMSCRIPT_EXTENSION_CONSOLE_EXPORT void quantumScriptExtension(XYO::QuantumScript::Executive *executive, void *extensionId) {
	XYO::QuantumScript::Extension::Console::initExecutive(executive, extensionId);
};
#endif
