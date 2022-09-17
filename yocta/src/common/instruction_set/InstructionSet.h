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

		void push_number_literal(double number)
		{
			instructions.push_back(OPCode::OP_PUSH_LITERAL);

			constants.push_back({ number });
			size_t index = constants.size() - 1;

			instructions.push_back((uint8_t)index);
		}

		void push_bool_literal(bool boolean)
		{
			instructions.push_back(OPCode::OP_PUSH_LITERAL);

			constants.push_back({ boolean });
			size_t index = constants.size() - 1;

			instructions.push_back((uint8_t)index);
		}

		void push_string_literal(std::string string)
		{
			instructions.push_back(OPCode::OP_PUSH_LITERAL);

			constants.push_back({ string });
			size_t index = constants.size() - 1;

			instructions.push_back((uint8_t)index);
		}

		void push_null_literal(NULL_TYPE null)
		{
			instructions.push_back(OPCode::OP_PUSH_LITERAL);

			constants.push_back({ null });
			size_t index = constants.size() - 1;

			instructions.push_back((uint8_t)index);
		}

		void push_value(YoctaValue value)
		{
			constants.push_back(value);
			size_t index = constants.size() - 1;

			instructions.push_back((uint8_t)index);
		}

	public:
		std::vector<uint8_t> instructions;
		std::vector<YoctaValue> constants;
	};
}