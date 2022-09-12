#include "VirtualMachine.h"

yo::VirtualMachine::InterpretResult yo::VirtualMachine::run()
{
	#ifdef DEBUG_TRACE
	printf("-=-= Disassembly : Interpreter =-=-\n");
	#endif

	auto chunk = compiler.currentChunk;
	IP = chunk->data.data();

	while (true)
	{
		#ifdef DEBUG_TRACE
		#ifdef DEBUG_VM_STACK_TRACE
		printf("Stack: %s", vmStack.empty() ? "[]" : "");
		
		for (const Value& value : vmStack)
		{
			printf("[");
			displayValue(value);
			printf("]");
		}
		printf("\n");
		#endif
		Disassembler::disassembleInstruction(*compiler.currentChunk, (int)(IP - chunk->data.data()));
		#endif

		uint8_t instruction = 0;

		switch (instruction = readByte())
		{
			case (uint8_t)OPCode::OP_RETURN: 
				return InterpretResult::OK;

			case (uint8_t)OPCode::OP_CONSTANT: 
			{
				Value constant = readConstant(*chunk);
				vmStack.push_back(constant);
				break;
			}

			case (uint8_t)OPCode::OP_NEGATE: 
			{
				Value back = vmStack.back();
				vmStack.pop_back();
				vmStack.push_back(-back);
				break;
			}

			case (uint8_t)OPCode::OP_ADD: 
			{
				binaryOperation(OPCode::OP_ADD);
				break;
			}

			case (uint8_t)OPCode::OP_SUB: 
			{
				binaryOperation(OPCode::OP_SUB);
				break;
			}

			case (uint8_t)OPCode::OP_MULT: 
			{
				binaryOperation(OPCode::OP_MULT);
				break;
			}

			case (uint8_t)OPCode::OP_DIV: 
			{
				binaryOperation(OPCode::OP_DIV);
				break;
			}

			case (uint8_t)OPCode::OP_NOT:
			{
				Value back = vmStack.back();
				vmStack.pop_back();

				vmStack.push_back({ ValueType::VT_BOOL, isBooleanFalse(back) });
				break;
			}

			case (uint8_t)OPCode::OP_NONE:
			{
				vmStack.push_back({ValueType::VT_NONE, 0.});
				break;
			}

			case (uint8_t)OPCode::OP_TRUE:
			{
				vmStack.push_back({ ValueType::VT_BOOL, true });
				break;
			}

			case (uint8_t)OPCode::OP_FALSE:
			{
				vmStack.push_back({ ValueType::VT_BOOL, false });
				break;
			}

			case (uint8_t)OPCode::OP_EQUAL:
			{
				Value b = vmStack.back();
				vmStack.pop_back();

				Value a = vmStack.back();
				vmStack.pop_back();

				vmStack.push_back({ ValueType::VT_BOOL, a == b });
				break;
			}

			case (uint8_t)OPCode::OP_GREATER:
			{
				binaryOperation(OPCode::OP_GREATER);
				break;
			}
			case (uint8_t)OPCode::OP_LESS:
			{
				binaryOperation(OPCode::OP_LESS);
				break;
			}

			case (uint8_t)OPCode::OP_PRINT:
			{
				Value back = vmStack.back();
				vmStack.pop_back();

				displayValue(back);
				printf("\n");
				break;
			}

			case (uint8_t)OPCode::OP_POP_BACK:
				vmStack.pop_back();
				break;

			case (uint8_t)OPCode::OP_DEFINE_GLOBAL_VAR:
			{
				StringObject* name = getStringObject(chunk->constantPool[readByte()]);
				vmGlobals[name->data] = vmStack.back();
				vmStack.pop_back();
				break;
			}

			case (uint8_t)OPCode::OP_GET_GLOBAL_VAR:
			{
				StringObject* name = getStringObject(chunk->constantPool[readByte()]);
				Value value;

				if (vmGlobals.find(name->data) == vmGlobals.end())
				{
					// HANDLE RUNTIME EXCEPTIONS
					return InterpretResult::RUNTIME_ERROR;
				}

				vmStack.push_back(vmGlobals[name->data]);
				break;
			}

			case (uint8_t)OPCode::OP_SET_GLOBAL_VAR:
			{
				StringObject* name = getStringObject(chunk->constantPool[readByte()]);

				if (vmGlobals.find(name->data) == vmGlobals.end())
				{
					runtimeError("Undefined variable '%s'.\n", name->data.c_str());
					return InterpretResult::RUNTIME_ERROR;
				}

				vmGlobals[name->data] = vmStack.back();
				break;
			}
		}
	}
}

yo::VirtualMachine::InterpretResult yo::VirtualMachine::interpret(const char* source)
{
	Chunk chunk;

	if (!compiler.compile(source, &chunk))
	{
		chunk.clear();
		return InterpretResult::COMPILE_ERROR;
	}

	InterpretResult result = run();

	chunk.clear();
	return result;
}

uint8_t yo::VirtualMachine::readByte()
{
	return *IP++;
}

yo::Value yo::VirtualMachine::readConstant(const Chunk& chunk)
{
	return chunk.constantPool[readByte()];
}

void yo::VirtualMachine::binaryOperation(OPCode operation)
{
	Value b = vmStack.back();
	vmStack.pop_back();

	Value a = vmStack.back();
	vmStack.pop_back();

	switch (operation)
	{
	case OPCode::OP_ADD:
		vmStack.push_back(a + b);
		break;

	case OPCode::OP_SUB:
		vmStack.push_back(a - b);
		break;

	case OPCode::OP_MULT:
		vmStack.push_back(a * b);
		break;

	case OPCode::OP_DIV:
		vmStack.push_back(a / b);
		break;

	case OPCode::OP_GREATER:
		vmStack.push_back({ ValueType::VT_BOOL, a > b });
		break;

	case OPCode::OP_LESS:
		vmStack.push_back({ ValueType::VT_BOOL, a < b });
		break;
	}
}
