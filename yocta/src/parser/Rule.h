#pragma once
#include <functional>
#include "Precedence.h"

namespace yo
{
	using rule_type = std::function<void()>;

	struct Rule
	{
	public:
		Rule() = default;

		Rule(rule_type prefix, rule_type infix, Precedence precedence)
			: prefix(prefix), infix(infix), precedence(precedence) { }

	public:
		rule_type prefix = {}, infix = {};
		Precedence precedence = Precedence::P_NONE;
	};
}