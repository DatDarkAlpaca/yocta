#pragma once
#include "Chunk.h"

namespace yo
{
	class Disassembler
	{
	public:
		static void disassemble(const Chunk& array, const char* instructionSetName);

		static unsigned int disassembleInstruction(const Chunk& array, int offset);

	private:
		static unsigned int simpleInstruction(uint8_t code, int offset);

		static unsigned int constantInstruction(uint8_t code, const Chunk& chunk, int offset);

		static unsigned int byteInstruction(uint8_t code, const Chunk& chunk, int offset);

		static unsigned int jumpInstruction(uint8_t code, int sign, const Chunk& chunk, int offset);
	};
}

