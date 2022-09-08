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
		void push_back(uint8_t opcode, int lineNumber)
		{
			data.push_back(opcode);
			lines.push_back(lineNumber);
		}

		void push_back(yocta_value constant, int lineNumber)
		{
			constantPool.push_back(constant);
			size_t index = constantPool.size() - 1;

			data.push_back((uint8_t)index);
			lines.push_back(lineNumber);
		}

		void clear()
		{
			data.clear();
			constantPool.clear();
		}

	public:
		std::vector<int> lines;
		std::vector<uint8_t> data;
		std::vector<yocta_value> constantPool;
	};
}