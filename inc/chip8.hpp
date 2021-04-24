#pragma once

#include <array>
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
#define START_AVAILABLE_MEMORY 512

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

    // Writes an instruction into memory, starting at memoryIndex with the most
    // significant byte
    ErrorCode setInstructionInMemory(unsigned short memoryIndex,
                                     unsigned short instruction);

    // Sets a byte in memory. Beware that an instruction is two bytes long,
    // so this function should not be used to set a complete instruction
    inline void setMemory(const unsigned short index, const unsigned char value)
    {
        memory[index] = value;
    }

    // Get a byte from memory
    inline unsigned char getMemory(const unsigned short index) const
    {
        return memory[index];
    }

    // Sets a value to the register selected
    inline void setRegister(unsigned char regIndex, unsigned char regValue)
    {
        v[regIndex] = regValue;
    }

    // Returns the value stored at a register
    inline unsigned char getRegister(unsigned char regIndex) const
    {
        return v[regIndex];
    }

    // Sets the program counter
    inline void setPc(unsigned short inPc)
    {
        pc = inPc;
    }

    // Returns the current value of the program counter
    inline unsigned short getPc() const
    {
        return pc;
    }

    // Sets the value of the stack pointer
    inline void setStackPointer(const unsigned char inSp)
    {
        sp = inSp;
    }

    // Gets the value of the stack pointer
    inline unsigned char getStackPointer() const
    {
        return sp;
    }

    // Sets the complete stack
    inline void setStack(const std::array<unsigned short, SIZE_STACK> &inStack)
    {
        stack = inStack;
    }

    // Returns a reference to the complete stack
    inline const std::array<unsigned short, SIZE_STACK> &getStack() const
    {
        return stack;
    }

    // Sets the I register
    inline void setI(const unsigned short inI)
    {
        i = inI;
    }

    // Returns the current value of the I register
    inline unsigned short getI() const
    {
        return i;
    }

    // Sets the delay timer register
    inline void setDelayTimer(const unsigned char inDtr)
    {
        dtr = inDtr;
    }

    // Returns the value of the delay timer register
    inline unsigned char getDelayTimer() const
    {
        return dtr;
    }

    // Sets the sound timer register
    inline void setSoundTimer(const unsigned char inStr)
    {
        str = inStr;
    }

    // Returns the value of the sound timer register
    inline unsigned char getSoundTimer() const
    {
        return str;
    }

private:
    // The RAM memory.
    std::array<unsigned char, NUM_BYTES_MEMORY> memory;

    // Chip 8 has 16 general purpose 8 bit registers, named Vx.
    std::array<unsigned char, NUM_REGISTERS> v;

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

    // The stack is an array of values used to store addresses that the
    // interpreter should return to after finishing the execution of a
    // subroutine. Only 16 levels in the stack are allowed, so only 16
    // subroutines can be nested at once.
    std::array<unsigned short, SIZE_STACK> stack;

    // The Stack Pointer or SP always points to the top of the stack.
    unsigned char sp;
};