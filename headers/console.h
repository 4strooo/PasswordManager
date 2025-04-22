#pragma once
#include <string>
#include <windows.h>

typedef std::basic_string<unsigned char> uString;
class Console 
{
public:
	Console();
	void Print(const char* text);
	void PrintError(const char* text);
	void PrintComplex(const char* text, const char* params);
	std::string GetLine();
	std::string GetLine(int length);
	std::string GetEmail();
	int GetNum(int min, int max);
	std::string Center(const char* text);
	void Break();
	void Clear();
	void SetColor(int col);
	void StartTable();
	void AddRow(uString login[4], int index);
	void CloseTable();

private:
	HANDLE handle;
	int consoleCenter;
	int lineIndices[6] = { 0 };
	void TABLE_Break(int len, char c);
	std::string TABLE_PadCell(std::string text, int len);
	std::string TABLE_CreateRow(std::string text[5], int len);
};