#include <iostream>
#include <fstream>
#include <filesystem>

#include "chip8.hpp"
#include "utils.hpp"

Chip8::Chip8()
{
}

Chip8::~Chip8()
{
}

ErrorCode Chip8::initialize()
{
	// Memory.
	for (size_t i = 0; i < NUM_BYTES_MEMORY; i++)
	{
		memory[i] = 0x00;
	}

	// General purpose registers.
	for (size_t i = 0; i < NUM_REGISTERS; i++)
	{
		v[i] = 0x0000;
	}

	// The stack.
	for (size_t i = 0; i < SIZE_STACK; i++)
	{
		stack[i] = 0x0000;
	}

	// Other registers.
	i = 0x0000;
	dtr = 0x00;
	str = 0x00;
	pc = START_AVAILABLE_MEMORY;
	sp = 0x00;

	return Ok;
}

ErrorCode Chip8::loadProgram(const std::string &filename)
{
	// Check the size of the file does not exceed the interpreter memory.
	size_t fileSizeBytes = std::filesystem::file_size(filename);
	if (fileSizeBytes > NUM_BYTES_MEMORY - START_AVAILABLE_MEMORY)
	{
		std::cout << "ERROR: the program file is too big to be loaded inside the interpreter's memory" << std::endl;
		return NotEnoughMemory;
	}

	// Try to open the program file in binary mode.
	std::ifstream inputFile;
	inputFile.open(filename, std::ios::binary | std::ios::in);
	if (!inputFile.is_open())
	{
		std::cout << "ERROR opening the file " << filename << std::endl;
		return FileOpenError;
	}

	// Read the file contents into memory.
	inputFile.read((char *)&memory[START_AVAILABLE_MEMORY], fileSizeBytes);

	// Debug the memory contents.
	std::cout << "Current contents of the interpreter's memory until end of program:" << std::endl;
	for (size_t i = START_AVAILABLE_MEMORY; i < START_AVAILABLE_MEMORY + fileSizeBytes; i = i + 2)
	{
		Utils::printHexNumber(*(reinterpret_cast<unsigned short *>(&memory[i])));
		std::cout << std::endl;
	}
	std::cout << std::endl;

	return Ok;
}

ErrorCode Chip8::executeCycle()
{
	// Get the new opcode from memory.
	unsigned short opcode = memory[pc] << 8 | memory[pc + 1];
	std::cout << "Current opcode: ";
	Utils::printHexNumber(opcode);
	std::cout << std::endl;

	// Jump the program counter to the new opcode address.
	pc += 2;
	if (pc > NUM_BYTES_MEMORY)
	{
		return Error;
	}

	return Ok;
}