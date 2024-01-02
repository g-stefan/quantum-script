// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_PARSERFUNCTIONHINT_HPP
#define XYO_QUANTUMSCRIPT_PARSERFUNCTIONHINT_HPP

namespace XYO::QuantumScript {

	class ParserFunctionHint {
		public:
			enum {
				None = 0x0000,
				LocalVariables = 0x0001,
				LocalVariablesLevel = 0x0002,
				Arguments = 0x0004,
				ArgumentsLevel = 0x0008,
				Yield = 0x0010,
				This = 0x0020,
				Self = 0x0040,
				All = 0x007F
			};
	};

};

#endif
