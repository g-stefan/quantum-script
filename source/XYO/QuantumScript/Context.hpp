// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_CONTEXT_HPP
#define XYO_QUANTUMSCRIPT_CONTEXT_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_PROTOTYPE_HPP
#	include <XYO/QuantumScript/Prototype.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_SYMBOLLIST_HPP
#	include <XYO/QuantumScript/SymbolList.hpp>
#endif

namespace XYO::QuantumScript {

	class Context {
		public:
			//
			// SymbolList
			//
			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbol(String name);
			XYO_QUANTUMSCRIPT_EXPORT static String getSymbolMirror(Symbol id);
			XYO_QUANTUMSCRIPT_EXPORT static SymbolList &getSymbolList();

			//
			// Variables
			//
			XYO_QUANTUMSCRIPT_EXPORT static TPointer<Variable> getValueUndefined();
			XYO_QUANTUMSCRIPT_EXPORT static TPointer<Variable> getGlobalObject();

			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbolLength();
			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbolPrototype();
			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbolFunctionBoolean();
			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbolFunctionNumber();
			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbolFunctionString();
			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbolFunctionArray();
			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbolFunctionFunction();
			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbolFunctionObject();
			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbolFunctionAssociativeArray();
			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbolFunctionResource();
			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbolSize();
			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbolToNumber();
			XYO_QUANTUMSCRIPT_EXPORT static Symbol getSymbolToString();

			XYO_QUANTUMSCRIPT_EXPORT static TPointer<Prototype> getPrototypeBoolean();
			XYO_QUANTUMSCRIPT_EXPORT static TPointer<Prototype> getPrototypeNumber();
			XYO_QUANTUMSCRIPT_EXPORT static TPointer<Prototype> getPrototypeString();
			XYO_QUANTUMSCRIPT_EXPORT static TPointer<Prototype> getPrototypeArray();
			XYO_QUANTUMSCRIPT_EXPORT static TPointer<Prototype> getPrototypeFunction();
			XYO_QUANTUMSCRIPT_EXPORT static TPointer<Prototype> getPrototypeObject();
			XYO_QUANTUMSCRIPT_EXPORT static TPointer<Prototype> getPrototypeAssociativeArray();
			XYO_QUANTUMSCRIPT_EXPORT static TPointer<Prototype> getPrototypeResource();

			XYO_QUANTUMSCRIPT_EXPORT static void initMemory();
			//
			//
			//
			XYO_QUANTUMSCRIPT_EXPORT static void newContext();
			XYO_QUANTUMSCRIPT_EXPORT static void deleteContext();
	};

};

#endif
