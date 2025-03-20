#include <iostream>
#include "console.h"

Console::Console()
{
	input = nullptr;
}

void Console::Print(const char* text)
{
	std::cout << text << std::endl;
}

std::string Console::GetLine()
{
	std::string line;
	std::getline(std::cin >> std::ws, line);
	return line;
}