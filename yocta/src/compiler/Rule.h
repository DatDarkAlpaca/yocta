#pragma once
#include <functional>
#include "Precedence.h"

namespace yo
{
	struct Rule
	{
	public:
		Rule(std::function<void()> prefix, std::function<void()> infix, Precedence precedence)
			: prefix(prefix), infix(infix), precedence(precedence) { }

		Rule() = default;

	public:
		std::function<void()> prefix;
		std::function<void()> infix;
		Precedence precedence;
	};
}