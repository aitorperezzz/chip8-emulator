#include <iostream>

#include "chip8.hpp"

int main(void)
{
	// Create a new instance of the chip 8 interpreter and initialize it.
	Chip8 chip8;
	if (chip8.initialize() != Ok)
	{
		std::cout << "ERROR: could not initialize the Chip 8 interpreter" << std::endl;
		return -1;
	}

	// TODO: initialize the graphics.

	// Load the program to the interpreter memory.
	std::string filename = "15PUZZLE";
	if (chip8.loadProgram(filename) != Ok)
	{
		std::cout << "ERROR: the program could not be loaded to memory" << std::endl;
		return -1;
	}

	// Loop forever.
	std::cout << "Begin execution..." << std::endl;
	while (true)
	{
		// Execute the next cycle in the chip 8 interpreter.
		if (chip8.executeCycle() != Ok)
		{
			std::cout << "ERROR: cycle execution went wrong" << std::endl;
			return -1;
		}

		// TODO:update the display if necessary.
	}

	return 0;
}