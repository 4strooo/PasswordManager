#pragma once
#include <string>

class Console 
{
public:
	Console();
	void Print(const char* text);
	std::string GetLine();

private:
	char** input;
};