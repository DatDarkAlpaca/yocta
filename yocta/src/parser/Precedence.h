#pragma once

namespace yo
{
	enum class Precedence
	{
		P_NONE,
		P_ASSIGNMENT,
		P_OR,
		P_AND,
		P_EQUAL,
		P_COMPARE,
		P_TERM,
		P_FACTOR,
		P_UNARY,
		P_CALL,
		P_PRIMARY
	};
}