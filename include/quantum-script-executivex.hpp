//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXECUTIVEX_HPP
#define QUANTUM_SCRIPT_EXECUTIVEX_HPP

#ifndef QUANTUM_SCRIPT_EXECUTIVE_HPP
#include "quantum-script-executive.hpp"
#endif

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class ExecutiveX {
				friend class Executive;
			protected:
				QUANTUM_SCRIPT_EXPORT static void setStackTrace(String stackTrace);
			public:
				QUANTUM_SCRIPT_EXPORT static bool initExecutive(int cmdN, char **cmdS, QuantumScriptInitExecutiveProc applicationInitExecutive);
				QUANTUM_SCRIPT_EXPORT static String getError();
				QUANTUM_SCRIPT_EXPORT static String getStackTrace();
				QUANTUM_SCRIPT_EXPORT static Executive &getExecutive();
				QUANTUM_SCRIPT_EXPORT static bool executeFile(const char *fileName);
				QUANTUM_SCRIPT_EXPORT static bool executeString(const char *strSource);
				QUANTUM_SCRIPT_EXPORT static TPointer<Variable> returnValue();
				QUANTUM_SCRIPT_EXPORT static void endProcessing();
				QUANTUM_SCRIPT_EXPORT static void setIncludeSource(String fileName, String source);
				QUANTUM_SCRIPT_EXPORT static void removeIncludeSource(String fileName);
				QUANTUM_SCRIPT_EXPORT static bool executeFileSkipLines(const char *fileName, size_t skipLines);
				QUANTUM_SCRIPT_EXPORT static bool executeStringSkipLines(const char *strSource, size_t skipLines);
				QUANTUM_SCRIPT_EXPORT static void includePath(const char *includePath_);
		};

	};
};


#endif

