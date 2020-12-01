//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script--config.hpp"

#ifdef XYO_OS_WINDOWS
#       define WIN32_LEAN_AND_MEAN
#       include <windows.h>
#else
#       include <dlfcn.h>
#endif

#ifndef QUANTUM_SCRIPT_NO_VERSION
#ifndef QUANTUM_SCRIPT_VERSION_HPP
#include "quantum-script-version.hpp"
#endif
#endif

#include "quantum-script-libstdscript.hpp"

#include "quantum-script-variablenull.hpp"
#include "quantum-script-variableboolean.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"
#include "quantum-script-variableassociativearray.hpp"

#include "quantum-script-variablenativevmfunction.hpp"

#include "quantum-script-variableresource.hpp"
#include "quantum-script-variablestacktrace.hpp"

#include "quantum-script-libstdscript.src"

//#define QUANTUM_SCRIPT_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {

		namespace LibStdScript {

			static TPointer<Variable> isUndefined(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-undefined\n");
#endif
				return VariableBoolean::newVariable(VariableUndefined::isVariableUndefined(arguments->index(0)));
			};

			static TPointer<Variable> isNull(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-null\n");
#endif
				return VariableBoolean::newVariable(VariableNull::isVariableNull(arguments->index(0)));
			};

			static TPointer<Variable> isNil(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-nil\n");
#endif
				TPointerX<Variable> &value = arguments->index(0);
				return VariableBoolean::newVariable(
						VariableUndefined::isVariableUndefined(value) ||
						VariableNull::isVariableNull(value)
					);
			};

			static TPointer<Variable> isNaN(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-nan\n");
#endif
				return VariableBoolean::newVariable(isnan((arguments->index(0))->toNumber()));
			};

			static TPointer<Variable> isFinite(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-finite\n");
#endif
				return VariableBoolean::newVariable(isfinite((arguments->index(0))->toNumber()));
			};

			static TPointer<Variable> isInfinite(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-infinite\n");
#endif
				return VariableBoolean::newVariable(isinf((arguments->index(0))->toNumber()));
			};

			static TPointer<Variable> signBit(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-sign-bit\n");
#endif
				return VariableBoolean::newVariable(signbit((arguments->index(0))->toNumber()));
			};


			static TPointer<Variable> isBoolean(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-boolean\n");
#endif
				return VariableBoolean::newVariable(VariableBoolean::isVariableBoolean(arguments->index(0)));
			};

			static TPointer<Variable> isNumber(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-number\n");
#endif
				return VariableBoolean::newVariable(VariableNumber::isVariableNumber(arguments->index(0)));
			};

			static TPointer<Variable> isString(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-string\n");
#endif
				return VariableBoolean::newVariable(VariableString::isVariableString(arguments->index(0)));
			};

			static TPointer<Variable> isArray(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-array\n");
#endif
				return VariableBoolean::newVariable(VariableArray::isVariableArray(arguments->index(0)));
			};

			static TPointer<Variable> isResource(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-resource\n");
#endif
				return VariableBoolean::newVariable(VariableResource::isVariableResource(arguments->index(0)));
			};

			static TPointer<Variable> isAssociativeArray(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-associative-array\n");
#endif
				return VariableBoolean::newVariable(VariableAssociativeArray::isVariableAssociativeArray(arguments->index(0)));
			};

			static TPointer<Variable> isFunction(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-function\n");
#endif

				TPointerX<Variable> &variable = arguments->index(0);
				return VariableBoolean::newVariable(
						VariableFunction::isVariableFunction(variable) ||
						VariableFunctionWithYield::isVariableFunctionWithYield(variable) ||
						VariableVmFunction::isVariableVmFunction(variable) ||
						VariableNativeVmFunction::isVariableNativeVmFunction(variable)
					);

			};

			static TPointer<Variable> isNativeFunction(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-native-function\n");
#endif
				TPointerX<Variable> &variable = arguments->index(0);
				return VariableBoolean::newVariable(
						VariableFunction::isVariableFunction(variable) ||
						VariableFunctionWithYield::isVariableFunctionWithYield(variable) ||
						VariableNativeVmFunction::isVariableNativeVmFunction(variable)
					);
			};

			static TPointer<Variable> isObject(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-object\n");
#endif
				return VariableBoolean::newVariable(VariableObject::isVariableObject(arguments->index(0)));
			};

			static TPointer<Variable> isDefined(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-defined\n");
#endif
				return VariableBoolean::newVariable(!VariableUndefined::isVariableUndefined(arguments->index(0)));
			};

			static TPointer<Variable> isStackTrace(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-is-stack-trace\n");
#endif
				return VariableBoolean::newVariable(VariableStackTrace::isVariableStackTrace(arguments->index(0)));
			};

			static TPointer<Variable> stackTraceWithLevel(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-stack-trace-with-level\n");
#endif
				return VariableString::newVariable((((VariableStackTrace *) (arguments->index(0)).value() ))->toString((Integer)((arguments->index(1))->toNumber())));
			};

			static QUANTUM_SCRIPT_INSTRUCTION_IMPLEMENT(LibStdScript_include) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("#%p    script-include\n", context->currentProgramCounter);
#endif

				TPointer<Variable> operand1;
				TPointer<Variable> operand2;
				char buf[2048];

				operand1 = context->getArgument(0);
				operand2 = context->getArgument(1);

				if(!VariableArray::isVariableArray(operand2)) {
					operand2=VariableArray::newVariable();
				};
				context->functionContext->functionArguments=static_cast<VariableArray *>(operand2.value());

				if(!context->functionContext->this_) {
					context->functionContext->this_=VariableUndefined::newVariable();
				};

				if (operand1) {
					if (VariableString::isVariableString(operand1)) {

						Executive *executive = (Executive *) (((VariableResource *) operand)->resource);
						bool found = false;
						String fileName;
						fileName = ((VariableString *) operand1.value())->value.value();
						if (Shell::fileExists(fileName)) {
							found = true;
						} else {
							TDoubleEndedQueue<String >::Node *scan;
							for (scan = executive->includePath->head; scan; scan = scan->next) {
								fileName = scan->value;
								fileName << "/" << ((VariableString *) operand1.value())->value.value();
								if (Shell::fileExists(fileName)) {
									found = true;
									break;
								};
							};
						};

						if(!found) {
							if(context->includedFile.head) {
								fileName = ((VariableString *) operand1.value())->value.value();
								if(!Shell::isAbsolutePath(fileName)) {
									fileName = Shell::getFilePathX((context->includedFile.head)->value) << fileName;
									if(Shell::fileExists(fileName)) {
										found = true;
									};
								};
							};
						};

						if (found) {
							int retV = executive->includeAndExecuteFile(context, fileName);
							if (retV == VmParserError::None) {
								return;
							} else if (retV == VmParserError::Compile) {
								sprintf( buf, "Compile error in %s line %u\n",
									executive->errorInfo.compileFileName,
									executive->errorInfo.compileLineNumber
								);
								context->push(context->newError(buf));
								InstructionVmThrow(context, nullptr);
								return;
							} else if (retV == VmParserError::FileNotFound) {
								sprintf( buf, "File not found \"%s\"\n",
									((VariableString *) operand1.value())->value.value()
								);
								context->push(context->newError(buf));
								InstructionVmThrow(context, nullptr);
								return;
							};
						};

						String stringToCompile;
						if(executive->includeSource->get(((VariableString *) operand1.value())->value, stringToCompile)) {
							int retV = executive->includeAndExecuteString(context, stringToCompile);
							if (retV == VmParserError::None) {
								return;
							} else if (retV == VmParserError::Compile) {
								sprintf( buf, "Compile error in %s line %u\n",
									executive->errorInfo.compileFileName,
									executive->errorInfo.compileLineNumber
								);
								context->push(context->newError(buf));
								InstructionVmThrow(context, nullptr);
								return;
							};
						};

						sprintf( buf, "Unable to open \"%s\"\n",
							((VariableString *) operand1.value())->value.value()
						);

						context->push(context->newError(buf));
						InstructionVmThrow(context, nullptr);
						return;
					};
				};


				context->push(context->newError("invalid arguments"));
				InstructionVmThrow(context, nullptr);

			};

			static QUANTUM_SCRIPT_INSTRUCTION_IMPLEMENT(LibStdScript_includeOnce) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("#%p    script-include-once\n", context->currentProgramCounter);
