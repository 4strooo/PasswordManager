#include "database.h"

Database::Database()
{
	db = nullptr;
	stmt = nullptr;
}

bool Database::CheckDatabaseFile(const char* path)
{
	if (sqlite3_open(path, &db) == SQLITE_OK)
	{
		const char* checkStatement = "SELECT EXISTS (SELECT name FROM sqlite_schema WHERE type='table' AND name='logins')";
		SQL_Prepare(checkStatement);
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			if (sqlite3_column_int(stmt, 0) == 1)
			{
				sqlite3_finalize(stmt);
				sqlite3_close(db);
				return true;
			}
		}
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return false;
}

void Database::CreateDatabase()
{
	sqlite3_open("data.db", &db);
	const char* query = "CREATE TABLE IF NOT EXISTS logins "
		"(id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"login_name BLOB NOT NULL, "
		"email BLOB NOT NULL, "
		"username BLOB NOT NULL, "
		"password BLOB NOT NULL)";
	SQL_Prepare(query);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

int Database::SQL_Prepare(const char* statement)
{
	return sqlite3_prepare_v2(db, statement, -1, &stmt, nullptr);
}