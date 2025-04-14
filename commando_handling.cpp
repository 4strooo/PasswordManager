#include <iostream>
#include <string>
#include <sqlite3.h> //SQLite database library
#include <windows.h> // For searching .db in windows

extern sqlite3* db;  // Reference db from main.cpp

void parseCommand(const std::string& input) {
    std::string command = input.substr(0, input.find(' '));

    if (command == "logins") {
        std::cout << "[Logins kommando modtaget]\n";
    }

    if (command == "add") {
        std::cout << "[Add kommando modtaget]\n";
        //SQLite add login
        const char* insertDataSQL = "INSERT INTO logins (login_name, password, username, email) VALUES "
            "('Minecraft', 'password1234', 'Notch', 'notch@mojang.com');";
        sqlite3_exec(db, insertDataSQL, 0, 0, nullptr); //execute INSERT INTO

    }
    else if (command == "del") {
        std::cout << "[Delete kommando modtaget]\n";
        sqlite3_exec(db, "DELETE FROM logins WHERE id = 1;", nullptr, nullptr, nullptr);
    }
    else if (command == "copy") {
        std::cout << "[Copy kommando modtaget]\n";
        //SQL print password
        sqlite3_exec(db, "SELECT password FROM logins;", [](void*, int argc, char** argv, char**) {
            std::cout << "Password: " << argv[0] << std::endl;
            return 0;
            }, nullptr, nullptr);
        system("pause");
    }
    else if (command == "locate") {
        std::cout << "[Locate kommando modtaget]\n";
        WIN32_FIND_DATAA findFileData;
        HANDLE hFind = FindFirstFileA("*.db", &findFileData);

        if (hFind != INVALID_HANDLE_VALUE) {
            char fullPath[MAX_PATH];
            GetFullPathNameA(findFileData.cFileName, MAX_PATH, fullPath, nullptr);

            std::string folderPath = std::string(fullPath).substr(0, std::string(fullPath).find_last_of("\\/"));
            std::cout << "Database folder: " << folderPath << std::endl;

            FindClose(hFind);
        }

    }
    else if (command == "exit") {
        std::cout << "Afslutter programmet.\n";
        sqlite3_close(db);
        exit(0);
    }
    else if (command == "help") {
        std::cout << R"(Kommandoliste:
            a) Vis login-liste
                - Format: "logins"

            b) Tilføj et login
               - Format: "add <login navn>, <password>, <username>, <email>"
               - Eksempel:
                 "add Minecraft konto, Password1234, Notch, notch@mojang.com"
                 "add Minecraft konto, Password1234, , notch@mojang.com"
                 "add Minecraft konto, Password1234, Notch, "
               - Info: Elementer er separeret af komma. Tomme elementpladser er tilladt, men 3 kommaer er påkrævet.

            c) Slet et login
               - Format: "del <login navn>"
               - Eksempel: "del Minecraft konto"

            d) Kopiér et login
               - Format: "copy <login nummer><u/e/p>"
                 (u: username, e: email, p: password)
                 - Eksempel: "copy 1p" (Kopierer passwordet fra login 1 til udklipsholderen)

            e) Åben sti til Databasen
               - Format: "locate <database navn>"
               - Eksempel: "locate Website-Logins"
               - Info: Åbner Windows-stien, hvor databasen er gemt. Brugbart til hurtig import og export af databaser.

            f) Afslut
               - Format: "exit"

         )";
    }
    else {
        std::cout << "Ukendt kommando.\n";
    }
}