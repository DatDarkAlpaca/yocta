#pragma once
#include <cstdint>
#include <algorithm>

namespace yo
{
	enum class OPCode : uint8_t { None = 0U, OP_RETURN };

	class InstructionArray
	{
	public:
		InstructionArray(unsigned int capacity = 0);

	public:
		void push_back(uint8_t instruction);

		void clear();

	private:
		void reallocateArray(unsigned int oldSize, unsigned int size);

	public:
		unsigned int count = 0, capacity = 0;
		uint8_t* data = nullptr;
	};
}