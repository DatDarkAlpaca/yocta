#pragma once
#include "InstructionSet.h"
#include "Expression.h"

namespace yo
{
	class Compiler
	{
	public:
		explicit Compiler(const std::vector<Expression>& expressions);

	public:
		InstructionSet compile();

	private:
		void handleExpression(const Expression& expression);

	private:
		void handleConstants(YoctaValue value);

		void handleOperations(YoctaValue value);

	private:
		InstructionSet m_Instructions;
		std::vector<Expression> m_Expressions;
	};
}