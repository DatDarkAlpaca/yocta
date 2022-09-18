#include "VirtualMachine.h"
#include "Error.h"

void yo::VirtualMachine::execute(InstructionSet& set)
{
	m_IP = set.instructions.data();

#ifdef DEBUG_INTERPRETER_STACK_TRACE
	printf("\n-=-= Compiler : Disassembler =-=-\n");
#endif

	while (true)
	{
#ifdef DEBUG_INTERPRETER_STACK_TRACE
		printf("\tStack: %s", m_VMStack.empty() ? "[]" : "");

		for (const YoctaValue& value : m_VMStack)
			printf("[%s]", translateYoctaValue(value).c_str());

		printf("\n");

#endif

#ifdef DEBUG_INTERPRETER_INSTRUCTION_TRACE
#ifndef DEBUG_INTERPRETER_STACK_TRACE
		printf("\n-=-= Compiler : Disassembler =-=-");
#endif
		disassembleInstruction(set, (int)(m_IP - set.instructions.data()));
#endif

		uint8_t instruction = 0;
		switch (instruction = readByte())
		{
			case OPCode::None:
				return;

			case OPCode::OP_PUSH_LITERAL:
			{
				YoctaValue value = readYoctaValue(set);
				m_VMStack.push_back(value);
				break;
			}

			case OPCode::OP_POP:
			{
				m_VMStack.pop_back();
				break;
			}

			case OPCode::OP_DEFINE_GLOBAL:
			{
				YoctaValue value = readYoctaValue(set);
				if (m_Globals.find(value.getString()) != m_Globals.end())
					throw VirtualMachineError("Global already defined", 0, 0);

				m_Globals[value.getString()] = m_VMStack.back();
				m_VMStack.pop_back();
				break;
			}
			
			case OPCode::OP_GET_GLOBAL:
			{
				YoctaValue value = readYoctaValue(set);

				if (m_Globals.find(value.getString()) == m_Globals.end())
					throw VirtualMachineError("Undefined variable", 0, 0);

				m_VMStack.push_back(m_Globals[value.getString()]);
				break;
			}

			case OPCode::OP_SET_GLOBAL:
			{
				YoctaValue value = readYoctaValue(set);

				if (m_Globals.find(value.getString()) == m_Globals.end())
					throw VirtualMachineError("Undefined constant", 0, 0);

				m_Globals[value.getString()] = m_VMStack.back();
				break;
			}


			case OPCode::OP_BITNOT:
				break;

			case OPCode::OP_ADD:
			case OPCode::OP_SUB:
			case OPCode::OP_MULT:
			case OPCode::OP_DIV:
			case OPCode::OP_MOD:
			case OPCode::OP_BITAND:
			case OPCode::OP_BITOR:
			case OPCode::OP_BITXOR:
			case OPCode::OP_BITSHIFT_LEFT:
			case OPCode::OP_BITSHIFT_RIGHT:
			{
				handleBinaryOperation((OPCode)instruction);
				break;
			}

			case OP_INCREMENT:
			{
				YoctaValue value = m_VMStack.back();
				m_VMStack.pop_back();
				if (!value.isNumber())
					throw VirtualMachineError("Invalid operator argument", 0, 0);

				YoctaValue result = { value.getNumber() + 1};
				m_VMStack.push_back(result);
				break;
			}

			case OP_DECREMENT:
			{
				YoctaValue value = m_VMStack.back();
				m_VMStack.pop_back();
				if (!value.isNumber())
					throw VirtualMachineError("Invalid operator argument", 0, 0);

				YoctaValue result = { value.getNumber() - 1 };
				m_VMStack.push_back(result);
				break;
			}
		}
	}
}

void yo::VirtualMachine::handleBinaryOperation(OPCode code)
{
	YoctaValue b = m_VMStack.back();
	m_VMStack.pop_back();

	YoctaValue a = m_VMStack.back();
	m_VMStack.pop_back();

	switch (code)
	{
		case OP_ADD:
			m_VMStack.push_back(a + b);
			break;

		case OP_SUB:
			m_VMStack.push_back(a - b);
			break;

		case OP_MULT:
			m_VMStack.push_back(a * b);
			break;

		case OP_DIV:
			m_VMStack.push_back(a / b);
			break;

		case OP_MOD:
			m_VMStack.push_back(a % b);
			break;

		case OP_BITAND:
			m_VMStack.push_back(a & b);
			break;

		case OP_BITOR:
			m_VMStack.push_back(a | b);
			break;

		case OP_BITXOR:
			m_VMStack.push_back(a ^ b);
			break;

		case OP_BITSHIFT_LEFT:
			m_VMStack.push_back(a << b);
			break;

		case OP_BITSHIFT_RIGHT:
			m_VMStack.push_back(a >> b);
			break;
	}
}
