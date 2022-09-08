#pragma once
#include "Token.h"

namespace yo
{
	struct Parser
	{
		bool errorFound = false, panicMode = false;
		Token current = {}, previous = {};
	};
}