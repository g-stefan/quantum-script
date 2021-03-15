//
// Quantum Script Extension Console
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-extension-console.hpp"
#include "quantum-script-extension-console-license.hpp"
#ifndef QUANTUM_SCRIPT_EXTENSION_CONSOLE_NO_VERSION
#include "quantum-script-extension-console-version.hpp"
#endif

//#define QUANTUM_SCRIPT_VM_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Console {

				using namespace XYO;
				using namespace Quantum::Script;

				static TPointer<Variable> write(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- console-write\n");
#endif

					printf("%s", ((arguments->index(0))->toString()).value());

					return Context::getValueUndefined();
				};


				static TPointer<Variable> writeLn(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- console-write-ln\n");
#endif

					printf("%s\r\n", ((arguments->index(0))->toString()).value());

					return Context::getValueUndefined();
				};

				static TPointer<Variable> readLn(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- console-read-ln\n");
#endif
					String  retV;

					Number ln;
					size_t readLn;
					size_t readToLn;
					size_t readTotal;
					size_t k;
					char buffer[2];

					buffer[1] = 0;

					ln = (arguments->index(0))->toNumber();
					if(isnan(ln) || isinf(ln) || signbit(ln)) {
						return Context::getValueUndefined();
					};

					readToLn = (size_t)(ln);
					readTotal = 0;
					if(readToLn < 1) {
						return VariableString::newVariable("");
					};
					for(;;) {
						readLn = fread(buffer, sizeof(uint8_t), 1, stdin);
						if(readLn > 0) {

							if(buffer[0] == '\r') {
								if(readTotal + 1 >= readToLn) {
									retV.concatenate("\r", 1);
									return VariableString::newVariable(retV);
								};
								readLn = fread(buffer, sizeof(uint8_t), 1, stdin);
								if(readLn > 0) {
									if(buffer[0] == '\n') {
										retV.concatenate("\r", 1);
										retV.concatenate("\n", 1);
										return VariableString::newVariable(retV);
										break;
									};
									retV.concatenate(buffer, 1);
									readTotal += 2;
									if(readTotal >= readToLn) {
										return VariableString::newVariable(retV);
									};
									continue;
								};

								retV.concatenate("\r", 1);
								//end of file
								return VariableString::newVariable(retV);
							};

							if(buffer[0] == '\n') {
								retV.concatenate("\n", 1);
								return VariableString::newVariable(retV);
							};

							retV.concatenate(buffer, 1);
							readTotal++;
							if(readTotal >= readToLn) {
								return VariableString::newVariable(retV);
							};
							continue;
						};
						// connection interrupted - 0 to read ...
						if(readTotal == 0) {
							break;
						};
						//end of file
						return VariableString::newVariable(retV);
					};

					return Context::getValueUndefined();
				};

				static TPointer<Variable> keyHit(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- console-key-hit\n");
#endif
					return VariableBoolean::newVariable(XYO::Console::keyHit());
				};

				static TPointer<Variable> getChar(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- console-get-char\n");
#endif
					char buf[2];
					buf[0] = XYO::Console::getChar();
					buf[1] = 0;
					return VariableString::newVariable((&buf[0]));
				};

				static TPointer<Variable> getKey(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- console-get-key\n");
#endif
					if(XYO::Console::keyHit()) {
						char buf[2];
						buf[0] = XYO::Console::getChar();
						buf[1] = 0;
						return VariableString::newVariable((&buf[0]));
					};

					return VariableString::newVariable("");
				};

				void registerInternalExtension(Executive *executive) {
					executive->registerInternalExtension("Console", initExecutive);
				};


				void initExecutive(Executive *executive, void *extensionId) {
#ifdef XYO_OS_UNIX
					setbuf(stdout, nullptr);
#endif

					String info = "Console\r\n";
					info << License::shortContent();

					executive->setExtensionName(extensionId, "Console");
					executive->setExtensionInfo(extensionId, info);
#ifndef QUANTUM_SCRIPT_EXTENSION_CONSOLE_NO_VERSION
					executive->setExtensionVersion(extensionId, Extension::Console::Version::versionWithBuild());
#endif
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
		};
	};
};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" QUANTUM_SCRIPT_EXTENSION_CONSOLE_EXPORT void quantumScriptExtension(Quantum::Script::Executive *executive, void *extensionId) {
	Quantum::Script::Extension::Console::initExecutive(executive, extensionId);
};
#endif

