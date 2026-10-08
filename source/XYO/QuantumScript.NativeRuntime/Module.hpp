// Quantum Script Native Runtime
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_NATIVERUNTIME_MODULE_HPP
#define XYO_QUANTUMSCRIPT_NATIVERUNTIME_MODULE_HPP

#ifndef XYO_QUANTUMSCRIPT_NATIVERUNTIME_DEPENDENCY_HPP
#	include <XYO/QuantumScript.NativeRuntime/Dependency.hpp>
#endif

namespace XYO::QuantumScript::NativeRuntime {

	class ModuleString {
		public:
			const char *value;
			size_t length;
	};

	// The top level code of a unit, a function called with this = Script and
	// the arguments of the include. The main unit has no name, the other
	// units are included by name with Script.include(name).
	class ModuleUnit {
		public:
			const char *name;
			FunctionProcedure procedure;
	};

	// Constant description of a compiled module, generated code
	class ModuleInfo {
		public:
			int abi;
			const char *const *symbol;
			size_t symbolCount;
			const ModuleString *string;
			size_t stringCount;
			const Number *number;
			size_t numberCount;
			const char *const *source; // source of the code, as in a stack trace: "#file", "!tag"
			size_t sourceCount;
			const ModuleUnit *unit; // the first is the main unit
			size_t unitCount;
	};

	// The data of a compiled module in the thread of an engine:
	// symbols, constant values. The functions of the module own it.
	class Module : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Module);

		public:
			const ModuleInfo *info;
			Executive *executive;

			// owned by the context of the engine, not by the module:
			// the global object has the functions, the functions have the module
			Variable *global;
			Variable *undefined;
			Variable *valueTrue;
			Variable *valueFalse;

			Symbol *symbol;
			Variable **string;
			Variable **number;
			Symbol *source;

			Symbol symbolMessage;
			Symbol symbolStackTrace;
			Symbol symbolError;

			// for-in on a C++ local: the iterator sets the value here
			TPointer<VariableArray> iteratorValue;
			TPointer<Variable> iteratorReference;

			XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT Module();
			XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT ~Module();

			// the module of info for the engine of the current thread
			XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT static TPointer<Module> newModule(const ModuleInfo &info);

		protected:
			TPointer<Variable> *string_;
			TPointer<Variable> *number_;
	};

};

#endif
