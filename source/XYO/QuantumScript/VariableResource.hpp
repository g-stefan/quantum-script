// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLERESOURCE_HPP
#define XYO_QUANTUMSCRIPT_VARIABLERESOURCE_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableResource;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableResource> : public TMemoryPoolActive<XYO::QuantumScript::VariableResource> {};
};

namespace XYO::QuantumScript {

	class VariableResource : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableResource);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableResource);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeResource;

		public:
			typedef void (*ResourceDelete)(void *);

			void *resource;
			ResourceDelete resourceDelete;

			XYO_QUANTUMSCRIPT_EXPORT VariableResource();

			inline void drop() {
				resource = nullptr;
				resourceDelete = nullptr;
			};

			inline void close() {
				if (resource != nullptr) {
					if (resourceDelete != nullptr) {
						(*resourceDelete)(resource);
						resourceDelete = nullptr;
					};
					resource = nullptr;
				};
			};

			inline void activeDestructor() {
				close();
			};

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable(void *resource, ResourceDelete resourceDelete);

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT String toString();
	};

};

#endif