#endif

				TPointer<Variable> operand1;
				TPointer<Variable> operand2;
				char buf[2048];

				operand1 = context->getArgument(0);
				operand2 = context->getArgument(1);

				if(!VariableArray::isVariableArray(operand2)) {
					operand2=VariableArray::newVariable();
				};

				context->functionContext->functionArguments=static_cast<VariableArray *>(operand2.value());

				if(!context->functionContext->this_) {
					context->functionContext->this_=VariableUndefined::newVariable();
				};

				if (operand1) {
					if (VariableString::isVariableString(operand1)) {
						Executive *executive = (Executive *) (((VariableResource *) operand)->resource);
						bool found = false;
						String fileName;
						fileName = ((VariableString *) operand1.value())->value.value();
						if (Shell::fileExists(fileName)) {
							found = true;
						} else {
							TDoubleEndedQueue<String >::Node *scan;
							for (scan = executive->includePath->head; scan; scan = scan->next) {
								fileName = scan->value;
								fileName << "/" << ((VariableString *) operand1.value())->value.value();
								if (Shell::fileExists(fileName)) {
									found = true;
									break;
								};
							};
						};

						if(!found) {
							if(context->includedFile.head) {
								fileName = ((VariableString *) operand1.value())->value.value();
								if(!Shell::isAbsolutePath(fileName)) {
									fileName = Shell::getFilePathX((context->includedFile.head)->value) << fileName;
									if(Shell::fileExists(fileName)) {
										found = true;
									};
								};
							};
						};

						if (found) {
							char fileNameOut[1024];
							String fileNameOut_;
							if(Shell::realpath(fileName, fileNameOut, 1024)) {
								fileNameOut_ = fileNameOut;
								TRedBlackTree<String, bool>::Node *node = context->listIncludeOnce.find(fileNameOut_);
								if(node) {
									return;
								};
								bool xxx_ = true;
								context->listIncludeOnce.set(fileNameOut_, xxx_);

								int retV = executive->includeAndExecuteFile(context, fileName);
								if (retV == VmParserError::None) {
									return;
								} else if (retV == VmParserError::Compile) {
									sprintf( buf, "Compile error in %s line %u\n",
										executive->errorInfo.compileFileName,
										executive->errorInfo.compileLineNumber
									);
									context->push(context->newError(buf));
									InstructionVmThrow(context, nullptr);
									return;
								} else if (retV == VmParserError::FileNotFound) {
									sprintf( buf, "File not found \"%s\"\n",
										((VariableString *) operand1.value())->value.value()
									);
									context->push(context->newError(buf));
									InstructionVmThrow(context, nullptr);
									return;
								};

							};
						};

						String stringToCompile;
						if(executive->includeSource->get(((VariableString *) operand1.value())->value, stringToCompile)) {
							int retV = executive->includeAndExecuteString(context, stringToCompile);
							if (retV == VmParserError::None) {
								return;
							} else if (retV == VmParserError::Compile) {
								sprintf( buf, "Compile error in %s line %u\n",
									executive->errorInfo.compileFileName,
									executive->errorInfo.compileLineNumber
								);
								context->push(context->newError(buf));
								InstructionVmThrow(context, nullptr);
								return;
							};
						};


						sprintf( buf, "Unable to open \"%s\"\n",
							((VariableString *) operand1.value())->value.value()
						);

						context->push(context->newError(buf));
						InstructionVmThrow(context, nullptr);
						return;
					};
				};


				context->push(context->newError("invalid arguments"));
				InstructionVmThrow(context, nullptr);

			};

			static QUANTUM_SCRIPT_INSTRUCTION_IMPLEMENT(LibStdScript_includeSkipLines) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("#%p    script-include-skip-lines\n", context->currentProgramCounter);
