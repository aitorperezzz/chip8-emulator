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
// code itself, and should not be used while loading a program.
#define START_AVAILABLE_MEMORY 0x200

// Number of general purpose registers.
#define NUM_REGISTERS 16

// Max number of adresses in the stack.
#define SIZE_STACK 16


class Chip8
{
public:
	Chip8();
	virtual ~Chip8();

	// Initializes the memory and registers in the CPU.
	// This is executed only once.
	ErrorCode initialize();

	// Loads the selected program into memory.
	ErrorCode loadProgram(const std::string &filename);

	// Emulates a cycle in the CPU.
	ErrorCode executeCycle();

private:
	// The RAM memory.
	unsigned char memory[NUM_BYTES_MEMORY];

	// Chip 8 has 16 general purpose 16 bit registers, named Vx.
	unsigned short v[NUM_REGISTERS];

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