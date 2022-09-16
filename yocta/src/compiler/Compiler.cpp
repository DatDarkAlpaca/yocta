#include "Compiler.h"

yo::Compiler::Compiler(const std::vector<Expression>& expressions)
	: m_Expressions(expressions)
{
}

yo::InstructionSet yo::Compiler::compile()
{
	for (const Expression& expression : m_Expressions)
		handleExpression(expression);

	return m_Instructions;
}

void yo::Compiler::handleExpression(const Expression& expression)
{
	switch (expression.type)
	{
		case ExpressionType::EXPR_NONE:
			break;

		case ExpressionType::EXPR_CONSTANT:
			handleConstants(expression.value);
			break;

		case ExpressionType::EXPR_OPERATION:
			handleOperations(expression.value);
			break;			
	}
}

void yo::Compiler::handleConstants(YoctaValue value)
{
	if (value.isNumber())
		m_Instructions.push_back(value.getNumber());
}

void yo::Compiler::handleOperations(YoctaValue value)
{
	if (value.getReservedToken() == ReservedToken::T_ADD)
		m_Instructions.push_instruction(OPCode::OP_ADD);

	else if (value.getReservedToken() == ReservedToken::T_SUB)
		m_Instructions.push_instruction(OPCode::OP_SUB);

	else if (value.getReservedToken() == ReservedToken::T_MULT)
		m_Instructions.push_instruction(OPCode::OP_MULT);

	else if (value.getReservedToken() == ReservedToken::T_DIV)
		m_Instructions.push_instruction(OPCode::OP_DIV);

	else if (value.getReservedToken() == ReservedToken::T_MOD)
		m_Instructions.push_instruction(OPCode::OP_MOD);
}
