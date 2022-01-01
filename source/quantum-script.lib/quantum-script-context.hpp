//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_CONTEXT_HPP
#define QUANTUM_SCRIPT_CONTEXT_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_PROTOTYPE_HPP
#include "quantum-script-prototype.hpp"
#endif

#ifndef QUANTUM_SCRIPT_SYMBOLLIST_HPP
#include "quantum-script-symbollist.hpp"
#endif

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class Context {
			public:
				//
				// SymbolList
				//
				QUANTUM_SCRIPT_EXPORT static Symbol getSymbol(String name);
				QUANTUM_SCRIPT_EXPORT static String getSymbolMirror(Symbol id);
				QUANTUM_SCRIPT_EXPORT static SymbolList &getSymbolList();

				//
				// Variables
				//
				QUANTUM_SCRIPT_EXPORT static TPointer<Variable> getValueUndefined();
				QUANTUM_SCRIPT_EXPORT static TPointer<Variable> getGlobalObject();

				QUANTUM_SCRIPT_EXPORT static Symbol getSymbolLength();
				QUANTUM_SCRIPT_EXPORT static Symbol getSymbolPrototype();
				QUANTUM_SCRIPT_EXPORT static Symbol getSymbolFunctionBoolean();
				QUANTUM_SCRIPT_EXPORT static Symbol getSymbolFunctionNumber();
				QUANTUM_SCRIPT_EXPORT static Symbol getSymbolFunctionString();
				QUANTUM_SCRIPT_EXPORT static Symbol getSymbolFunctionArray();
				QUANTUM_SCRIPT_EXPORT static Symbol getSymbolFunctionFunction();
				QUANTUM_SCRIPT_EXPORT static Symbol getSymbolFunctionObject();
				QUANTUM_SCRIPT_EXPORT static Symbol getSymbolFunctionAssociativeArray();
				QUANTUM_SCRIPT_EXPORT static Symbol getSymbolFunctionResource();
				QUANTUM_SCRIPT_EXPORT static Symbol getSymbolSize();
				QUANTUM_SCRIPT_EXPORT static Symbol getSymbolToNumber();
				QUANTUM_SCRIPT_EXPORT static Symbol getSymbolToString();

				QUANTUM_SCRIPT_EXPORT static TPointer<Prototype> getPrototypeBoolean();
				QUANTUM_SCRIPT_EXPORT static TPointer<Prototype> getPrototypeNumber();
				QUANTUM_SCRIPT_EXPORT static TPointer<Prototype> getPrototypeString();
				QUANTUM_SCRIPT_EXPORT static TPointer<Prototype> getPrototypeArray();
				QUANTUM_SCRIPT_EXPORT static TPointer<Prototype> getPrototypeFunction();
				QUANTUM_SCRIPT_EXPORT static TPointer<Prototype> getPrototypeObject();
				QUANTUM_SCRIPT_EXPORT static TPointer<Prototype> getPrototypeAssociativeArray();
				QUANTUM_SCRIPT_EXPORT static TPointer<Prototype> getPrototypeResource();

				QUANTUM_SCRIPT_EXPORT static void initMemory();
				//
				//
				//
				QUANTUM_SCRIPT_EXPORT static void newContext();
				QUANTUM_SCRIPT_EXPORT static void deleteContext();
		};

	};
};


#endif
