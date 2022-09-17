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
			return handleConstants(expression.value);

		case ExpressionType::EXPR_OPERATION:
			return handleOperations(expression.value);

		case ExpressionType::EXPR_POP:
			return m_Instructions.push_instruction(OPCode::OP_POP);

		case ExpressionType::EXPR_DEF_GLOBAL_VAR:
			m_Instructions.push_instruction(OPCode::OP_DEFINE_GLOBAL);
			m_Instructions.push_value(expression.value);
			break;

		case ExpressionType::EXPR_GET_GLOBAL_VAR:
			m_Instructions.push_instruction(OPCode::OP_GET_GLOBAL);
			m_Instructions.push_value(expression.value);
			break;

		case ExpressionType::EXPR_SET_GLOBAL_VAR:
			m_Instructions.push_instruction(OPCode::OP_SET_GLOBAL);
			m_Instructions.push_value(expression.value);
			break;
	}
}

void yo::Compiler::handleConstants(YoctaValue value)
{
	if (value.isNumber())
		m_Instructions.push_number_literal(value.getNumber());

	else if(value.isBool())
		m_Instructions.push_bool_literal(value.getBool());

	else if (value.isNull())
		m_Instructions.push_null_literal(value.getNull());
}

void yo::Compiler::handleOperations(YoctaValue value)
{
	if (!value.isReservedToken())
		throw "Undefined operation";

	auto token = value.getReservedToken();
	switch (token)
	{
		case ReservedToken::T_NONE:
			return m_Instructions.push_instruction(OPCode::None);

		// Arithmetic Operators:
		case ReservedToken::T_ADD:
			return m_Instructions.push_instruction(OPCode::OP_ADD);

		case ReservedToken::T_SUB:
			return m_Instructions.push_instruction(OPCode::OP_SUB);

		case ReservedToken::T_MULT:
			return m_Instructions.push_instruction(OPCode::OP_MULT);

		case ReservedToken::T_DIV:
			return m_Instructions.push_instruction(OPCode::OP_DIV);

		case ReservedToken::T_MOD:
			return m_Instructions.push_instruction(OPCode::OP_MOD);

		// Increment & Decrement:
		case ReservedToken::T_INCREMENT:
			return m_Instructions.push_instruction(OPCode::OP_INCREMENT);

		case ReservedToken::T_DECREMENT:
			return m_Instructions.push_instruction(OPCode::OP_DECREMENT);

		// Bitwise Operators:
		case ReservedToken::T_BITNOT:
			return m_Instructions.push_instruction(OPCode::OP_BITNOT);

		case ReservedToken::T_BITAND:
			return m_Instructions.push_instruction(OPCode::OP_BITAND);

		case ReservedToken::T_BITOR:
			return m_Instructions.push_instruction(OPCode::OP_BITOR);

		case ReservedToken::T_BITXOR:
			return m_Instructions.push_instruction(OPCode::OP_BITXOR);

		case ReservedToken::T_BITSHIFT_LEFT:
			return m_Instructions.push_instruction(OPCode::OP_BITSHIFT_LEFT);

		case ReservedToken::T_BITSHIFT_RIGHT:
			return m_Instructions.push_instruction(OPCode::OP_BITSHIFT_RIGHT);
			
		// Logical Operators:
		case ReservedToken::T_NOT:
			return m_Instructions.push_instruction(OPCode::OP_NOT);
	}
}