#endif

				TPointer<Variable> operand1;
				TPointer<Variable> operand2;
				char buf[2048];

				operand1 = context->getArgument(0);
				operand2 = context->getArgument(1);
				size_t skipLines = (context->getArgument(2))->toIndex();

				if(!VariableArray::isVariableArray(operand2)) {
					operand2=VariableArray::newVariable();
				};
				context->functionContext->functionArguments=static_cast<VariableArray *>(operand2.value());

				if(!context->functionContext->this_) {
					context->functionContext->this_=VariableUndefined::newVariable();
				};

				if (operand1) {
					if (VariableString::isVariableString(operand1)) {

						Executive *executive = (Executive *) (((VariableResource *) operand)->resource);
						bool found = false;
						String fileName;
						fileName = ((VariableString *) operand1.value())->value.value();
						if (Shell::fileExists(fileName)) {
							found = true;
						} else {
							TDoubleEndedQueue<String >::Node *scan;
							for (scan = executive->includePath->head; scan; scan = scan->next) {
								fileName = scan->value;
								fileName << "/" << ((VariableString *) operand1.value())->value.value();
								if (Shell::fileExists(fileName)) {
									found = true;
									break;
								};
							};
						};

						if(!found) {
							if(context->includedFile.head) {
								fileName = ((VariableString *) operand1.value())->value.value();
								if(!Shell::isAbsolutePath(fileName)) {
									fileName = Shell::getFilePathX((context->includedFile.head)->value) << fileName;
									if(Shell::fileExists(fileName)) {
										found = true;
									};
								};
							};
						};

						if (found) {
							int retV = executive->includeAndExecuteFileSkipLines(context, fileName, skipLines);
							if (retV == VmParserError::None) {
								return;
							} else if (retV == VmParserError::Compile) {
								sprintf( buf, "Compile error in %s line %u\n",
									executive->errorInfo.compileFileName,
									executive->errorInfo.compileLineNumber
								);
								context->push(context->newError(buf));
								InstructionVmThrow(context, nullptr);
								return;
							} else if (retV == VmParserError::FileNotFound) {
								sprintf( buf, "File not found \"%s\"\n",
									((VariableString *) operand1.value())->value.value()
								);
								context->push(context->newError(buf));
								InstructionVmThrow(context, nullptr);
								return;
							};
						};

						String stringToCompile;
						if(executive->includeSource->get(((VariableString *) operand1.value())->value, stringToCompile)) {
							int retV = executive->includeAndExecuteStringSkipLines(context, stringToCompile, skipLines);
							if (retV == VmParserError::None) {
								return;
							} else if (retV == VmParserError::Compile) {
								sprintf( buf, "Compile error in %s line %u\n",
									executive->errorInfo.compileFileName,
									executive->errorInfo.compileLineNumber
								);
								context->push(context->newError(buf));
								InstructionVmThrow(context, nullptr);
								return;
							};
						};

						sprintf( buf, "Unable to open \"%s\"\n",
							((VariableString *) operand1.value())->value.value()
						);

						context->push(context->newError(buf));
						InstructionVmThrow(context, nullptr);
						return;
					};
				};


				context->push(context->newError("invalid arguments"));
				InstructionVmThrow(context, nullptr);

			};



			static QUANTUM_SCRIPT_INSTRUCTION_IMPLEMENT(LibStdScript_execute) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("#%p    script-execute\n", context->currentProgramCounter);
