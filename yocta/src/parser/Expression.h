#pragma once
#include "YoctaValue.h"

namespace yo
{
	enum class ExpressionType
	{
		EXPR_NONE,
		EXPR_CONSTANT,
		EXPR_OPERATION,
		EXPR_POP
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