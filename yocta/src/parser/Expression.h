#pragma once
#include "YoctaValue.h"

namespace yo
{
	enum class ExpressionType
	{
		EXPR_NONE,
		EXPR_CONSTANT,
		EXPR_OPERATION,
		EXPR_POP,
		EXPR_DEF_GLOBAL_VAR,
		EXPR_GET_GLOBAL_VAR,
		EXPR_SET_GLOBAL_VAR
	};

	constexpr const char* translateExpression(ExpressionType type)
	{
		switch (type)
		{
			case ExpressionType::EXPR_NONE:
				return "EXPR_NONE";
			case ExpressionType::EXPR_CONSTANT:
				return "EXPR_CONSTANT";
			case ExpressionType::EXPR_OPERATION:
				return "EXPR_OPERATION";
			case ExpressionType::EXPR_POP:
				return "EXPR_POP";
			case ExpressionType::EXPR_DEF_GLOBAL_VAR:
				return "EXPR_DEF_GL_VAR";
			case ExpressionType::EXPR_GET_GLOBAL_VAR:
				return "EXPR_GET_GL_VAR";
			case ExpressionType::EXPR_SET_GLOBAL_VAR:
				return "EXPR_SET_GL_VAR";
			default:
				throw std::invalid_argument("Unimplemented expression");
		}
	}

	struct Expression
	{
		ExpressionType type = ExpressionType::EXPR_NONE;
		YoctaValue value = { };
	};
}