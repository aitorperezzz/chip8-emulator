#pragma once

class Utils
{
public:
	// Prints any value in hexadecimal human readable form. It prints it
	// to stdout without a newline.
	template <typename T>
	static void printHexNumber(T number);
};

template <typename T>
void Utils::printHexNumber(T number)
{
	// Decide the width of the number.
	size_t width = sizeof(T) * 2 + 2;
	std::cout << std::showbase << std::internal << std::setfill('0');
	std::cout << std::hex << std::setw(width) << number;
}