#endif

				TPointer<Variable> operand1;
				TPointer<Variable> operand2;
				char buf[2048];

				operand1 = context->getArgument(0);
				operand2 = context->getArgument(1);

				if(!VariableArray::isVariableArray(operand2)) {
					operand2=VariableArray::newVariable();
				};
				context->functionContext->functionArguments=static_cast<VariableArray *>(operand2.value());

				if(!context->functionContext->this_) {
					context->functionContext->this_=VariableUndefined::newVariable();
				};

				if (operand1) {
					if (VariableString::isVariableString(operand1)) {
						Executive *executive = (Executive *) (((VariableResource *) operand)->resource);
						String stringToCompile;
						stringToCompile = ((VariableString *) operand1.value())->value.value();
						int retV = executive->includeAndExecuteString(context, stringToCompile);
						if (retV == VmParserError::None) {
							return;
						} else if (retV == VmParserError::Compile) {
							sprintf( buf, "Compile error in %s line %u\n",
								executive->errorInfo.compileFileName,
								executive->errorInfo.compileLineNumber
							);
							context->push(context->newError(buf));
							InstructionVmThrow(context, nullptr);
							return;
						};
					};
				};

				context->push(context->newError("invalid arguments"));
				InstructionVmThrow(context, nullptr);
			};

			static QUANTUM_SCRIPT_INSTRUCTION_IMPLEMENT(LibStdScript_executeSkipLines) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("#%p    script-execute-skip-lines\n", context->currentProgramCounter);
#endif

				TPointer<Variable> operand1;
				TPointer<Variable> operand2;
				char buf[2048];

				operand1 = context->getArgument(0);
				operand2 = context->getArgument(1);
				size_t skipLines = (context->getArgument(2))->toIndex();

				if(!VariableArray::isVariableArray(operand2)) {
					operand2=VariableArray::newVariable();
				};
				context->functionContext->functionArguments=static_cast<VariableArray *>(operand2.value());

				if(!context->functionContext->this_) {
					context->functionContext->this_=VariableUndefined::newVariable();
				};

				if (operand1) {
					if (VariableString::isVariableString(operand1)) {
						Executive *executive = (Executive *) (((VariableResource *) operand)->resource);
						String stringToCompile;
						stringToCompile = ((VariableString *) operand1.value())->value.value();
						int retV = executive->includeAndExecuteStringSkipLines(context, stringToCompile, skipLines);
						if (retV == VmParserError::None) {
							return;
						} else if (retV == VmParserError::Compile) {
							sprintf( buf, "Compile error in %s line %u\n",
								executive->errorInfo.compileFileName,
								executive->errorInfo.compileLineNumber
							);
							context->push(context->newError(buf));
							InstructionVmThrow(context, nullptr);
							return;
						};
					};
				};

				context->push(context->newError("invalid arguments"));
				InstructionVmThrow(context, nullptr);
			};


			static QUANTUM_SCRIPT_INSTRUCTION_IMPLEMENT(LibStdScript_getIncludedFile) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("#%p    script-get-included-file\n", context->currentProgramCounter);
