// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXECUTIVE_HPP
#define XYO_QUANTUMSCRIPT_EXECUTIVE_HPP

#ifndef XYO_QUANTUMSCRIPT_PARSER_HPP
#	include <XYO/QuantumScript/Parser.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_ASM_HPP
#	include <XYO/QuantumScript/Asm.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_VARIABLEFUNCTIONWITHYIELD_HPP
#	include <XYO/QuantumScript/VariableFunctionWithYield.hpp>
#endif

extern "C" typedef void (*QuantumScriptExtensionProc)(void *, void *);

namespace XYO::QuantumScript {

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
			XYO_QUANTUMSCRIPT_EXPORT void errorInfoInit();
			XYO_QUANTUMSCRIPT_EXPORT int execute_(InstructionContext *);
			ProgramCounter *currentContextExit;
			ProgramCounter *currentContextReturn;
			ProgramCounter *currentContextThrow;
			ProgramCounter *currentContextContinue;
			ProgramCounter *currentContextBreak;

		public:
			int mainCmdN;
			char **mainCmdS;
			String pathExecutable;
			int exitCode;
			bool isExit;

			TPointer<TDoubleEndedQueue<String>> includePath;
			TPointer<TDoubleEndedQueue<Extension_>> extensionList;
			TPointer<TDoubleEndedQueue<InternalExtension_>> internalExtensionList;
			TPointer<TRedBlackTree<String, String>> includeSource;

			QuantumScriptInitExecutiveProc applicationInitExecutive;

			TPointer<InstructionContext> instructionContext;
			int configPrintStackTraceLimit;

			TPointer<Asm> assembler;
			ErrorInfo errorInfo;

			XYO_QUANTUMSCRIPT_EXPORT Executive();

			XYO_QUANTUMSCRIPT_EXPORT ~Executive();

			XYO_QUANTUMSCRIPT_EXPORT bool isExtensionLoaded(String extensionName);
			XYO_QUANTUMSCRIPT_EXPORT void setExtensionName(void *extensionId, String name);
			XYO_QUANTUMSCRIPT_EXPORT void setExtensionInfo(void *extensionId, String info);
			XYO_QUANTUMSCRIPT_EXPORT void setExtensionVersion(void *extensionId, String version);
			XYO_QUANTUMSCRIPT_EXPORT void setExtensionPublic(void *extensionId, bool isPublic);
			XYO_QUANTUMSCRIPT_EXPORT void setExtensionDeleteContext(void *extensionId, QuantumScriptExtensionDeleteContextProc deleteContext);
			XYO_QUANTUMSCRIPT_EXPORT void initExtension(QuantumScriptExtensionInitProc extensionProc);
			XYO_QUANTUMSCRIPT_EXPORT void registerInternalExtension(String extensionName, QuantumScriptExtensionInitProc extensionProc);

			XYO_QUANTUMSCRIPT_EXPORT void initExecutive();
			XYO_QUANTUMSCRIPT_EXPORT int execute();
			XYO_QUANTUMSCRIPT_EXPORT int compileFile(const char *fileName);
			XYO_QUANTUMSCRIPT_EXPORT int compileFileSkipLines(const char *fileName, size_t skipLines);
			XYO_QUANTUMSCRIPT_EXPORT int includeAndExecuteFile(InstructionContext *context, const char *fileName);
			XYO_QUANTUMSCRIPT_EXPORT int includeAndExecuteFileSkipLines(InstructionContext *context, const char *fileName, size_t skipLines);
			XYO_QUANTUMSCRIPT_EXPORT void setVmFunction(const char *name, InstructionProcedure procedure, Variable *operand);

			XYO_QUANTUMSCRIPT_EXPORT int compileString(const char *);
			XYO_QUANTUMSCRIPT_EXPORT int compileStringSkipLines(const char *, size_t skipLines);
			XYO_QUANTUMSCRIPT_EXPORT int includeAndExecuteString(InstructionContext *context, const char *);
			XYO_QUANTUMSCRIPT_EXPORT int includeAndExecuteStringSkipLines(InstructionContext *context, const char *, size_t skipLines);
			XYO_QUANTUMSCRIPT_EXPORT int setVmFunctionFromFile(const char *name, const char *fileName);
			XYO_QUANTUMSCRIPT_EXPORT int setVmFunctionFromString(const char *name, const char *data);
			XYO_QUANTUMSCRIPT_EXPORT int setVmFunctionFromFileX(InstructionContext *context, const char *name, const char *fileName);
			XYO_QUANTUMSCRIPT_EXPORT int setVmFunctionFromStringX(InstructionContext *context, const char *name, const char *fileName);

			XYO_QUANTUMSCRIPT_EXPORT void compileEnd();

			XYO_QUANTUMSCRIPT_EXPORT int executeExtension(InstructionContext *context, QuantumScriptExtensionProc extensionProc, void *extensionId);

			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> callVmFunction(Variable *fnProcedure, Variable *fnThis, Variable *fnParameters);

			XYO_QUANTUMSCRIPT_EXPORT void compileStringX(const char *);
			XYO_QUANTUMSCRIPT_EXPORT void setFunction(const char *name, Variable *nativeFunction);
			XYO_QUANTUMSCRIPT_EXPORT void setFunction2(const char *name, FunctionProcedure functionProcedure);
			XYO_QUANTUMSCRIPT_EXPORT void setFunction3(const char *name, FunctionProcedure functionProcedure, Object *super);
			XYO_QUANTUMSCRIPT_EXPORT void setFunction4(const char *name, FunctionProcedure functionProcedure, void *valueSuper);
			XYO_QUANTUMSCRIPT_EXPORT void setFunction5(const char *name, FunctionProcedure functionProcedure, Object *super, void *valueSuper);
			XYO_QUANTUMSCRIPT_EXPORT void setFunctionWithYield2(const char *name, FunctionProcedureWithYield functionProcedureWithYield);
			XYO_QUANTUMSCRIPT_EXPORT void setFunctionWithYield3(const char *name, FunctionProcedureWithYield functionProcedureWithYield, Object *super);
			XYO_QUANTUMSCRIPT_EXPORT void setFunctionWithYield4(const char *name, FunctionProcedureWithYield functionProcedureWithYield, void *valueSuper);
			XYO_QUANTUMSCRIPT_EXPORT void setFunctionWithYield5(const char *name, FunctionProcedureWithYield functionProcedureWithYield, Object *super, void *valueSuper);

			XYO_QUANTUMSCRIPT_EXPORT static void initMemory();

			XYO_QUANTUMSCRIPT_EXPORT Variable *cloneVariable(SymbolList &inSymbolList, Variable *in);

			XYO_QUANTUMSCRIPT_EXPORT void endProcessing();

			XYO_QUANTUMSCRIPT_EXPORT void setIncludeSource(String fileName, String source);
			XYO_QUANTUMSCRIPT_EXPORT void removeIncludeSource(String fileName);
			XYO_QUANTUMSCRIPT_EXPORT bool hasIncludeSource(String fileName);
	};

};

#endif
