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
		OP_NOT,
		OP_EQUAL,
		OP_LESS,
		OP_GREATER,
		OP_PRINT,
		OP_POP_BACK,
		OP_DEFINE_GLOBAL_VAR,
		OP_GET_GLOBAL_VAR,
		OP_SET_GLOBAL_VAR,
		OP_GET_LOCAL_VAR,
		OP_SET_LOCAL_VAR,
		OP_JUMP,
		OP_JUMP_IF_FALSE
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

			case OPCode::OP_EQUAL:
				return "OP_EQUAL";

			case OPCode::OP_LESS:
				return "OP_LESS";	
			
			case OPCode::OP_GREATER:
				return "OP_GREATER";

			case OPCode::OP_PRINT:
				return "OP_PRINT";

			case OPCode::OP_POP_BACK:
				return "OP_POP_BACK"; 

			case OPCode::OP_DEFINE_GLOBAL_VAR:
				return "OP_DEF_GLOBAL";

			case OPCode::OP_GET_GLOBAL_VAR:
				return "OP_GET_GLOBAL";				

			case OPCode::OP_SET_GLOBAL_VAR:
				return "OP_SET_GLOBAL";

			case OPCode::OP_GET_LOCAL_VAR:
				return "OP_GET_LOCAL";

			case OPCode::OP_SET_LOCAL_VAR:
				return "OP_SET_LOCAL";

			case OPCode::OP_JUMP:
				return "OP_JUMP";

			case OPCode::OP_JUMP_IF_FALSE:
				return "OP_JUMP_IF_FALSE";
		}
		
		return "";
	}
}