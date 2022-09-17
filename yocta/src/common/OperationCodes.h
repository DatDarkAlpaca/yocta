#pragma once
#include <cstdint>
#include <string>

namespace yo
{
	enum OPCode : uint8_t
	{
		None = 0U,
		OP_PUSH_LITERAL,
		OP_POP,

		OP_ADD,
		OP_SUB,
		OP_MULT,
		OP_DIV,
		OP_MOD,

		OP_INCREMENT,
		OP_DECREMENT,

		OP_NOT
	};

	inline std::string translateCode(const OPCode& code)
	{
		switch (code)
		{
			case OPCode::None:
				return "None";

			case OPCode::OP_PUSH_LITERAL:
				return "PUSH";

			case OPCode::OP_POP:
				return "POP";

			case OPCode::OP_ADD:
				return "ADD";

			case OPCode::OP_SUB:
				return "SUB";

			case OPCode::OP_MULT:
				return "MULT";

			case OPCode::OP_DIV:
				return "DIV";

			case OPCode::OP_MOD:
				return "MOD";

			case OPCode::OP_INCREMENT:
				return "INC";

			case OPCode::OP_DECREMENT:
				return "DEC";

			case OPCode::OP_NOT:
				return "NOT";
		}
		
		return "";
	}
}