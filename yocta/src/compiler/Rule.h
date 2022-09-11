#pragma once
#include <functional>
#include "Precedence.h"

namespace yo
{
	struct Rule
	{
	public:
		Rule(std::function<void(bool)> prefix, std::function<void(bool)> infix, Precedence precedence)
			: prefix(prefix), infix(infix), precedence(precedence) { }

		Rule() = default;

	public:
		std::function<void(bool)> prefix;
		std::function<void(bool)> infix;
		Precedence precedence;
	};
}