#endif
				if(context->includedFile.head) {
					context->push(VariableString::newVariable((context->includedFile.head)->value));
				} else {
					context->push(VariableString::newVariable(""));
				};
				InstructionVmReturn(context, nullptr);
			};

			static QUANTUM_SCRIPT_INSTRUCTION_IMPLEMENT(LibStdScript_requireExternalExtension) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("#%p    script-require-external-extension\n", context->currentProgramCounter);
#endif

				TPointer<Variable> operand1;
				TPointer<Variable> operand2;
				char buf[2048];

				operand1 = context->getArgument(0);
				operand2 = context->getArgument(1);

				if(!VariableArray::isVariableArray(operand2)) {
					operand2=VariableArray::newVariable();
				};
				context->functionContext->functionArguments=static_cast<VariableArray *>(operand2.value());

				if(!context->functionContext->this_) {
					context->functionContext->this_=VariableUndefined::newVariable();
				};

				if (operand1) {
					if (VariableString::isVariableString(operand1)) {
						Executive *executive = (Executive *) (((VariableResource *) operand)->resource);

						if(executive->isExtensionLoaded(((VariableString *) operand1.value())->value)) {
							return;
						};

						bool found = false;
						String fileNameFull;
						String filePath;
						String fileName;
						String fileNameFinal;
						String fileNameX;
						size_t index;


						fileNameFull = ((VariableString *) operand1.value())->value;
						fileName = "qse-";
						if(String::indexOfFromEnd(fileNameFull, "\\", 0, index)) {
							filePath = String::substring(fileNameFull, 0, index + 1);
							fileName << String::toLowerCaseAscii(String::substring(fileNameFull, index + 1));
						} else if(String::indexOfFromEnd(fileNameFull, "/", 0, index)) {
							filePath = String::substring(fileNameFull, 0, index + 1);
							fileName << String::toLowerCaseAscii(String::substring(fileNameFull, index + 1));
						} else {
							fileName << String::toLowerCaseAscii(fileNameFull);
						};

						fileNameFinal = filePath;
						fileNameFinal << fileName;

#ifdef XYO_OS_WINDOWS
						fileNameFinal << ".dll";
#else
						fileNameFinal << ".so";
#endif

						if (Shell::fileExists(fileNameFinal)) {
							fileNameX = fileNameFinal;
							found = true;
						} else {
							TDoubleEndedQueue<String>::Node *scan;
							for (scan = executive->includePath->head; scan; scan = scan->next) {
								fileNameX = scan->value;
								fileNameX << "/" << fileNameFinal;
								if (Shell::fileExists(fileNameX)) {
									found = true;
									break;
								};
							};
						};

						if (found) {
							TDoubleEndedQueue<Extension_>::Node *scan;
							for (scan = executive->extensionList->head; scan; scan = scan->next) {
								if(scan->value.fileName == fileNameX) {
									return;
								};
							};
#ifdef XYO_OS_WINDOWS
							HMODULE hModule = LoadLibrary(fileNameX);
							if(hModule) {
								FARPROC farProc = GetProcAddress(hModule, "quantumScriptExtension");
								if(farProc) {
									executive->extensionList->push();
									(executive->extensionList->head)->value.fileName = fileNameX;
									(executive->extensionList->head)->value.name = ((VariableString *) operand1.value())->value;
									(executive->extensionList->head)->value.isPublic = true;
									if(executive->executeExtension(context, (QuantumScriptExtensionProc)farProc, executive->extensionList->head) == 0) {
										return;
									};
								};
							};
#else
							void *hModule = dlopen(fileNameX, RTLD_LAZY);
							if(hModule) {
								QuantumScriptExtensionProc farProc = (QuantumScriptExtensionProc)dlsym(hModule, "quantumScriptExtension");
								if(dlerror() == nullptr) {
									if(farProc) {
										executive->extensionList->push();
										(executive->extensionList->head)->value.fileName = fileNameX;
										(executive->extensionList->head)->value.name = ((VariableString *) operand1.value())->value;
										(executive->extensionList->head)->value.isPublic = true;
										if(executive->executeExtension(context, (QuantumScriptExtensionProc)farProc, executive->extensionList->head) == 0) {
											return;
										};
									};
								};
							};
#endif
						} else {

							TDoubleEndedQueue<InternalExtension_ >::Node *scan;
							String extensionName_ = String::toLowerCaseAscii(((VariableString *) operand1.value())->value);
							for (scan = executive->internalExtensionList->head; scan; scan = scan->next) {
								if(extensionName_ == String::toLowerCaseAscii(scan->value.name)) {
									executive->extensionList->push();
									(executive->extensionList->head)->value.fileName = "";
									(executive->extensionList->head)->value.name = ((VariableString *) operand1.value())->value;
									(executive->extensionList->head)->value.isPublic = true;
									if(executive->executeExtension(context, (QuantumScriptExtensionProc)scan->value.extensionProc, executive->extensionList->head) == 0) {
										return;
									};
									break;
								};
							};

						};
						sprintf( buf, "Unable to open \"%s\"\n",
							((VariableString *) operand1.value())->value.value()
						);

						context->push(context->newError(buf));
						InstructionVmThrow(context, nullptr);
						return;
					};
				};

				context->push(context->newError("invalid arguments"));
				InstructionVmThrow(context, nullptr);

			};


			static QUANTUM_SCRIPT_INSTRUCTION_IMPLEMENT(LibStdScript_requireInternalExtension) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("#%p    script-require-internal-extension\n", context->currentProgramCounter);
