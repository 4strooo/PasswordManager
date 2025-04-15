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

std::string Console::GetLine(int length)
{
	std::string line;
	while (true)
	{
		line = GetLine();
		if (line.length() == length)
		{
			break;
		}
		Print("Input has wrong amount of characters. Try again:");
	}
	return line;
}

void Console::Clear()
{
	system("cls");
	Print("===== PASSWORD MANAGER CONSOLE =====");
	Print("");
}