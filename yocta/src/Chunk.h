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
		void push_back(uint8_t opcode)
		{
			data.push_back(opcode);
		}

		void push_back(yocta_value constant)
		{
			constantPool.push_back(constant);
			int index = constantPool.size() - 1;
			data.push_back(index);
		}

		void clear()
		{
			data.clear();
			constantPool.clear();
		}

	public:
		std::vector<uint8_t> data;
		std::vector<yocta_value> constantPool;
	};
}