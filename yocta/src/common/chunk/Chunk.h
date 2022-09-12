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

		void push_constant(Value value, int lineNumber);

		void push_constant_only(Value value)
		{
			constantPool.push_back(value);
		}

		void clear();

	public:
		std::vector<int> lines;
		std::vector<uint8_t> data;
		std::vector<Value> constantPool;
	};
}