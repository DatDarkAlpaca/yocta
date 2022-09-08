#pragma once
#include "Chunk.h"

namespace yo
{
	class Disassembler
	{
	public:
		void operator()(const Chunk& array, const char* instructionSetName);

	private:
		unsigned int disassembleInstruction(const Chunk& array, int offset);

	private:
		unsigned int simpleInstruction(uint8_t code, int offset);

		unsigned int constantInstruction(uint8_t code, const Chunk& chunk, int offset);
	};
}

