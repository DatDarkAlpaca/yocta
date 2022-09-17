#include "VirtualMachine.h"

void yo::VirtualMachine::execute(InstructionSet& set)
{
	m_IP = set.instructions.data();

#ifdef DEBUG_INTERPRETER_STACK_TRACE
	printf("\n-=-= Compiler : Disassembler =-=-\n");
#endif

	while (true)
	{
#ifdef DEBUG_INTERPRETER_STACK_TRACE
		printf("Stack: %s", m_VMStack.empty() ? "[]" : "");

		for (const YoctaValue& value : m_VMStack)
			printf("[%s]", translateYoctaValue(value).c_str());

		printf(" | ");

		printf("Globals: %s", m_Globals.empty() ? "[]" : "");

		auto it = m_Globals.begin();
		while (it != m_Globals.end())
		{
			printf("[%s=%s]", it->first.c_str(), translateYoctaValue(it->second).c_str());
			it++;
		}

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
				{
					throw "Constant already defined";
					return;
				}

				m_Globals[value.getString()] = m_VMStack.back();
				m_VMStack.pop_back();
				break;
			}
			
			case OPCode::OP_GET_GLOBAL:
			{
				YoctaValue value = readYoctaValue(set);

				if (m_Globals.find(value.getString()) == m_Globals.end())
				{
					throw "Undefined constant";
					return;
				}

				m_VMStack.push_back(m_Globals[value.getString()]);
				break;
			}

			case OPCode::OP_SET_GLOBAL:
			{
				YoctaValue value = readYoctaValue(set);

				if (m_Globals.find(value.getString()) == m_Globals.end())
				{
					throw "Undefined constant";
					return;
				}

				m_Globals[value.getString()] = m_VMStack.back();
				break;
			}


			case OPCode::OP_ADD:
			case OPCode::OP_SUB:
			case OPCode::OP_MULT:
			case OPCode::OP_DIV:
			case OPCode::OP_MOD:
			{
				handleBinaryOperation((OPCode)instruction);
				break;
			}

			case OP_INCREMENT:
			{
				YoctaValue value = m_VMStack.back();
				m_VMStack.pop_back();
				if (!value.isNumber())
					return throw "Invalid operator argument";

				YoctaValue result = { value.getNumber() + 1};
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
	}
}
