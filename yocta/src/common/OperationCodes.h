#pragma once
#include <cstdint>

namespace yo
{
	enum class OPCode : uint8_t
	{
		None = 0U,
		OP_NONE,
		OP_TRUE,
		OP_FALSE,
		OP_RETURN,
		OP_CONSTANT,
		OP_NEGATE,
		OP_ADD,
		OP_SUB,
		OP_MULT,
		OP_DIV,
		OP_NOT
	};

	inline const char* translateCode(const OPCode& code)
	{
		switch (code)
		{
			case OPCode::None:
				return "None";

			case OPCode::OP_NONE:
				return "OP_NONE";

			case OPCode::OP_TRUE:
				return "OP_TRUE";

			case OPCode::OP_FALSE:
				return "OP_FALSE";

			case OPCode::OP_RETURN:
				return "OP_RETURN";

			case OPCode::OP_CONSTANT:
				return "OP_CONSTANT";

			case OPCode::OP_NEGATE:
				return "OP_NEGATE";

			case OPCode::OP_ADD:
				return "OP_ADD";

			case OPCode::OP_SUB:
				return "OP_SUB";

			case OPCode::OP_MULT:
				return "OP_MULT";

			case OPCode::OP_DIV:
				return "OP_DIV";

			case OPCode::OP_NOT:
				return "OP_NOT";
		}

		return "";
	}
}