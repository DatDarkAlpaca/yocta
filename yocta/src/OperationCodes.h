#pragma once
#include <cstdint>

namespace yo
{
	enum class OPCode : uint8_t
	{
		None = 0U,
		OP_RETURN,
		OP_CONSTANT
	};

	inline const char* translateCode(const OPCode& code)
	{
		switch (code)
		{
			case OPCode::None:
				return "None";

			case OPCode::OP_RETURN:
				return "OP_RETURN";

			case OPCode::OP_CONSTANT:
				return "OP_CONSTANT";
		}

		return "";
	}
}