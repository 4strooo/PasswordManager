#pragma once
#include "sqlite3.h"

class Database
{
public:
	Database();
	bool CheckDatabaseFile(const char* path);
	void CreateDatabase();
	int SQL_Prepare(const char* statement);

private:
	sqlite3* db;
	sqlite3_stmt* stmt;
};