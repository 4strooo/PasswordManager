#pragma once
#include "sqlite3.h"
#include "console.h"

class Database
{
public:
	Database(Console* cmd);
	bool CheckDatabaseFile(const char* path);
	void CreateDatabase();
	int SQL_Prepare(const char* statement);
	void Open();
	void InsertLogin(std::string login[4]);
	void UpdateLogin(std::string login[4], int id);
	int GetLoginCount();
	int PrepPrintAll();
	void PrepFind(int id);
	void GetRaw(uString login[4]);
	void Cleanup();
	void Close();

private:
	Console* console;
	sqlite3* db;
	sqlite3_stmt* stmt;
};