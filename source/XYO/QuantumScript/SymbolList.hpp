// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_SYMBOLLIST_HPP
#define XYO_QUANTUMSCRIPT_SYMBOLLIST_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

namespace XYO::QuantumScript {

	class SymbolList : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(SymbolList);

		public:
			typedef TRedBlackTree<String, Symbol, TMemoryPoolActive> XSymbolList;
			typedef TRedBlackTree<Symbol, String, TMemoryPoolActive> XSymbolListMirror;

			Symbol lastId;
			XSymbolList symbolList;
			XSymbolListMirror symbolListMirror;

			XYO_QUANTUMSCRIPT_EXPORT SymbolList();

			XYO_QUANTUMSCRIPT_EXPORT Symbol getSymbol(String name);
			// find only, a symbol that does not exist is not created
			XYO_QUANTUMSCRIPT_EXPORT bool findSymbol(const String &name, Symbol &out);
			XYO_QUANTUMSCRIPT_EXPORT String getSymbolMirror(Symbol id);

			XYO_QUANTUMSCRIPT_EXPORT static void initMemory();
	};

};

#endif
