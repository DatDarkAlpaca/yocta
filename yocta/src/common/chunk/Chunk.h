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

		void push_constant(YoctaValue constant, int lineNumber);

		void clear();

	public:
		std::vector<int> lines;
		std::vector<uint8_t> data;
		std::vector<YoctaValue> constantPool;
	};
}