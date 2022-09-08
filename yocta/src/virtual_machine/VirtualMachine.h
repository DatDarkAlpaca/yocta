#pragma once
#include "Disassembler.h"
#include "Compiler.h"
#include "Debug.h"

namespace yo
{
	class VirtualMachine
	{
	public:
		enum class InterpretResult { OK = 0, COMPILE_ERROR, RUNTIME_ERROR };

	public:
		InterpretResult run();

		InterpretResult interpret(const char* source);

	private:
		uint8_t readByte();

		yocta_value readConstant(const Chunk& chunk);

	private:
		void binaryOperation(OPCode operation);

	private:
		const uint8_t* IP = nullptr;
		std::vector<yocta_value> vmStack;

	private:
		Compiler compiler;
	};
}