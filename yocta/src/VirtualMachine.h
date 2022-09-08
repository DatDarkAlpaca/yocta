#pragma once
#include "Chunk.h"
#include "Common.h"
#include "Disassembler.h"

#include "Compiler.h"

namespace yo
{
	class VirtualMachine
	{
	public:
		enum class InterpretResult { OK = 0, COMPILE_ERROR, RUNTIME_ERROR };

	public:
		InterpretResult run()
		{
#ifdef DEBUG_TRACE
			printf("-=-= Disassembly : Interpreter =-=-\n");
#endif

			auto chunk = compiler.currentChunk;
			IP = chunk->data.data();

			while (true)
			{
#ifdef DEBUG_TRACE
				printf("Stack: %s", vmStack.empty() ? "[]" : "");

				for (const yocta_value& value : vmStack)
				{
					printf("[");
					printf("%g", value);
					printf("]");
				}
				printf("\n");

				Disassembler::disassembleInstruction(*compiler.currentChunk, (int)(IP - chunk->data.data()));
#endif

				uint8_t instruction = 0;

				switch (instruction = readByte())
				{
					case (uint8_t)OPCode::OP_RETURN: {
						yocta_value back = vmStack.back();
						vmStack.pop_back();

#ifdef DEBUG_TRACE
						printf("\n");
#endif

						printf("%g\n", back);
						return InterpretResult::OK;
					}

					case (uint8_t)OPCode::OP_CONSTANT: {
						yocta_value constant = readConstant(*chunk);
						vmStack.push_back(constant);
						//printf("%g", constant);
						//printf("\n");
						break;
					}

					case (uint8_t)OPCode::OP_NEGATE: {
						yocta_value back = vmStack.back();
						vmStack.pop_back();
						vmStack.push_back(-back);
						break;
					}

					case (uint8_t)OPCode::OP_ADD: {
						binaryOperation(OPCode::OP_ADD);
						break;
					}
					
					case (uint8_t)OPCode::OP_SUB: {
						binaryOperation(OPCode::OP_SUB);
						break;
					}
					
					case (uint8_t)OPCode::OP_MULT: {
						binaryOperation(OPCode::OP_MULT);
						break;
					}
					
					case (uint8_t)OPCode::OP_DIV: {
						binaryOperation(OPCode::OP_DIV);
						break;
					}
				}
			}
		}

		InterpretResult interpret(const char* source)
		{
			Chunk chunk;

			if (!compiler.compile(source, &chunk))
			{
				chunk.clear();
				return VirtualMachine::InterpretResult::COMPILE_ERROR;
			}

			InterpretResult result = run();

			chunk.clear();
			return result;
		}

	private:
		uint8_t readByte()
		{
			return *IP++;
		}

		yocta_value readConstant(const Chunk& chunk)
		{
			return chunk.constantPool[readByte()];
		}

	private:
		void binaryOperation(OPCode operation)
		{
			yocta_value b = vmStack.back();
			vmStack.pop_back();

			yocta_value a = vmStack.back();
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
			}
		}

	private:
		const uint8_t* IP = nullptr;
		std::vector<yocta_value> vmStack;

	private:
		Compiler compiler;
	};
}