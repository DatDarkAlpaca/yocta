#pragma once
#include "compilerDisassembler.h"
#include "InstructionSet.h"
#include "Debug.h"

#include <unordered_map>

namespace yo
{
	class VirtualMachine
	{
	public:
		VirtualMachine() = default;

	public:
		void execute(InstructionSet& set);

	private:
		void handleBinaryOperation(OPCode code);

	private:
		YoctaValue peek() { return m_VMStack.back(); }

		uint8_t readByte() { return *m_IP++; }

		YoctaValue readYoctaValue(InstructionSet& set) { return set.constants[readByte()]; }

	private:
		std::unordered_map<std::string, YoctaValue> m_Globals;
		std::vector<YoctaValue> m_VMStack;
		uint8_t* m_IP = nullptr;
	};
}