#endif

				TPointer<Variable> operand1;
				TPointer<Variable> operand2;
				char buf[2048];

				operand1 = context->getArgument(0);
				operand2 = context->getArgument(1);

				if(!VariableArray::isVariableArray(operand2)) {
					operand2=VariableArray::newVariable();
				};
				context->functionContext->functionArguments=static_cast<VariableArray *>(operand2.value());

				if(!context->functionContext->this_) {
					context->functionContext->this_=VariableUndefined::newVariable();
				};

				if (operand1) {
					if (VariableString::isVariableString(operand1)) {
						Executive *executive = (Executive *) (((VariableResource *) operand)->resource);

						if(executive->isExtensionLoaded(((VariableString *) operand1.value())->value)) {
							return;
						};

						TDoubleEndedQueue<InternalExtension_ >::Node *scan;
						String extensionName_ = String::toLowerCaseAscii(((VariableString *) operand1.value())->value);
						for (scan = executive->internalExtensionList->head; scan; scan = scan->next) {
							if(extensionName_ == String::toLowerCaseAscii(scan->value.name)) {
								executive->extensionList->push();
								(executive->extensionList->head)->value.fileName = "";
								(executive->extensionList->head)->value.name = ((VariableString *) operand1.value())->value;
								(executive->extensionList->head)->value.isPublic = true;
								if(executive->executeExtension(context, (QuantumScriptExtensionProc)scan->value.extensionProc, executive->extensionList->head) == 0) {
									return;
								};
								break;
							};
						};


						sprintf( buf, "Unable to open \"%s\"\n",
							((VariableString *) operand1.value())->value.value()
						);

						context->push(context->newError(buf));
						InstructionVmThrow(context, nullptr);
						return;
					};
				};

				context->push(context->newError("invalid arguments"));
				InstructionVmThrow(context, nullptr);

			};


			static TPointer<Variable> setIncludePath(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-set-include-path\n");
#endif

				((Executive *)function->valueSuper)->includePath->push((arguments->index(0))->toString());
				return VariableBoolean::newVariable(true);
			};

			static TPointer<Variable> getPrototypeOf(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-get-prototype-of\n");
#endif

				TPointerX<Variable> &value = arguments->index(0);
				Variable *prototype = value->instancePrototype();
				if(prototype) {
					return prototype;
				};
				return Context::getValueUndefined();
			};


			static QUANTUM_SCRIPT_INSTRUCTION_IMPLEMENT(LibStdScript_getExtensionList) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-get-extension-list\n");
#endif

				Executive *executive = (Executive *) (((VariableResource *) operand)->resource);

				Variable *out = VariableArray::newVariable();
				TDoubleEndedQueue<Extension_>::Node *scan;
				Symbol symFileName = Context::getSymbol("fileName");
				Symbol symName = Context::getSymbol("name");
				Symbol symInfo = Context::getSymbol("info");
				Symbol symVersion = Context::getSymbol("version");
				int k;
				for (scan = executive->extensionList->head, k = 0; scan; scan = scan->next, ++k) {
					if(scan->value.isPublic) {
						(out->operatorIndex(k))=VariableObject::newVariable();
						((out->operatorIndex(k))->operatorReferenceOwnProperty(symFileName))=VariableString::newVariable(scan->value.fileName);
						((out->operatorIndex(k))->operatorReferenceOwnProperty(symName))=VariableString::newVariable(scan->value.name);
						((out->operatorIndex(k))->operatorReferenceOwnProperty(symInfo))=VariableString::newVariable(scan->value.info);
						((out->operatorIndex(k))->operatorReferenceOwnProperty(symVersion))=VariableString::newVariable(scan->value.version);
					};
				};
				context->push(out);
				InstructionVmReturn(context, nullptr);
			};

			static TPointer<Variable> protectSource(VariableFunction *function, Variable *this_, VariableArray *arguments) {
				TPointerX<Variable> &value = arguments->index(0);
				if(VariableVmFunction::isVariableVmFunction(value)) {
					((VariableVmFunction *)value.value())->fnSource = 0;
					if(((VariableVmFunction *)value.value())->valueEnd) {
						TDoubleEndedQueue<InstructionX>::Node *index = (TDoubleEndedQueue<InstructionX>::Node *)(((VariableVmFunction *)value.value())->value);
						TDoubleEndedQueue<InstructionX>::Node *indexEnd = (TDoubleEndedQueue<InstructionX>::Node *)(((VariableVmFunction *)value.value())->valueEnd);

						for(; index != indexEnd; index = index->next) {
							index->value.sourceSymbol = 0;
							index->value.sourceLineNumber = 0;
						};

						// indexEnd also
						index->value.sourceSymbol = 0;
						index->value.sourceLineNumber = 0;

					};
				};
				return Context::getValueUndefined();
			};

			static QUANTUM_SCRIPT_INSTRUCTION_IMPLEMENT(LibStdScript_setFunctionFromFile) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("#%p    script-set-function-from-file\n", context->currentProgramCounter);
