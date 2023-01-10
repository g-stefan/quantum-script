// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/SymbolList.hpp>

namespace XYO::QuantumScript {

	void SymbolList::initMemory() {
		TMemory<XSymbolList>::initMemory();
		TMemory<XSymbolListMirror>::initMemory();
	};

	SymbolList::SymbolList() {
		lastId = 0;
	};

	Symbol SymbolList::getSymbol(String name) {
		Symbol id;
		if (symbolList.get(name, id)) {
			return id;
		};
		++lastId;
		symbolList.set(name, lastId);
		symbolListMirror.set(lastId, name);
		return lastId;
	};

	String SymbolList::getSymbolMirror(Symbol id) {
		XSymbolListMirror::Node *tmp = symbolListMirror.find(id);
		if (tmp) {
			return tmp->value;
		};
		throw(Error("getSymbolMirror"));
	};

};
