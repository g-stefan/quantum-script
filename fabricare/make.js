// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

if(Project.name=="quantum-script"){
	if(Project.make=="lib-and-dll"){
		Fabricare.include("make.quantum-script");
		return;
	};
};

if(!Fabricare.include("make."+Project.make)){
	messageError("Don't know how to make '"+Project.make+"'!");
	exit(1);
};