#endif

				TPointer<Variable> operand1;
				TPointer<Variable> operand2;
				char buf[2048];

				operand2 = context->getArgument(0);
				operand1 = context->getArgument(1);

				if(!context->functionContext->this_) {
					context->functionContext->this_=VariableUndefined::newVariable();
				};

				if (operand1) {
					if (VariableString::isVariableString(operand1)) {
						if (operand2) {
							if (VariableString::isVariableString(operand2)) {
								Executive *executive = (Executive *) (((VariableResource *) operand)->resource);
								bool found = false;
								String fileName;
								fileName = ((VariableString *) operand1.value())->value.value();
								if (Shell::fileExists(fileName)) {
									found = true;
								} else {
									TDoubleEndedQueue<String >::Node *scan;
									for (scan = executive->includePath->head; scan; scan = scan->next) {
										fileName = scan->value;
										fileName << "/" << ((VariableString *) operand1.value())->value.value();
										if (Shell::fileExists(fileName)) {
											found = true;
											break;
										};
									};
								};

								if (found) {
									int retV = executive->setVmFunctionFromFileX(context, ((VariableString *) operand2.value())->value, fileName);
									if (retV == VmParserError::None) {
										return;
									} else if (retV == VmParserError::Compile) {
										sprintf( buf, "Compile error in %s line %u\n",
											executive->errorInfo.compileFileName,
											executive->errorInfo.compileLineNumber
										);
										context->push(context->newError(buf));
										InstructionVmThrow(context, nullptr);
										return;
									} else if (retV == VmParserError::FileNotFound) {
										sprintf( buf, "File not found \"%s\"\n",
											((VariableString *) operand1.value())->value.value()
										);
										context->push(context->newError(buf));
										InstructionVmThrow(context, nullptr);
										return;
									};

								};


								sprintf( buf, "Unable to open \"%s\"\n",
									((VariableString *) operand1.value())->value.value()
								);

								context->push(context->newError(buf));
								InstructionVmThrow(context, nullptr);
								return;
							};
						};
					};
				};


				context->push(context->newError("invalid arguments"));
				InstructionVmThrow(context, nullptr);
			};


			static QUANTUM_SCRIPT_INSTRUCTION_IMPLEMENT(LibStdScript_setFunctionFromString) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("#%p    script-set-function-from-string\n", context->currentProgramCounter);
