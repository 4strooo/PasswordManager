#include "database.h"
#include "encrypter.h"
#include <iostream>
Database::Database(Console* cmd)
{
	console = cmd;
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
	console->Print("Setting up database...");
	if (sqlite3_open("data.db", &db) == SQLITE_OK)
	{
		const char* query = "CREATE TABLE IF NOT EXISTS logins "
			"(id INTEGER PRIMARY KEY AUTOINCREMENT, "
			"login_name BLOB NOT NULL, "
			"email BLOB NOT NULL, "
			"username BLOB NOT NULL, "
			"password BLOB NOT NULL)";
		SQL_Prepare(query);
		if (sqlite3_step(stmt) != SQLITE_DONE)
		{
			console->PrintError("SQL ERROR - Could not create table");
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return;
	}
	console->PrintError("COULD NOT CREATE DATABASE FILE");
}

int Database::SQL_Prepare(const char* statement)
{
	return sqlite3_prepare_v2(db, statement, -1, &stmt, nullptr);
}

void Database::Open()
{
	if (sqlite3_open_v2("data.db", &db, SQLITE_OPEN_READWRITE, nullptr) != SQLITE_OK)
	{
		console->PrintError("COULD NOT OPEN DATABASE");
	}
}

void Database::InsertLogin(std::string login[4])
{
	console->Print("Saving login...");
	const char* query = "INSERT INTO logins (login_name, email, username, password) VALUES (?1, ?2, ?3, ?4)";
	SQL_Prepare(query);
	for (int i = 0; i < 4; i++)
	{
		int len = login[i].length();
		unsigned char* buffer = new unsigned char[len];
		for (int ii = 0; ii < len; ii++)
		{
			buffer[ii] = login[i][ii];
		}
		if (sqlite3_bind_blob(stmt, i + 1, buffer, len, SQLITE_TRANSIENT) != SQLITE_OK)
		{
			console->PrintError("SERIALIZATION FAILED - Could not convert data to binary object");
		}
		delete[] buffer;
	}
	if (sqlite3_step(stmt) != SQLITE_DONE)
	{
		console->PrintError("SQL ERROR - Could not save login");
	}
	else
	{
		console->PrintComplex("", "LOGIN SAVED");
	}
	sqlite3_finalize(stmt);
}

void Database::UpdateLogin(std::string login[4], int id)
{
	console->Print("Saving login...");
	const char* query = "UPDATE logins SET (login_name, email, username, password) = (?1, ?2, ?3, ?4) WHERE id = ?5";
	SQL_Prepare(query);
	for (int i = 0; i < 4; i++)
	{
		int len = login[i].length();
		unsigned char* buffer = new unsigned char[len];
		for (int ii = 0; ii < len; ii++)
		{
			buffer[ii] = login[i][ii];
		}
		if (sqlite3_bind_blob(stmt, i + 1, buffer, len, SQLITE_TRANSIENT) != SQLITE_OK)
		{
			console->PrintError("SERIALIZATION FAILED - Could not convert data to binary object");
		}
		delete[] buffer;
	}
	sqlite3_bind_int(stmt, 5, id);
	if (sqlite3_step(stmt) != SQLITE_DONE)
	{
		console->PrintError("SQL ERROR - Could not save login");
	}
	else
	{
		console->PrintComplex("", "LOGIN SAVED");
	}
	sqlite3_finalize(stmt);
}

int Database::GetLoginCount()
{
	const char* query = "SELECT COUNT() FROM logins";
	SQL_Prepare(query);
	int count = -1;
	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		count = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return count;
}

int Database::PrepPrintAll()
{
	console->Print("Fetching logins...");
	int rows = GetLoginCount();
	const char* query = "SELECT id, login_name, email, username, password FROM logins";
	SQL_Prepare(query);
	return rows;
}

void Database::PrepFind(int id)
{
	const char* query = "SELECT id, login_name, email, username, password FROM logins WHERE id = ?1";
	SQL_Prepare(query);
	sqlite3_bind_int(stmt, 1, id);
}

void Database::GetRaw(uString login[4])
{
	if (sqlite3_step(stmt) != SQLITE_ROW)
	{
		console->PrintError("SQL ERROR - Could not fetch login");
		return;
	}
	for (int i = 0; i < 4; i++)
	{	
		const void* blobPtr = sqlite3_column_blob(stmt, i + 1);
		int len = sqlite3_column_bytes(stmt, i + 1);
		unsigned char* uChar = static_cast<unsigned char*>(const_cast<void*>(blobPtr));
		login[i] = uString(len, 0);
		for (int j = 0; j < len; j++)
		{
			login[i][j] = uChar[j];
		}
	}
}

void Database::Cleanup()
{
	sqlite3_finalize(stmt);
}

void Database::Close()
{
	sqlite3_close(db);
}