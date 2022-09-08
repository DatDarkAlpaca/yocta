#include "InstructionArray.h"

yo::InstructionArray::InstructionArray(unsigned int capacity)
	: capacity(capacity)
{
	data = new uint8_t[capacity];
}

void yo::InstructionArray::push_back(uint8_t instruction)
{
	if (capacity < count + 1)
	{
		unsigned int oldCapacity = capacity;
		capacity = (capacity < 16) ? 16 : int(capacity * 1.5);

		reallocateArray(oldCapacity, capacity);
	}

	data[count] = instruction;
	++count;
}

void yo::InstructionArray::clear()
{
	reallocateArray(capacity, 0);
	count = 0;
	capacity = 0;
	data = nullptr;
}

void yo::InstructionArray::reallocateArray(unsigned int oldSize, unsigned int size)
{
	uint8_t* reallocatedArray = new uint8_t[size];
	std::copy(data, data + std::min(oldSize, size), reallocatedArray);

	delete data;
	data = reallocatedArray;
}
