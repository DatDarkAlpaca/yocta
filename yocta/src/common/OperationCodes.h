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

		OP_DEFINE_GLOBAL,
		OP_GET_GLOBAL,
		OP_SET_GLOBAL,

		OP_ADD,
		OP_SUB,
		OP_MULT,
		OP_DIV,
		OP_MOD,

		OP_INCREMENT,
		OP_DECREMENT,

		OP_BITNOT,
		OP_BITAND,
		OP_BITOR,
		OP_BITXOR,
		OP_BITSHIFT_LEFT,
		OP_BITSHIFT_RIGHT,

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

			case OPCode::OP_DEFINE_GLOBAL:
				return "DEF_GL";

			case OPCode::OP_GET_GLOBAL:
				return "GET_GL";

			case OPCode::OP_SET_GLOBAL:
				return "SET_GL";

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


			case OPCode::OP_BITNOT:
				return "B_NOT";

			case OPCode::OP_BITAND:
				return "B_AND";

			case OPCode::OP_BITOR:
				return "B_OR";

			case OPCode::OP_BITXOR:
				return "B_XOR";

			case OPCode::OP_BITSHIFT_LEFT:
				return "BSH_LEFT";

			case OPCode::OP_BITSHIFT_RIGHT:
				return "BSH_RIGHT";


			case OPCode::OP_NOT:
				return "NOT";
		}
		
		return "";
	}
}