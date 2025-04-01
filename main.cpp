#include <iostream>
#include <string> // Til datatypen 'string'.
#include <locale> // Til ekstra UTF-8 support for Æ, Ø, Å
#include "console.h" // Til bedre output muligheder i konsol (warning, colors osv)
#include "encrypter.h"
#include "sqlite3.h" // SQLite database library

// Forward declaration (så passwordManager.cpp ved at funktionen parseCommand findes et sted)
void parseCommand(const std::string& input);

// For at ungå at skrive 'std::' foran alt såsom std::cout.
using namespace std;

string dbName = "Database";
string pass, confirmPass;
Console* console = new Console();
Encrypter* encrypter = new Encrypter();


void showDatabaseSetupPage() 
{
    system("cls"); // Clears screen
    cout << "===== PASSWORD MANAGER CONSOLE =====\n";
    cout << "\n";
    cout << "Ingen database er registreret.\n";
    cout << "Skriv et database navn og et master-password for at oprette en ny:\n";

    cout << "\nDatabase navn: ";
    cin >> dbName;

    while (true)
    {
        cout << "Opret password på 8 karakterer: ";
        cin >> pass;

        cout << "Bekræft password: ";
        cin >> confirmPass;

        if (pass == confirmPass && pass.length() == 8)
        {
            encrypter->GenerateKey(pass.c_str());
            cout << "Password bekræftet" << endl;
            system("pause");
            system("cls");
            break;
        } 
        cout << "Invalid password. Prøv igen." << endl;
    }
}

void showUserAuthenticationPage() {
    cout << "Følgende database vil blive åbnet: " << dbName << endl;
    do {
        cout << "Skriv password: ";
        cin >> confirmPass;
        if (pass != confirmPass)
            cout << "Forkert password. Prøv igen." << endl;
    } while (pass != confirmPass);  // Loop indtil passwords matcher

    cout << "Password er korrekt. Dekrypterer database eller sådan noget" << endl;
    system("cls");
    return;
}

void showDatabaseMenu() {
    string input;

    cout << "PASSWORD MANAGER CONSOLE - " << dbName << endl;
    cout << "Dine gemte logins\n" << endl;
    cout << "-----------------\n" << endl;
    cout << "Database indhold her\n" << endl;
    cout << "Skriv 'help' for kommandoliste.\n" << endl;

    while (true) {
        cout << "> ";
        getline(cin, input);

        parseCommand(input); // Kald funktionen fra 'commando_handling.cpp'
    }


}


int main() {
    sqlite3* db;
    sqlite3_open("test.db", &db); //create database

    if (sqlite3_open("test.db", &db) == SQLITE_OK) {
        std::cout << "Database opened successfully!" << std::endl;


        //SQL Add 

        const char* sql = "CREATE TABLE logins ("
            "login_navn TEXT PRIMARY KEY, "
            "password TEXT, "
            "username TEXT, "
            "email TEXT);";

        const char* insertDataSQL = "INSERT INTO logins (login_navn, password, username, email) VALUES "
            "('Minecraft', 'password1234', 'Notch', 'notch@mojang.com');";

        //sqlite3_exec(db, sql, 0, 0, nullptr);
        sqlite3_exec(db, insertDataSQL, 0, 0, nullptr);


        /*
        sqlite3_exec(db, "SELECT password FROM logins;", [](void*, int argc, char** argv, char**) {
            std::cout << "Password: " << argv[0] << std::endl;
            return 0;
            }, nullptr, nullptr);
        */

        system("pause");

        //SQL delete
        //sqlite3_exec(db, "DROP TABLE IF EXISTS logins;", nullptr, nullptr, nullptr);



    }
    else {
        std::cout << "Error opening database!" << std::endl;
    }
    sqlite3_close(db);
    return 0;
}

/*
int main() {
    std::locale::global(std::locale("")); //Ekstra UTF-8 support for Æ, Ø, Å

    if (!encrypter->CheckKey("key.txt")) // Tjek om database-fil ikke eksisterer
    {
        showDatabaseSetupPage();
    }
    showUserAuthenticationPage();
    showDatabaseMenu();
    return 0;
}
*/


