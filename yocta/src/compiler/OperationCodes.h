#pragma once
#include <cstdint>
#include <string>

namespace yo
{
	enum OPCode : uint8_t
	{
		None = 0U,
		OP_PUSH_LITERAL,
		OP_ADD,
		OP_SUB,
		OP_MULT,
		OP_DIV,
		OP_MOD
	};

	inline std::string translateCode(const OPCode& code)
	{
		switch (code)
		{
			case OPCode::None:
				return "None";

			case OPCode::OP_PUSH_LITERAL:
				return "OP_PUSH";

			case OPCode::OP_ADD:
				return "OP_ADD";

			case OPCode::OP_SUB:
				return "OP_SUB";

			case OPCode::OP_MULT:
				return "OP_MULT";

			case OPCode::OP_DIV:
				return "OP_DIV";

			case OPCode::OP_MOD:
				return "OP_MOD";
		}
		
		return "";
	}
}