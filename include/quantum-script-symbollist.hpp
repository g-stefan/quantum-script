//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_SYMBOLLIST_HPP
#define QUANTUM_SCRIPT_SYMBOLLIST_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class SymbolList :
			public Object {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(SymbolList);
			public:

				typedef TRedBlackTree<String, Symbol, TMemoryPoolActive> XSymbolList;
				typedef TRedBlackTree<Symbol, String, TMemoryPoolActive> XSymbolListMirror;

				Symbol lastId;
				XSymbolList symbolList;
				XSymbolListMirror symbolListMirror;

				QUANTUM_SCRIPT_EXPORT SymbolList();

				QUANTUM_SCRIPT_EXPORT Symbol getSymbol(String name);
				QUANTUM_SCRIPT_EXPORT String getSymbolMirror(Symbol id);

				QUANTUM_SCRIPT_EXPORT static void initMemory();
		};


	};
};



#endif
