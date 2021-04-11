#include <filesystem>
#include <fstream>
#include <iostream>

#include "chip8.hpp"
#include "utils.hpp"

ErrorCode Chip8::initialize()
{
    // Memory
    for (size_t i = 0; i < NUM_BYTES_MEMORY; i++)
    {
        memory[i] = 0x00;
    }

    // General purpose registers
    for (size_t i = 0; i < NUM_REGISTERS; i++)
    {
        v[i] = 0x00;
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
    // Check the size of the file does not exceed the interpreter memory
    size_t fileSizeBytes = std::filesystem::file_size(filename);
    if (fileSizeBytes > NUM_BYTES_MEMORY - START_AVAILABLE_MEMORY)
    {
        std::cout << "Error: the program file is too big to be loaded inside the "
                     "interpreter's memory"
                  << std::endl;
        return NotEnoughMemory;
    }

    // Try to open the program file in binary mode.
    std::ifstream inputFile;
    inputFile.open(filename, std::ios::binary | std::ios::in);
    if (!inputFile.is_open())
    {
        std::cout << "Error opening file " << filename << std::endl;
        return FileOpenError;
    }

    // Read the file contents into memory.
    inputFile.read((char *)&memory[START_AVAILABLE_MEMORY], fileSizeBytes);

    // Debug the memory contents.
    std::cout << "Current contents of the interpreter's memory:" << std::endl;
    for (size_t i = START_AVAILABLE_MEMORY; i < START_AVAILABLE_MEMORY + fileSizeBytes; i = i + 2)
    {
        Utils::printHexNumber("Position " + std::to_string(i), *(reinterpret_cast<unsigned short *>(&memory[i])));
    }

    return Ok;
}

ErrorCode Chip8::executeCycle()
{
    // Get the new opcode from memory
    unsigned short opcode = memory[pc] << 8 | memory[pc + 1];

    // Execute instruction
    if (executeInstruction(opcode) != Ok)
    {
        std::cout << "Error: instruction could not be executed" << std::endl;
        return Error;
    }

    // Jump the program counter
    pc += 2;
    if (pc > NUM_BYTES_MEMORY)
    {
        std::cout << "Error: program counter has reached the end of the memory" << std::endl;
        return Error;
    }

    return Ok;
}

ErrorCode Chip8::executeInstruction(const unsigned short &instruction)
{
    // Get the indices of the x and y register indices involved from opcode
    unsigned char xRegister = instruction >> 8 & 0x0f;
    unsigned char yRegister = instruction >> 4 & 0x0f;

    // Select the operation and perform it
    if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x1))
    {
        // OR operation
        v[xRegister] = v[xRegister] | v[yRegister];
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x2))
    {
        // AND operation
        v[xRegister] = v[xRegister] & v[yRegister];
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x3))
    {
        // XOR operation
        v[xRegister] = v[xRegister] ^ v[yRegister];
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x4))
    {
        // ADD operation
        unsigned short sum = v[xRegister] + v[yRegister];
        v[xRegister] = sum & 0xff;
        v[0xf] = ((sum >> 8) > 0x0) ? 0x1 : 0x0;
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x5))
    {
        // SUB operation
        v[0xf] = v[xRegister] > v[yRegister] ? 0x1 : 0x0;
        v[xRegister] = v[xRegister] - v[yRegister];
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x6))
    {
        // SHR operation
        v[0xf] = (v[xRegister] & 0x1) == 0x1 ? 0x1 : 0x0;
        v[xRegister] = v[xRegister] >> 1;
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x7))
    {
        // SUBN operation
        v[0xf] = v[yRegister] > v[xRegister] ? 0x1 : 0x0;
        v[xRegister] = v[yRegister] - v[xRegister];
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0xe))
    {
        // SHL operation
        v[0xf] = (v[xRegister] >> 7) == 0x1 ? 0x1 : 0x0;
        v[xRegister] = v[xRegister] << 1;
    }
    else
    {
        std::cout << "Error: the instruction was not recognised" << std::endl;
        Utils::printHexNumber("Instruction not recognised", instruction);
        return Error;
    }

    return Ok;
}

ErrorCode Chip8::setInstructionInMemory(unsigned short memoryIndex, unsigned short instruction)
{
    memory[memoryIndex] = instruction >> 8;
    memory[memoryIndex + 1] = instruction & 0xff;
    return Ok;
}

ErrorCode Chip8::setRegister(unsigned char registerIndex, unsigned char registerValue)
{
    v[registerIndex] = registerValue;
    return Ok;
}