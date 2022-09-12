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
			// TODO: fix it
			return vmStack.at(- 1 - distance);
		}

		uint8_t readByte();

		Value readConstant(const Chunk& chunk);

	private:
		void binaryOperation(OPCode operation);

	private:
		template <class X>
		using is_not_string = typename std::enable_if<!std::is_same<X, std::string>::value>::type;

		template <class T, class = is_not_string<T>>
		constexpr T forward_or_transform(T t) 
		{
			return std::forward<T>(t);
		}

		template<typename... Values>
		void runtimeError(const char* format, Values... value)
		{
			// TODO: calculate instruction number.
			size_t instruction = 1;
			printf("<Line %d> ", compiler.currentChunk->lines[instruction]);
			printf(format, forward_or_transform(value)...);
		}

	private:
		inline bool isBooleanFalse(const Value& value) const
		{
			return value.type == ValueType::VT_NONE || (value.type == ValueType::VT_BOOL && !std::get<bool>(value.variantValue));
		}

	private:
		const uint8_t* IP = nullptr;
		std::vector<Value> vmStack;
		std::unordered_map<std::string, Value> vmGlobals;

	private:
		Compiler compiler;
	};
}