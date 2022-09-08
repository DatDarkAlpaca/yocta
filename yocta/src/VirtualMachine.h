#pragma once
#include "Chunk.h"
#include "Common.h"
#include "Disassembler.h"

namespace yo
{
	class VirtualMachine
	{
	public:
		enum class InterpretResult { OK = 0, COMPILE_ERROR, RUNTIME_ERROR };

	public:
		InterpretResult run(const Chunk& chunk)
		{
			IP = chunk.data.data();

			while (true)
			{
#ifdef DEBUG_TRACE
				printf("        ");
				for (const yocta_value& value : vmStack)
				{
					printf("[");
					printf("%g", value);
					printf("]");
				}
				printf("\n");

				Disassembler::disassembleInstruction(chunk, (int)(IP - chunk.data.data()));
#endif

				uint8_t instruction = 0;

				switch (instruction = readByte())
				{
					case (uint8_t)OPCode::OP_RETURN: {
						yocta_value back = vmStack.back();
						vmStack.pop_back();

						printf("%g", back);
						return InterpretResult::OK;
					}

					case (uint8_t)OPCode::OP_CONSTANT: {
						yocta_value constant = readConstant(chunk);
						vmStack.push_back(constant);
						printf("%g", constant);
						printf("\n");
						break;
					}

				}
			}
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
		const uint8_t* IP = nullptr;
		std::vector<yocta_value> vmStack;
	};
}