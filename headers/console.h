#pragma once
#include <string>

class Console 
{
public:
	Console();
	void Print(const char* text);
	std::string GetLine();
	std::string GetLine(int length);
	void Clear();

private:
	char** input;
};