#pragma once
#include "Token.h"

namespace yo
{
	struct LocalVar
	{
	public:
		Token name;
		int depth;

	public:
		friend bool operator==(const LocalVar& lhs, const LocalVar& rhs);
	};
	
	inline bool operator==(const LocalVar& lhs, const LocalVar& rhs)
	{
		if (lhs.depth != rhs.depth)
			return false;

		return (lhs.name == rhs.name);
	}

	struct LocalStack
	{
	public:
		std::vector<LocalVar> locals;
		unsigned int scopeDepth = 0;
	};
}