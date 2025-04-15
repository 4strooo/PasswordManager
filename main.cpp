#include <iostream>
#include "console.h" // Til bedre output muligheder i konsol (warning, colors osv)
#include "encrypter.h"
#include "database.h"
#include <windows.h> // For searching .db in windows


// Forward declaration (så passwordManager.cpp ved at funktionen parseCommand findes et sted)
void parseCommand(const std::string& input);

//forward declaration (så koden kører denne void først)
void outputLoginsAfChatGPT();

// For at ungå at skrive 'std::' foran alt såsom std::cout.
using namespace std;

char dbName[100] = "Database.db";
string pass, confirmPass;
Console* console = new Console();
Encrypter* encrypter = new Encrypter();
sqlite3* db = nullptr;  // Define db pointer only here. It conflicts with the one in commando_handling.cpp. 



void showDatabaseSetupPage() 
{
    system("cls"); // Clears screen
    cout << "===== PASSWORD MANAGER CONSOLE =====\n";
    cout << "\n";
    cout << "Ingen database er registreret.\n";
    cout << "Skriv et database navn og et master-password for at oprette en ny:\n";

    cout << "\nDatabase navn: ";
    cin >> dbName;
    strcat_s(dbName, sizeof(dbName), ".db"); //adds string ".db" to the end of dbName

    while (true)
    {
        cout << "Opret password på 8 karakterer: ";
        cin >> pass;

        cout << "Bekræft password: ";
        cin >> confirmPass;

        if (pass == confirmPass && pass.length() == 8)
        {
            //encrypter->GenerateKey(pass.c_str());
            cout << "Password bekræftet" << endl;
            system("pause");
            system("cls");
            break;
        } 
        cout << "Invalid password. Prøv igen." << endl;
    }
}

void setupDatabase()
{
    sqlite3_open(dbName, &db);  // Open or create a database

    //Create database template
    const char* sql = "CREATE TABLE logins ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "login_name TEXT, "
        "password TEXT, "
        "username TEXT, "
        "email TEXT);";

    sqlite3_exec(db, sql, 0, 0, nullptr); //execute CREATE TABLE
    sqlite3_close(db);             // Close the database
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

void openDatabase() { //Lavet med ChatGPT indtil videre
    string input;

    //Search for any .db file in project folder
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA("*.db", &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        strcpy_s(dbName, findFileData.cFileName);
        FindClose(hFind);
    }

    //Open database
    if (sqlite3_open(dbName, &db) == SQLITE_OK)
    {
        cout << "Database with the name " << dbName << " opened successfully! \n" << endl;
        cout << "Dine gemte logins" << endl;
        cout << "----------------- \n" << endl;
        outputLoginsAfChatGPT();
        cout << "Skriv 'help' for kommandoliste.\n" << endl;

        while (true) {
            cout << "> ";
            getline(cin, input);

            parseCommand(input); // Kald funktionen fra 'commando_handling.cpp'
        }
    }
    else
    {
        std::cout << "Error opening database!" << std::endl;
    }
}

void outputLoginsAfChatGPT() {

    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, login_name, password, username, email FROM logins;";

    // Check if database is accessible
    if (!db) {
        cerr << "Error: Database pointer is null after opening." << endl;
        return;
    }

    // Prepare SQL statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Error: Failed to prepare SQL statement. " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    bool hasData = false;

    // Execute query and print results
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hasData = true;
        cout << "ID: " << sqlite3_column_int(stmt, 0) << endl;
        cout << "Login Name: " << (sqlite3_column_text(stmt, 1) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) : "NULL") << endl;
        cout << "Password: " << (sqlite3_column_text(stmt, 2) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) : "NULL") << endl;
        cout << "Username: " << (sqlite3_column_text(stmt, 3) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) : "NULL") << endl;
        cout << "Email: " << (sqlite3_column_text(stmt, 4) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) : "NULL") << endl;
        cout << "-----------------------------------" << endl;
    }

    if (!hasData) {
        cout << "No stored logins found in the database." << endl;
    }

    // Cleanup
    sqlite3_finalize(stmt);
}

int main() 
{
    Console* console = new Console();
    Encrypter* encrypter = new Encrypter();
    Database* database = new Database();
    console->Clear();
    if (!encrypter->CheckKeyFile("key.txt") || !database->CheckDatabaseFile("data.db"))
    {
        console->Print("No account found");
        console->Print("You have to create a master password for the password manager. Make sure you remember it!");
        console->Print("");
        console->Print("Input password (must be 8 characters):");
        std::string pass = console->GetLine(8);
        console->Clear();
        console->Print("Password set");
        console->Print("Generating encryption keys...");
        encrypter->GenerateKeys(pass.c_str());
        console->Print("Saving encryption keys...");
        encrypter->GenerateKeyFile();
        console->Print("Setting up database...");
        database->CreateDatabase();
        console->Clear();
    }
    console->Print("Enter password to log in:");
    while (true) 
    {
        std::string pass = console->GetLine(8);
        console->Print("");
        console->Print("Authenticating...");
        if (encrypter->Login("key.txt", pass.c_str()))
        {
            console->Print("Login successful");
            break;
        }
        console->Print("Authentication failed. Try again:");
    }

    return 0;
}



