// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

messageAction("test");

// ---

for(var k=1;k<=9;++k){
	exitIfTest(Shell.execute("output/bin/quantum-script  --execution-time test/test.000"+k+".js"),"000"+k);
};

for(var k=10;k<=36;++k){
	exitIfTest(Shell.execute("output/bin/quantum-script  --execution-time test/test.00"+k+".js"),"00"+k);
};
