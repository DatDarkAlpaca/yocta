#pragma once
#include <vector>

#include "OperationCodes.h"
#include "YoctaValue.h"

namespace yo
{
	class InstructionSet
	{
	public:
		void push_instruction(OPCode code)
		{
			instructions.push_back(code);
		}

		void push_back(double number)
		{
			instructions.push_back(OPCode::OP_PUSH_LITERAL);

			constants.push_back({ number });
			size_t index = constants.size() - 1;

			instructions.push_back((size_t)index);
		}

	public:
		std::vector<uint8_t> instructions;
		std::vector<YoctaValue> constants;
	};
}