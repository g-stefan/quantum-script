// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

messageAction("test");

// ---

for(var k=1;k<=9;++k){
	exitIfTest(Shell.execute("output/bin/quantum-script  --execution-time test/test.000"+k+".js"),"000"+k);
};

for(var k=10;k<=57;++k){
	exitIfTest(Shell.execute("output/bin/quantum-script  --execution-time test/test.00"+k+".js"),"00"+k);
};

// exit code set by Script.exit must be the process exit code
exitIfTest(Shell.execute("output/bin/quantum-script test/test.exit-code.js") != 3, "exit-code");

// --- bytecode: compile each test, run the bytecode

Shell.mkdirRecursivelyIfNotExists("temp/bytecode");
Shell.mkdirRecursivelyIfNotExists("temp/bytecode-strip");
Shell.mkdirRecursivelyIfNotExists("temp/bytecode-bundle");
// included by relative path, next to the including file
Shell.copyFile("test/test.0004.include.js", "temp/bytecode/test.0004.include.js");
Shell.copyFile("test/test.0052.include.js", "temp/bytecode/test.0052.include.js");
Shell.copyFile("test/test.0004.include.js", "temp/bytecode-strip/test.0004.include.js");
Shell.copyFile("test/test.0052.include.js", "temp/bytecode-strip/test.0052.include.js");

for(var k=1;k<=57;++k){
	var name = "test.00" + ((k<10)?"0":"") + k;
	exitIfTest(Shell.execute("output/bin/quantum-script --compile=temp/bytecode/" + name + ".qsbc test/" + name + ".js"), "bytecode compile " + name);
	exitIfTest(Shell.execute("output/bin/quantum-script temp/bytecode/" + name + ".qsbc"), "bytecode " + name);
	exitIfTest(Shell.execute("output/bin/quantum-script --compile=temp/bytecode-strip/" + name + ".qsbc --strip test/" + name + ".js"), "bytecode strip compile " + name);
	exitIfTest(Shell.execute("output/bin/quantum-script temp/bytecode-strip/" + name + ".qsbc"), "bytecode strip " + name);
};

exitIfTest(Shell.execute("output/bin/quantum-script --compile=temp/bytecode/test.exit-code.qsbc test/test.exit-code.js"), "bytecode compile exit-code");
exitIfTest(Shell.execute("output/bin/quantum-script temp/bytecode/test.exit-code.qsbc") != 3, "bytecode exit-code");

// bundle: the included file is a unit of the bytecode, not a file on disk
exitIfTest(Shell.execute("output/bin/quantum-script --compile=temp/bytecode-bundle/test.0004.qsbc --add=test.0004.include.js=test/test.0004.include.js test/test.0004.js"), "bytecode bundle compile");
exitIfTest(Shell.execute("output/bin/quantum-script temp/bytecode-bundle/test.0004.qsbc"), "bytecode bundle");

// invalid bytecode must not run
Shell.filePutContents("temp/bytecode/invalid.qsbc", "QSBC invalid");
exitIfTest(Shell.execute("output/bin/quantum-script temp/bytecode/invalid.qsbc") == 0, "bytecode invalid");

// --- C++: each test translated to C++ for the native runtime (inline and runtime calls only), all in one executable, one test per run

Shell.mkdirRecursivelyIfNotExists("temp/cpp/bin");
Shell.mkdirRecursivelyIfNotExists("temp/cpp/run");
// included by name from the current folder, the executable folder has no script
Shell.copyFile("test/test.0004.include.js", "temp/cpp/run/test.0004.include.js");
Shell.copyFile("test/test.0052.include.js", "temp/cpp/run/test.0052.include.js");

var cppTest = [];
var cppCompile = function(name, options, script) {
	exitIfTest(Shell.execute("output/bin/quantum-script --compile-cpp=temp/cpp/" + name + ".cpp " + options + script), "cpp compile " + name);
	cppTest[cppTest.length] = name;
};

