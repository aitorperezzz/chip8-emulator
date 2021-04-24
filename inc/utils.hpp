#pragma once

#include <string>

// This class provides general logging and debugging utilities
class Utils
{
public:
    // Prints any value in hexadecimal human readable form
    template <typename T>
    static void printHexNumber(const std::string &description, T number);
};

template <typename T>
void Utils::printHexNumber(const std::string &description, T number)
{
    // Keep a copy of the current state of flags
    std::ios_base::fmtflags f(std::cout.flags());

    // Print the description
    std::cout << description + ": ";

    // Decide the width of the number and print as hex
    size_t width = sizeof(T) * 2;
    std::cout << "0x" << std::setfill('0') << std::setw(width) << std::hex
              << static_cast<int>(number) << std::endl;

    // Reset to the previous state of cout flags.
    std::cout.flags(f);
}