#endif

				TPointer<Variable> operand1;
				TPointer<Variable> operand2;
				char buf[2048];

				operand2 = context->getArgument(0);
				operand1 = context->getArgument(1);

				if(!context->functionContext->this_) {
					context->functionContext->this_=VariableUndefined::newVariable();
				};

				if (operand1) {
					if (VariableString::isVariableString(operand1)) {
						if (operand2) {
							if (VariableString::isVariableString(operand2)) {
								Executive *executive = (Executive *) (((VariableResource *) operand)->resource);

								int retV = executive->setVmFunctionFromStringX(context, ((VariableString *) operand2.value())->value, ((VariableString *) operand1.value())->value);
								if (retV == VmParserError::None) {
									return;
								} else if (retV == VmParserError::Compile) {
									sprintf( buf, "Compile error in %s line %u\n",
										executive->errorInfo.compileFileName,
										executive->errorInfo.compileLineNumber
									);
									context->push(context->newError(buf));
									InstructionVmThrow(context, nullptr);
									return;
								};

								context->push(context->newError("script-set-function-from-string"));
								InstructionVmThrow(context, nullptr);
								return;
							};
						};
					};
				};


				context->push(context->newError("invalid arguments"));
				InstructionVmThrow(context, nullptr);
			};


			static TPointer<Variable> compare(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-compare\n");
#endif
				return VariableNumber::newVariable((arguments->index(0))->compare(arguments->index(1)));
			};

			static TPointer<Variable> reverseCompare(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-reverse-compare\n");
#endif
				return VariableNumber::newVariable(-((arguments->index(0))->compare(arguments->index(1))));
			};

			static TPointer<Variable> getVersion(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
				printf("- script-get-version\n");
#endif

#ifndef QUANTUM_SCRIPT_NO_VERSION
				return VariableString::newVariable(Quantum::Script::Version::versionWithBuild());
#else
				return VariableString::newVariable("0.0.0.0");
#endif
			};

			void initExecutive(Executive *executive, void *extensionId) {
				executive->compileStringX("var Script={};");
				executive->setFunction2("Script.isUndefined(x)", isUndefined);
				executive->setFunction2("Script.isNull(x)", isNull);
				executive->setFunction2("Script.isNil(x)", isNil);
				executive->setFunction2("Script.isNaN(x)", isNaN);
				executive->setFunction2("Script.isFinite(x)", isFinite);
				executive->setFunction2("Script.isInfinite(x)", isInfinite);
				executive->setFunction2("Script.signBit(x)", signBit);
				executive->setFunction2("Script.isBoolean(x)", isBoolean);
				executive->setFunction2("Script.isNumber(x)", isNumber);
				executive->setFunction2("Script.isString(x)", isString);
				executive->setFunction2("Script.isArray(x)", isArray);
				executive->setFunction2("Script.isResource(x)", isResource);
				executive->setFunction2("Script.isAssociativeArray(x)", isAssociativeArray);
				executive->setFunction2("Script.isFunction(x)", isFunction);
				executive->setFunction2("Script.isNativeFunction(x)", isNativeFunction);
				executive->setFunction2("Script.isObject(x)", isObject);
				executive->setFunction2("Script.isDefined(x)", isDefined);
				executive->setFunction2("Script.isStackTrace(x)", isStackTrace);
				executive->setFunction2("Script.stackTraceWithLevel(x,level)", stackTraceWithLevel);
				executive->setVmFunction("Script.include", InstructionLibStdScript_include, VariableResource::newVariable(executive, nullptr));
				executive->setVmFunction("Script.includeOnce", InstructionLibStdScript_includeOnce, TPointer<Variable>(VariableResource::newVariable(executive, nullptr)));
				executive->setVmFunction("Script.includeSkipLines", InstructionLibStdScript_includeSkipLines, VariableResource::newVariable(executive, nullptr));
				executive->setVmFunction("Script.execute", InstructionLibStdScript_execute, TPointer<Variable>(VariableResource::newVariable(executive, nullptr)));
				executive->setVmFunction("Script.executeSkipLines", InstructionLibStdScript_executeSkipLines, TPointer<Variable>(VariableResource::newVariable(executive, nullptr)));
				executive->setVmFunction("Script.requireExternalExtension", InstructionLibStdScript_requireExternalExtension, TPointer<Variable>(VariableResource::newVariable(executive, nullptr)));
				executive->setVmFunction("Script.requireInternalExtension", InstructionLibStdScript_requireInternalExtension, TPointer<Variable>(VariableResource::newVariable(executive, nullptr)));
				executive->setVmFunction("Script.getExtensionList", InstructionLibStdScript_getExtensionList, TPointer<Variable>(VariableResource::newVariable(executive, nullptr)));
				executive->setFunction4("Script.setIncludePath(path)", setIncludePath, executive);
				executive->setFunction2("Script.getPrototypeOf(x)", getPrototypeOf);
				executive->setFunction2("Script.protectSource(fn)", protectSource);
				executive->setFunction2("Script.compare(a,b)", compare);
				executive->setFunction2("Script.reverseCompare(a,b)", reverseCompare);
				executive->setVmFunction("Script.setFunctionFromFile", InstructionLibStdScript_setFunctionFromFile, TPointer<Variable>(VariableResource::newVariable(executive, nullptr)));
				executive->setVmFunction("Script.setFunctionFromString", InstructionLibStdScript_setFunctionFromString, TPointer<Variable>(VariableResource::newVariable(executive, nullptr)));
				executive->setVmFunction("Script.getIncludedFile", InstructionLibStdScript_getIncludedFile, nullptr);
				executive->compileStringX("Script.requireExtension=Script.requireExternalExtension;");
				executive->setFunction2("Script.getVersion()", getVersion);
				executive->compileStringX(libStdScriptSource);
			};

		};
	};
};


