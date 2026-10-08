// Quantum Script Native Runtime
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.NativeRuntime/Copyright.hpp>
#include <XYO/QuantumScript.NativeRuntime/Copyright.rh>

namespace XYO::QuantumScript::NativeRuntime::Copyright {

	static const char *copyright_ = XYO_QUANTUMSCRIPT_NATIVERUNTIME_COPYRIGHT;
	static const char *publisher_ = XYO_QUANTUMSCRIPT_NATIVERUNTIME_PUBLISHER;
	static const char *company_ = XYO_QUANTUMSCRIPT_NATIVERUNTIME_COMPANY;
	static const char *contact_ = XYO_QUANTUMSCRIPT_NATIVERUNTIME_CONTACT;

	const char *copyright() {
		return copyright_;
	};

	const char *publisher() {
		return publisher_;
	};

	const char *company() {
		return company_;
	};

	const char *contact() {
		return contact_;
	};

};
