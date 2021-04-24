#include <climits>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>

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
        std::cout
            << "Error: the program file is too big to be loaded inside the "
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
    for (size_t i = START_AVAILABLE_MEMORY;
         i < START_AVAILABLE_MEMORY + fileSizeBytes; i = i + 2)
    {
        Utils::printHexNumber(
            "Position " + std::to_string(i),
            *(reinterpret_cast<unsigned short *>(&memory[i])));
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

    return Ok;
}

ErrorCode Chip8::executeInstruction(const unsigned short &instruction)
{
    // Get the indices of the x and y register indices involved from opcode
    // TODO: remove this from here, maybe move math to a sub function
    unsigned char xRegister = instruction >> 8 & 0x0f;
    unsigned char yRegister = instruction >> 4 & 0x0f;

    // Select the operation and perform it
    if (instruction == 0x00ee)
    {
        // 00EE - RET.
        // Return from a subroutine
        pc = stack[sp];
        sp--;
    }
    else if (instruction >> 12 == 0x1)
    {
        // 1nnn - JP addr
        // Jump to location nnn.
        pc = instruction & 0x0fff;
    }
    else if (instruction >> 12 == 0x2)
    {
        // 2nnn - CALL addr
        // Call subroutine at nnn.
        sp++;
        stack[sp] = pc;
        pc = instruction & 0xfff;
    }
    else if (instruction >> 12 == 0x3)
    {
        // 3xkk - SE Vx, byte
        // Skip next instruction if Vx = kk.
        unsigned char registerIndex = (instruction >> 8) & 0x0f;
        const size_t step = v[registerIndex] == (instruction & 0xff) ? 4 : 2;
        pc += step;
    }
    else if (instruction >> 12 == 0x4)
    {
        // 4xkk - SNE Vx, byte
        // Skip next instruction if Vx != kk.
        unsigned char registerIndex = (instruction >> 8) & 0x0f;
        const size_t step = v[registerIndex] != (instruction & 0xff) ? 4 : 2;
        pc += step;
    }
    else if (instruction >> 12 == 0x5)
    {
        // 5xy0 - SE Vx, Vy
        // Skip next instruction if Vx = Vy.
        unsigned char registerIndex1 = (instruction >> 8) & 0xf;
        unsigned char registerIndex2 = (instruction >> 4) & 0xf;
        const size_t step = v[registerIndex1] == v[registerIndex2] ? 4 : 2;
        pc += step;
    }
    else if (instruction >> 12 == 0x6)
    {
        // 6xkk - LD Vx, byte
        // Set Vx = kk
        v[(instruction >> 8) & 0xf] = (instruction & 0xff);
        pc += 2;
    }
    else if (instruction >> 12 == 0x7)
    {
        // 7xkk - ADD Vx, byte
        // Set Vx = Vx + kk
        const unsigned char index = (instruction >> 8) & 0xf;
        v[index] = v[index] + (instruction & 0xff);
        pc += 2;
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x0))
    {
        // 8xy0 - LD Vx, Vy
        // Set Vx = Vy.
        v[(instruction >> 8) & 0xf] = v[(instruction >> 4) & 0xf];
        pc += 2;
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x1))
    {
        // OR operation
        v[xRegister] = v[xRegister] | v[yRegister];
        pc += 2;
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x2))
    {
        // AND operation
        v[xRegister] = v[xRegister] & v[yRegister];
        pc += 2;
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x3))
    {
        // XOR operation
        v[xRegister] = v[xRegister] ^ v[yRegister];
        pc += 2;
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x4))
    {
        // ADD operation
        unsigned short sum = v[xRegister] + v[yRegister];
        v[xRegister] = sum & 0xff;
        v[0xf] = ((sum >> 8) > 0x0) ? 0x1 : 0x0;
        pc += 2;
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x5))
    {
        // SUB operation
        v[0xf] = v[xRegister] > v[yRegister] ? 0x1 : 0x0;
        v[xRegister] = v[xRegister] - v[yRegister];
        pc += 2;
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x6))
    {
        // SHR operation
        v[0xf] = (v[xRegister] & 0x1) == 0x1 ? 0x1 : 0x0;
        v[xRegister] = v[xRegister] >> 1;
        pc += 2;
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0x7))
    {
        // SUBN operation
        v[0xf] = v[yRegister] > v[xRegister] ? 0x1 : 0x0;
        v[xRegister] = v[yRegister] - v[xRegister];
        pc += 2;
    }
    else if ((instruction >> 12 == 0x8) && ((instruction & 0xf) == 0xe))
    {
        // SHL operation
        v[0xf] = (v[xRegister] >> 7) == 0x1 ? 0x1 : 0x0;
        v[xRegister] = v[xRegister] << 1;
        pc += 2;
    }
    else if ((instruction >> 12) == 0xa)
    {
        // Annn - LD I, addr
        // Set I = nnn
        i = instruction & 0xfff;
        pc += 2;
    }
    else if ((instruction >> 12) == 0xb)
    {
        // Bnnn - JP V0, addr
        // Jump to location nnn + V0
        pc = v[0x0] + (instruction & 0xfff);
    }
    else if ((instruction >> 12) == 0xc)
    {
        // Create a function that can generate randomly distributed
        // unsigned chars
        std::random_device r;
        std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
        auto rand = std::bind(std::uniform_int_distribution<>(0, UCHAR_MAX),
                              std::mt19937(seed));

        // Cxkk - RND Vx, byte
        // Set Vx = random byte AND kk.
        const unsigned char index = (instruction >> 8) & 0xf;
        v[index] = static_cast<unsigned char>(rand()) & (instruction & 0xff);
    }
    else if ((instruction >> 12) == 0xf && (instruction & 0xff) == 0x07)
    {
        // Fx07 - LD Vx, DT
        // Set Vx = delay timer value.
        v[(instruction >> 8) & 0xf] = dtr;
        pc += 2;
    }
    else if ((instruction >> 12) == 0xf && (instruction & 0xff) == 0x15)
    {
        // Fx15 - LD DT, Vx
        // Set delay timer = Vx.
        dtr = v[(instruction >> 8) & 0xf];
        pc += 2;
    }
    else if ((instruction >> 12) == 0xf && (instruction & 0xff) == 0x18)
    {
        // Fx18 - LD ST, Vx
        // Set sound timer = Vx.
        str = v[(instruction >> 8) & 0xf];
        pc += 2;
    }
    else if ((instruction >> 12) == 0xf && (instruction & 0xff) == 0x1e)
    {
        // Fx1E - ADD I, Vx
        // Set I = I + Vx.
        i = i + v[(instruction >> 8) & 0xf];
        pc += 2;
    }
    else if ((instruction >> 12) == 0xf && (instruction & 0xff) == 0x33)
    {
        // Fx33 - LD B, Vx
        // Store BCD representation of Vx in memory locations I, I+1, and I+2.
        unsigned char value = v[(instruction >> 8) & 0xf];
        memory[i] = value / 100;
        memory[i + 1] = (value / 10) % 10;
        memory[i + 2] = value % 10;
        pc += 2;
    }
    else if ((instruction >> 12) == 0xf && (instruction & 0xff) == 0x55)
    {
        // Fx55 - LD [I], Vx
        // Store registers V0 through Vx in memory starting at location I.
        const unsigned char indexMax = (instruction >> 8) & 0xf;
        for (unsigned char index = 0; index <= indexMax; index++)
        {
            memory[i + index] = v[index];
        }
        pc += 2;
    }
    else if ((instruction >> 12) == 0xf && (instruction & 0xff) == 0x65)
    {
        // Fx65 - LD Vx, [I]
        // Read registers V0 through Vx from memory starting at location I.
        const unsigned char indexMax = (instruction >> 8) & 0xf;
        for (unsigned char index = 0; index <= indexMax; index++)
        {
            v[index] = memory[i + index];
        }
        pc += 2;
    }
    else
    {
        std::cout << "Error: the instruction was not recognised" << std::endl;
        Utils::printHexNumber("Instruction not recognised: ", instruction);
        return Error;
    }

    return Ok;
}

ErrorCode Chip8::setInstructionInMemory(unsigned short memoryIndex,
                                        unsigned short instruction)
{
    memory[memoryIndex] = instruction >> 8;
    memory[memoryIndex + 1] = instruction & 0xff;
    return Ok;
}