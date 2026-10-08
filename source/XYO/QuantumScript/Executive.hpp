// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXECUTIVE_HPP
#define XYO_QUANTUMSCRIPT_EXECUTIVE_HPP

#ifndef XYO_QUANTUMSCRIPT_PARSER_HPP
#	include <XYO/QuantumScript/Parser.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_ASM_HPP
#	include <XYO/QuantumScript/Asm.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_BYTECODE_HPP
#	include <XYO/QuantumScript/Bytecode.hpp>
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
				FileNotFound,
				Bytecode // invalid bytecode file, ErrorInfo::bytecodeError
			};
	};

	class ErrorInfo {
		public:
			int compileError;
			uint32_t compileLineNumber;
			const char *compileFileName;
			String bytecodeError;
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

	class ExecutiveInclude {
		public:
			AsmLabel linkBegin;
			AsmLabel linkStart;
			AsmLabel linkFinally;
	};

	class Executive : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Executive);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT void errorInfoInit();
			XYO_QUANTUMSCRIPT_EXPORT void includeBegin(ExecutiveInclude &include);
			XYO_QUANTUMSCRIPT_EXPORT void includeEnd(InstructionContext *context, ExecutiveInclude &include, bool clearIncludedFile);
			XYO_QUANTUMSCRIPT_EXPORT void includeAbort(ExecutiveInclude &include);
			XYO_QUANTUMSCRIPT_EXPORT size_t assembleNameReference(const char *name);
			XYO_QUANTUMSCRIPT_EXPORT int compileFile_(Asm *target, const char *fileName, size_t skipLines);
			XYO_QUANTUMSCRIPT_EXPORT int compileString_(Asm *target, const char *data_, size_t skipLines, const char *tag);
			XYO_QUANTUMSCRIPT_EXPORT int includeAndExecuteBytecodeFile_(InstructionContext *context, const char *fileName);
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
			// named units, included by name, from bytecode bundles
			TPointer<CodeUnitMap> includeUnit;
			// named units as functions (compiled code), included by name:
			// called with the this and the arguments of the include, the result is the result of the include
			TPointer<TRedBlackTree<String, TPointer<Variable>>> includeFunction;

			QuantumScriptInitExecutiveProc applicationInitExecutive;

			TPointer<InstructionContext> instructionContext;
			int configPrintStackTraceLimit;

			TPointer<Asm> assembler;
			// linked code, the assembler output is added by the linker
			TPointer<InstructionList> instructionList;
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

			XYO_QUANTUMSCRIPT_EXPORT int compileString(const char *, const char *tag = nullptr);
			XYO_QUANTUMSCRIPT_EXPORT int compileStringSkipLines(const char *, size_t skipLines, const char *tag = nullptr);
			XYO_QUANTUMSCRIPT_EXPORT int includeAndExecuteString(InstructionContext *context, const char *, const char *tag = nullptr);
			XYO_QUANTUMSCRIPT_EXPORT int includeAndExecuteStringSkipLines(InstructionContext *context, const char *, size_t skipLines, const char *tag = nullptr);
			XYO_QUANTUMSCRIPT_EXPORT int setVmFunctionFromFile(const char *name, const char *fileName);
			XYO_QUANTUMSCRIPT_EXPORT int setVmFunctionFromString(const char *name, const char *data, const char *tag = nullptr);
			XYO_QUANTUMSCRIPT_EXPORT int setVmFunctionFromFileX(InstructionContext *context, const char *name, const char *fileName);
			XYO_QUANTUMSCRIPT_EXPORT int setVmFunctionFromStringX(InstructionContext *context, const char *name, const char *data, const char *tag = nullptr);

			XYO_QUANTUMSCRIPT_EXPORT void compileEnd();

			// compile to a separate unit, not linked, to save as bytecode
			XYO_QUANTUMSCRIPT_EXPORT int compileFileToUnit(const char *fileName, CodeUnit &unit, size_t skipLines = 0);
			XYO_QUANTUMSCRIPT_EXPORT int compileStringToUnit(const char *data_, CodeUnit &unit, const char *tag = nullptr);
			// run a unit (compiled or loaded) as included code
			XYO_QUANTUMSCRIPT_EXPORT int includeAndExecuteUnit(InstructionContext *context, CodeUnit &unit);
			// a new script function with unit as its code, see Linker::unlinkFunction
			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> newFunctionFromUnit(CodeUnit &unit, int functionHint = ParserFunctionHint::All);

			XYO_QUANTUMSCRIPT_EXPORT int executeExtension(InstructionContext *context, QuantumScriptExtensionProc extensionProc, void *extensionId);

			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> callVmFunction(Variable *fnProcedure, Variable *fnThis, Variable *fnParameters);

			XYO_QUANTUMSCRIPT_EXPORT void compileStringX(const char *, const char *tag = nullptr);
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

			XYO_QUANTUMSCRIPT_EXPORT void setIncludeUnit(String name, CodeUnit *unit);
			XYO_QUANTUMSCRIPT_EXPORT void removeIncludeUnit(String name);
			XYO_QUANTUMSCRIPT_EXPORT bool hasIncludeUnit(String name);

			XYO_QUANTUMSCRIPT_EXPORT void setIncludeFunction(String name, Variable *function);
			XYO_QUANTUMSCRIPT_EXPORT void removeIncludeFunction(String name);
			XYO_QUANTUMSCRIPT_EXPORT bool hasIncludeFunction(String name);

		protected:
			// reuse instruction context for callVmFunction, native code calling script functions
			TStack<TPointer<InstructionContext>> callContextPool;
			size_t callContextPoolSize;

			XYO_QUANTUMSCRIPT_EXPORT void callContextRelease(TPointer<InstructionContext> &fnContext);
	};

};

#endif
