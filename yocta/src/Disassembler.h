#pragma once
#include "InstructionArray.h"

namespace yo
{
	class Disassembler
	{
	public:
		void operator()(const InstructionArray& array, const char* instructionSetName);

	private:
		unsigned int disassembleInstruction(const InstructionArray& array, int offset);

	private:
		unsigned int simpleInstruction(const char* name, int offset);
	};
}

