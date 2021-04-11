#pragma once

#include <string>

// Define some error codes that the interpreter can return to main
// and can use internally.
enum ErrorCode
{
    Ok,
    Error,
    FileOpenError,
    NotEnoughMemory
};

// Number of bytes in RAM memory
#define NUM_BYTES_MEMORY 4096

// The first 512 bytes of memory were originally occupied by the interpreter
// code itself, and a program should be loaded after those bytes.
#define START_AVAILABLE_MEMORY 0x200

// Number of general purpose registers.
#define NUM_REGISTERS 16

// Max number of adresses in the stack.
#define SIZE_STACK 16

// Definition of the interpreter's class
class Chip8
{

public:
    Chip8() = default;
    ~Chip8() = default;

    // Initializes the memory and registers in the CPU.
    ErrorCode initialize();

    // Loads the selected program into memory.
    ErrorCode loadProgram(const std::string &filename);

    // Emulates a cycle in the CPU.
    ErrorCode executeCycle();

    // Executes the provided instruction.
    ErrorCode executeInstruction(const unsigned short &instruction);

    // Writes an instruction into memory, starting at memoryIndex with the most significant byte
    ErrorCode setInstructionInMemory(unsigned short memoryIndex, unsigned short instruction);

    // Sets a value to the register selected
    ErrorCode setRegister(unsigned char registerIndex, unsigned char registerValue);

    // Returns the value stored at a register
    inline unsigned char getRegister(unsigned char registerIndex) const
    {
        return v[registerIndex];
    }

    // Sets the program counter
    void setPc(unsigned short inPc)
    {
        pc = inPc;
    }

    // Returns the current value of the program counter
    inline unsigned short getPc() const
    {
        return pc;
    }

private:
    // The RAM memory.
    unsigned char memory[NUM_BYTES_MEMORY];

    // Chip 8 has 16 general purpose 8 bit registers, named Vx.
    unsigned char v[NUM_REGISTERS];

    // I register, usually used to store memory addresses, so normally only
    // the lowest 12 bits of it are going to be used.
    unsigned short i;

    // Delay timer register.
    unsigned char dtr;

    // Sound timer register.
    unsigned char str;

    // The Program Counter or PC is used to store the address that
    // is currently being executed.
    unsigned short pc;

    // The stack is an array of values used to store addresses that the interpreter
    // should return to after finishing the execution of a subroutine. Only 16 levels
    // in the stack are allowed, so only 16 subroutines can be nested at once.
    unsigned short stack[SIZE_STACK];

    // The Stack Pointer or SP always points to the top of the stack.
    unsigned char sp;
};