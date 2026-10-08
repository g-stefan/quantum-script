// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: the exit code set by Script.exit was ignored
// Used by fabricare/test.js, the process exit code must be 3

Script.setExitCode(1);
Script.exit(3);
Script.setExitCode(4);
