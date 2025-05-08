#include <iostream>
#include "console.h"
#define CP_1252 1252
#define ROW_WIDTH 100

Console::Console()
{
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	consoleSize = info.dwSize.X;
	SetConsoleCP(CP_1252);
	SetConsoleOutputCP(CP_1252);
	
}

void Console::Print(const char* text)
{
	SetColor(GREY);
	std::cout << text << std::endl;
}

void Console::PrintError(const char* text)
{
	SetColor(RED);
	std::cout << "ERROR: " << text << std::endl;
}

void Console::PrintComplex(const char* text, const char* params, int col)
{
	SetColor(GREY);
	int currentParam = 0;
	for (int i = 0; i < strlen(text); i++)
	{
		if (text[i] == '?')
		{
			SetColor(col);
			std::cout << params[currentParam];
			currentParam++;
			SetColor(GREY);
			continue;
		}
		std::cout << text[i];
	}
	std::cout << std::endl;
}

void Console::PrintCenter(const char* text, int col)
{
	SetColor(col);
	std::cout << Center(text) << std::endl;
}

std::string Console::GetLine()
{
	SetColor(WHITE);
	std::string line;
	std::getline(std::cin >> std::ws, line);
	return line;
}

std::string Console::GetLine(int length)
{
	SetColor(WHITE);
	std::string line;
	while (true)
	{
		line = GetLine();
		if (line.length() == length)
		{
			break;
		}
		PrintError("INVALID INPUT - Wrong amount of characters");
	}
	return line;
}

std::string Console::GetEmail()
{
	SetColor(WHITE);
	std::string line;
	while (true)
	{
		line = GetLine();
		int len = line.length();
		if (len < 3 || len > 255)
		{
			PrintError("INVALID INPUT - Email address must be between 3 and 255 characters");
			continue;
		}
		bool atSignFound = false;
		bool valid = true;
		for (int i = 0; i < len; i++)
		{
			if (i == 0 || i == len - 1)
			{
				if (line[i] == '@' || line[i] == '.' || line[i] == ' ')
				{
					PrintError("INVALID INPUT - Not a valid email address");
					valid = false;
				}
			}
			if (line[i] == '@')
			{
				atSignFound = true;
			}
		}
		if (!valid)
		{
			continue;
		}
		if (!atSignFound)
		{
			PrintError("INVALID INPUT - Email address must contain '@'");
			continue;
		}
		return line;
	}

}

int Console::GetNum(int min, int max)
{
	SetColor(WHITE);
	std::string line;
	int result;
	while (true)
	{
		line = GetLine();
		try 
		{
			result = std::stoi(line);
		}
		catch (...)
		{
			PrintError("INVALID INPUT - Not a number");
			continue;
		}
		if (result < min || result > max)
		{
			PrintError("INVALID INPUT - Number out of allowed range");
			continue;
		}
		break;
	}
	return result;
}

std::string Console::Center(const char* text)
{
	int pos = (consoleSize - strlen(text))/2;
	std::string result(pos, ' ');
	result += text;
	return result;
}

void Console::Break()
{
	std::string line(consoleSize, '-');
	PrintCenter(line.c_str(), DARKGREEN);
}

void Console::Clear()
{
	system("cls");
	TABLE_Break(26, '=');
	PrintCenter("| PASSWORD MANAGER CONSOLE |", DARKGREEN);
	TABLE_Break(26, '=');
	Print("");
	Print("");
}

void Console::SetColor(int col)
{
	SetConsoleTextAttribute(handle, col);
}

void Console::StartTable()
{
	if (consoleSize < (ROW_WIDTH + 2))
	{
		PrintError("CONSOLE ERROR - Window too small to draw table");
	}
	TABLE_Break(ROW_WIDTH, '=');
	std::string names[5] = { "ID", "TITLE", "EMAIL ADDRESS", "USERNAME", "PASSWORD" };
	std::string nextRow = TABLE_CreateRow(names, ROW_WIDTH);
	std::string print = "|" + nextRow + "|";
	print = Center(print.c_str());
	int counter = 0;
	for (int i = 0; i < print.length(); i++)
	{
		if (print[i] == '|')
		{
			lineIndices[counter] = i;
			counter++;
		}
	}
	SetColor(DARKGREEN);
	std::cout << print << std::endl;
}

void Console::AddRow(uString login[4], int index)
{
	TABLE_Break(ROW_WIDTH, '-');
	int cellSizes[5] = { 5, 19, 34, 19, 19 };
	std::string values[5];
	values[0] = std::to_string(index);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < login[i].length(); j++)
		{
			if (login[i][j] != '\0') 
			{
				values[i + 1].push_back(login[i][j]);
			}
		}
	}
	int emptied = 0;
	while (emptied < 5)
	{
		std::string nextRow[5];
		for (int i = 0; i < 5; i++)
		{
			if (values[i].length() > 0)
			{
				nextRow[i] = values[i].substr(0, cellSizes[i] - 2);
				values[i].erase(0, cellSizes[i] - 2);
				if (values[i].length() == 0)
				{
					emptied++;
				}
			}
		}
		std::string row = TABLE_CreateRow(nextRow, ROW_WIDTH);
		std::string print = "|" + row + "|";
		print = Center(print.c_str());
		for (int i = 0; i < print.length(); i++)
		{
			SetColor(GREY);
			bool isGreen = false;
			for (int j = 0; j < 6; j++)
			{
				if (lineIndices[j] == i)
				{
					isGreen = true;
					break;
				}
			}
			if (isGreen)
			{
				SetColor(DARKGREEN);
			}
			std::cout << print[i];
		}
		Print("");
	}
}

void Console::CloseTable()
{
	TABLE_Break(ROW_WIDTH, '=');
	Print("");
	PrintCenter("TABLE PRINTED", GREEN);
	Break();
}

void Console::TABLE_Break(int len, char c)
{
	std::string edge(len, c);
	std::string line = '+' + edge + '+';
	PrintCenter(line.c_str(), DARKGREEN);
}

std::string Console::TABLE_PadCell(std::string text, int len)
{
	
	int pos = (len - text.length()) / 2;
	std::string result(len, ' ');
	for (int i = 0; i < text.length(); i++)
	{
		result[pos + i] = text[i];
	}
	return result;
}

std::string Console::TABLE_CreateRow(std::string text[5], int len)
{
	int cellSizes[5] = { 5, 19, 34, 19, 19 };
	std::string result;
	for (int i = 0; i < 5; i++)
	{
		result += TABLE_PadCell(text[i], cellSizes[i]);
		result += '|';
	}
	result.pop_back();
	return result;
}