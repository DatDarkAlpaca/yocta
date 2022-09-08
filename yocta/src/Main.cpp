#include "InstructionArray.h"
#include "Disassembler.h"

int main()
{
	using namespace yo;

	InstructionArray instructionArray(8);
	instructionArray.push_back((uint8_t)OPCode::OP_RETURN);

	Disassembler disassembler;

	disassembler(instructionArray, "Instruction Array: Before");

	instructionArray.clear();
	printf("\n");

	disassembler(instructionArray, "Instruction Array: After");

	return 0;
}