#include <iostream>

#include "chip8.hpp"

int main(void)
{
    // Create a new instance of the chip 8 interpreter and initialize it
    Chip8 chip8;
    if (chip8.initialize() != Ok)
    {
        std::cout << "Error: could not initialize the Chip 8 interpreter"
                  << std::endl;
        return -1;
    }
    else
    {
        std::cout << "Chip 8 interpreter instance has been initialized"
                  << std::endl;
    }

    // TODO: initialize the graphics

    // Load the program to the interpreter memory
    std::string filename = "./games/15PUZZLE";
    if (chip8.loadProgram(filename) != Ok)
    {
        std::cout << "Error: program " + filename +
                         " could not be loaded to memory"
                  << std::endl;
        return -1;
    }
    else
    {
        std::cout << "Program " + filename + " has been loaded to memory"
                  << std::endl;
    }

    // Loop forever
    std::cout << "Beginning execution..." << std::endl;
    while (true)
    {
        // Execute the next cycle in the chip 8 interpreter
        if (chip8.executeCycle() != Ok)
        {
            std::cout << "Error: cycle execution went wrong" << std::endl;
            std::cout << "Error: quitting the application now" << std::endl;
            return -1;
        }

        // TODO: update the display if necessary
    }

    return 0;
}