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
		const Value& peek(int distance) const
		{
			return vmStack.at(- 1 - distance);
		}

		uint8_t readByte();

		Value readConstant(const Chunk& chunk);

	private:
		void binaryOperation(OPCode operation);

	private:
		inline bool isBooleanFalse(const Value& value) const
		{
			return value.type == ValueType::VT_NONE || (value.type == ValueType::VT_BOOL && !std::get<bool>(value.variantValue));
		}

	private:
		const uint8_t* IP = nullptr;
		std::vector<Value> vmStack;

	private:
		Compiler compiler;
	};
}