#pragma once
#include <string>
#include <windows.h>
#define DARKGREEN 2
#define GREY 7
#define GREEN 10
#define RED 12
#define YELLOW 14
#define WHITE 15
typedef std::basic_string<unsigned char> uString;
class Console 
{
public:
	Console();
	void Print(const char* text);
	void PrintError(const char* text);
	void PrintComplex(const char* text, const char* params, int col);
	void PrintCenter(const char* text, int col);
	std::string GetLine();
	std::string GetLine(int length);
	std::string GetEmail();
	int GetNum(int min, int max);
	void Break();
	void Clear();
	void StartTable();
	void AddRow(uString login[4], int index);
	void CloseTable();

private:
	HANDLE handle;
	int consoleSize;
	int lineIndices[6] = { 0 };
	std::string Center(const char* text);
	void SetColor(int col);
	void TABLE_Break(int len, char c);
	std::string TABLE_PadCell(std::string text, int len);
	std::string TABLE_CreateRow(std::string text[5], int len);
};