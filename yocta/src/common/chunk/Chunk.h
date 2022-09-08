#pragma once
#include <cstdint>
#include <vector>

#include "OperationCodes.h"
#include "Value.h"

namespace yo
{
	class Chunk
	{
	public:
		Chunk() = default;

	public:
		void push_back(uint8_t opcode, int lineNumber);

		void push_constant(yocta_value constant, int lineNumber);

		void clear();

	public:
		std::vector<int> lines;
		std::vector<uint8_t> data;
		std::vector<yocta_value> constantPool;
	};
}