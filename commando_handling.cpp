#include <iostream>
#include <string>

void parseCommand(const std::string& input) {
    std::string command = input.substr(0, input.find(' '));

    if (command == "add") {
        std::cout << "[Add kommando modtaget]\n";
    }
    else if (command == "del") {
        std::cout << "[Delete kommando modtaget]\n";
    }
    else if (command == "copy") {
        std::cout << "[Copy kommando modtaget]\n";
    }
    else if (command == "Locate") {
        std::cout << "[Locate kommando modtaget]\n";
    }
    else if (command == "exit") {
        std::cout << "Afslutter programmet.\n";
        exit(0);
    }
    else if (command == "help") {
        std::cout << R"(Kommandoliste:

            a) Tilføj et login
               - Format: "add <login navn>, <password>, <username>, <email>"
               - Eksempel:
                 "add Minecraft konto, Password1234, Notch, notch@mojang.com"
                 "add Minecraft konto, Password1234, , notch@mojang.com"
                 "add Minecraft konto, Password1234, Notch, "
               - Info: Elementer er separeret af komma. Tomme elementpladser er tilladt, men 3 kommaer er påkrævet.

            b) Slet et login
               - Format: "del <login navn>"
               - Eksempel: "del Minecraft konto"

            c) Kopiér et login
               - Format: "copy <login nummer><u/e/p>"
                 (u: username, e: email, p: password)
                 - Eksempel: "copy 1p" (Kopierer passwordet fra login 1 til udklipsholderen)

            d) Åben sti til Databasen
               - Format: "Locate <database navn>"
               - Eksempel: "Locate Website-Logins"
               - Info: Åbner Windows-stien, hvor databasen er gemt. Brugbart til hurtig import og export af databaser.

            e) Afslut
               - Format: "exit"

         )";
    }
    else {
        std::cout << "Ukendt kommando.\n";
    }
}