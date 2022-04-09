//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXECUTIVE_HPP
#define QUANTUM_SCRIPT_EXECUTIVE_HPP

#ifndef QUANTUM_SCRIPT_PARSER_HPP
#	include "quantum-script-parser.hpp"
#endif

#ifndef QUANTUM_SCRIPT_ASM_HPP
#	include "quantum-script-asm.hpp"
#endif

#ifndef QUANTUM_SCRIPT_VARIABLEFUNCTIONWITHYIELD_HPP
#	include "quantum-script-variablefunctionwithyield.hpp"
#endif

extern "C" typedef void (*QuantumScriptExtensionProc)(void *, void *);

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VmParserError {
			public:
				enum {
					None = ParserError::None,
					Compile = ParserError::Compile,
					FileNotFound
				};
		};

		class ErrorInfo {
			public:
				int compileError;
				uint32_t compileLineNumber;
				const char *compileFileName;
		};

		class Executive;
		typedef void (*QuantumScriptExtensionInitProc)(Executive *, void *);
		typedef void (*QuantumScriptInitExecutiveProc)(Executive *);
		typedef void (*QuantumScriptExtensionDeleteContextProc)();

		class Extension_ : public Object {
			public:
				String fileName;
				String name;
				String info;
				String version;
				bool isPublic;

				QuantumScriptExtensionDeleteContextProc deleteContext;

				inline Extension_() {
					isPublic = false;
					deleteContext = nullptr;
				};
		};

		class InternalExtension_ : public Object {
			public:
				String name;
				QuantumScriptExtensionInitProc extensionProc;
		};

		class Executive : public Object {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(Executive);

			protected:
				QUANTUM_SCRIPT_EXPORT void errorInfoInit();
				QUANTUM_SCRIPT_EXPORT int execute_(InstructionContext *);
				ProgramCounter *currentContextExit;
				ProgramCounter *currentContextReturn;
				ProgramCounter *currentContextThrow;
				ProgramCounter *currentContextContinue;
				ProgramCounter *currentContextBreak;

			public:
				int mainCmdN;
				char **mainCmdS;
				String pathExecutable;

				TPointer<TDoubleEndedQueue<String>> includePath;
				TPointer<TDoubleEndedQueue<Extension_>> extensionList;
				TPointer<TDoubleEndedQueue<InternalExtension_>> internalExtensionList;
				TPointer<TRedBlackTree<String, String>> includeSource;

				QuantumScriptInitExecutiveProc applicationInitExecutive;

				TPointer<InstructionContext> instructionContext;
				int configPrintStackTraceLimit;

				TPointer<Asm> assembler;
				ErrorInfo errorInfo;

				QUANTUM_SCRIPT_EXPORT Executive();

				QUANTUM_SCRIPT_EXPORT ~Executive();

				QUANTUM_SCRIPT_EXPORT bool isExtensionLoaded(String extensionName);
				QUANTUM_SCRIPT_EXPORT void setExtensionName(void *extensionId, String name);
				QUANTUM_SCRIPT_EXPORT void setExtensionInfo(void *extensionId, String info);
				QUANTUM_SCRIPT_EXPORT void setExtensionVersion(void *extensionId, String version);
				QUANTUM_SCRIPT_EXPORT void setExtensionPublic(void *extensionId, bool isPublic);
				QUANTUM_SCRIPT_EXPORT void setExtensionDeleteContext(void *extensionId, QuantumScriptExtensionDeleteContextProc deleteContext);
				QUANTUM_SCRIPT_EXPORT void initExtension(QuantumScriptExtensionInitProc extensionProc);
				QUANTUM_SCRIPT_EXPORT void registerInternalExtension(String extensionName, QuantumScriptExtensionInitProc extensionProc);

				QUANTUM_SCRIPT_EXPORT void initExecutive();
				QUANTUM_SCRIPT_EXPORT int execute();
				QUANTUM_SCRIPT_EXPORT int compileFile(const char *fileName);
				QUANTUM_SCRIPT_EXPORT int compileFileSkipLines(const char *fileName, size_t skipLines);
				QUANTUM_SCRIPT_EXPORT int includeAndExecuteFile(InstructionContext *context, const char *fileName);
				QUANTUM_SCRIPT_EXPORT int includeAndExecuteFileSkipLines(InstructionContext *context, const char *fileName, size_t skipLines);
				QUANTUM_SCRIPT_EXPORT void setVmFunction(const char *name, InstructionProcedure procedure, Variable *operand);

				QUANTUM_SCRIPT_EXPORT int compileString(const char *);
				QUANTUM_SCRIPT_EXPORT int compileStringSkipLines(const char *, size_t skipLines);
				QUANTUM_SCRIPT_EXPORT int includeAndExecuteString(InstructionContext *context, const char *);
				QUANTUM_SCRIPT_EXPORT int includeAndExecuteStringSkipLines(InstructionContext *context, const char *, size_t skipLines);
				QUANTUM_SCRIPT_EXPORT int setVmFunctionFromFile(const char *name, const char *fileName);
				QUANTUM_SCRIPT_EXPORT int setVmFunctionFromString(const char *name, const char *data);
				QUANTUM_SCRIPT_EXPORT int setVmFunctionFromFileX(InstructionContext *context, const char *name, const char *fileName);
				QUANTUM_SCRIPT_EXPORT int setVmFunctionFromStringX(InstructionContext *context, const char *name, const char *fileName);

				QUANTUM_SCRIPT_EXPORT void compileEnd();

				QUANTUM_SCRIPT_EXPORT int executeExtension(InstructionContext *context, QuantumScriptExtensionProc extensionProc, void *extensionId);

				QUANTUM_SCRIPT_EXPORT TPointer<Variable> callVmFunction(Variable *fnProcedure, Variable *fnThis, Variable *fnParameters);

				QUANTUM_SCRIPT_EXPORT void compileStringX(const char *);
				QUANTUM_SCRIPT_EXPORT void setFunction(const char *name, Variable *nativeFunction);
				QUANTUM_SCRIPT_EXPORT void setFunction2(const char *name, FunctionProcedure functionProcedure);
				QUANTUM_SCRIPT_EXPORT void setFunction3(const char *name, FunctionProcedure functionProcedure, Object *super);
				QUANTUM_SCRIPT_EXPORT void setFunction4(const char *name, FunctionProcedure functionProcedure, void *valueSuper);
				QUANTUM_SCRIPT_EXPORT void setFunction5(const char *name, FunctionProcedure functionProcedure, Object *super, void *valueSuper);
				QUANTUM_SCRIPT_EXPORT void setFunctionWithYield2(const char *name, FunctionProcedureWithYield functionProcedureWithYield);
				QUANTUM_SCRIPT_EXPORT void setFunctionWithYield3(const char *name, FunctionProcedureWithYield functionProcedureWithYield, Object *super);
				QUANTUM_SCRIPT_EXPORT void setFunctionWithYield4(const char *name, FunctionProcedureWithYield functionProcedureWithYield, void *valueSuper);
				QUANTUM_SCRIPT_EXPORT void setFunctionWithYield5(const char *name, FunctionProcedureWithYield functionProcedureWithYield, Object *super, void *valueSuper);

				QUANTUM_SCRIPT_EXPORT static void initMemory();

				QUANTUM_SCRIPT_EXPORT Variable *cloneVariable(SymbolList &inSymbolList, Variable *in);

				QUANTUM_SCRIPT_EXPORT void endProcessing();

				QUANTUM_SCRIPT_EXPORT void setIncludeSource(String fileName, String source);
				QUANTUM_SCRIPT_EXPORT void removeIncludeSource(String fileName);
		};

	};
};

#endif