for(var k=1;k<=57;++k){
	var name = "test.00" + ((k<10)?"0":"") + k;
	cppCompile(name, "", "test/" + name + ".js");
	cppCompile(name + ".call", "--cpp-no-inline ", "test/" + name + ".js");
};
cppCompile("test.exit-code", "", "test/test.exit-code.js");
// from bytecode, without source
cppCompile("test.0004.strip", "", "temp/bytecode-strip/test.0004.qsbc");
// bundle: the included file is a compiled unit
cppCompile("test.0004.bundle", "--add=test.0004.include.js=test/test.0004.include.js ", "test/test.0004.js");

// the namespace of the generated code is from the file name
var cppNamespace = function(name) {
	return "QuantumScriptCompiled_" + name.replace(".", "_").replace("-", "_");
};

var cppMain = "#include <string.h>\n";
cppMain += "#include <XYO/QuantumScript.hpp>\n";
cppMain += "#include <XYO/QuantumScript.Extension/Console.hpp>\n\n";
for(var name of cppTest) {
	cppMain += "namespace " + cppNamespace(name) + " {\n";
	cppMain += "\tint run(int cmdN, char *cmdS[], XYO::QuantumScript::QuantumScriptInitExecutiveProc initExecutive);\n";
	cppMain += "};\n";
};
cppMain += "\nusing namespace XYO::QuantumScript;\n\n";
cppMain += "static void initExecutive(Executive *executive) {\n";
cppMain += "\tExtension::Console::registerInternalExtension(executive);\n";
cppMain += "};\n\n";
cppMain += "class Application : public virtual IApplication {\n";
cppMain += "\tpublic:\n";
cppMain += "\t\tint main(int cmdN, char *cmdS[]) {\n";
cppMain += "\t\t\tif (cmdN < 2) {\n";
cppMain += "\t\t\t\treturn 1;\n";
cppMain += "\t\t\t};\n";
for(var name of cppTest) {
	cppMain += "\t\t\tif (strcmp(cmdS[1], \"" + name + "\") == 0) {\n";
	cppMain += "\t\t\t\treturn " + cppNamespace(name) + "::run(cmdN, cmdS, initExecutive);\n";
	cppMain += "\t\t\t};\n";
};
cppMain += "\t\t\treturn 1;\n";
cppMain += "\t\t};\n";
cppMain += "};\n\n";
cppMain += "XYO_APPLICATION_MAIN(Application);\n";
Shell.filePutContents("temp/cpp/test-cpp.cpp", cppMain);

var cppSource = ["temp/cpp/test-cpp.cpp"];
for(var name of cppTest) {
	cppSource[cppSource.length] = "temp/cpp/" + name + ".cpp";
};

Project = {
	name: "test-cpp",
	dependency: ["quantum-script.native-runtime", "quantum-script--console", "quantum-script"],
	outputPath: "temp/cpp/bin"
};
compileExe({
	project: "test-cpp",
	defines: ["XYO_QUANTUMSCRIPT_COMPILED_NO_MAIN"],
	includePath: ["source"],
	hppSource: [],
	cppSource: cppSource,
	libraryPath: ["output/lib"]
});

addOutputBinToPath();

var cppExecutable = "../bin/test-cpp";
if (OS.isWindows()) {
	cppExecutable = "..\\bin\\test-cpp";
};

runInPath("temp/cpp/run", function() {
	for(var name of cppTest) {
		if ((name == "test.exit-code") || (name == "test.0004.bundle")) {
			continue;
		};
		exitIfTest(Shell.execute(cppExecutable + " " + name), "cpp " + name);
	};
	exitIfTest(Shell.execute(cppExecutable + " test.exit-code") != 3, "cpp exit-code");
});

runInPath("temp/cpp/bin", function() {
	exitIfTest(Shell.execute(cppExecutable + " test.0004.bundle"), "cpp bundle");
});
