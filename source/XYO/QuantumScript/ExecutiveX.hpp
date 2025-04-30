// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXECUTIVEX_HPP
#define XYO_QUANTUMSCRIPT_EXECUTIVEX_HPP

#ifndef XYO_QUANTUMSCRIPT_EXECUTIVE_HPP
#	include <XYO/QuantumScript/Executive.hpp>
#endif

namespace XYO::QuantumScript {

	class ExecutiveX {
			friend class Executive;

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static void setStackTrace(String stackTrace);

		public:
			XYO_QUANTUMSCRIPT_EXPORT static bool initExecutive(int cmdN, char **cmdS, QuantumScriptInitExecutiveProc applicationInitExecutive);
			XYO_QUANTUMSCRIPT_EXPORT static String getError();
			XYO_QUANTUMSCRIPT_EXPORT static String getStackTrace();
			XYO_QUANTUMSCRIPT_EXPORT static Executive &getExecutive();
			XYO_QUANTUMSCRIPT_EXPORT static bool executeFile(const char *fileName);
			XYO_QUANTUMSCRIPT_EXPORT static bool executeString(const char *strSource);
			XYO_QUANTUMSCRIPT_EXPORT static TPointer<Variable> returnValue();
			XYO_QUANTUMSCRIPT_EXPORT static void endProcessing();
			XYO_QUANTUMSCRIPT_EXPORT static void setIncludeSource(String fileName, String source);
			XYO_QUANTUMSCRIPT_EXPORT static void removeIncludeSource(String fileName);
			XYO_QUANTUMSCRIPT_EXPORT static bool executeFileSkipLines(const char *fileName, size_t skipLines);
			XYO_QUANTUMSCRIPT_EXPORT static bool executeStringSkipLines(const char *strSource, size_t skipLines);
			XYO_QUANTUMSCRIPT_EXPORT static void includePath(const char *includePath_);
			XYO_QUANTUMSCRIPT_EXPORT static bool isExit();
			XYO_QUANTUMSCRIPT_EXPORT static int getExitCode();
	};

};

#endif
