//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_PARSERFUNCTIONHINT_HPP
#define QUANTUM_SCRIPT_PARSERFUNCTIONHINT_HPP


namespace Quantum {
	namespace Script {


		class ParserFunctionHint {
			public:

				enum {
					None =                       0x0000,
					LocalVariables =             0x0001,
					LocalVariablesLevel =        0x0002,
					Arguments =                  0x0004,
					ArgumentsLevel =             0x0008,
					Yield =                      0x0010,
					This =                       0x0020,
					Self =                       0x0040,
					All =                        0x007F
				};

		};


	};
};


#endif
