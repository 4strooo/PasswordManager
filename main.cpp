#include <iostream>
#include <string> // Til datatypen 'string'.
#include <locale> // Til ekstra UTF-8 support for �, �, �
#include "console.h" // Til bedre output muligheder i konsol (warning, colors osv)
#include "encrypter.h"

// Forward declaration (s� passwordManager.cpp ved at funktionen parseCommand findes et sted)
void parseCommand(const std::string& input);

// For at ung� at skrive 'std::' foran alt s�som std::cout.
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
        cout << "Opret password p� 8 karakterer: ";
        cin >> pass;

        cout << "Bekr�ft password: ";
        cin >> confirmPass;

        if (pass == confirmPass && pass.length() == 8)
        {
            encrypter->GenerateKey(pass.c_str());
            cout << "Password bekr�ftet" << endl;
            system("pause");
            system("cls");
            break;
        } 
        cout << "Invalid password. Pr�v igen." << endl;
    }
}

void showUserAuthenticationPage() {
    cout << "F�lgende database vil blive �bnet: " << dbName << endl;
    do {
        cout << "Skriv password: ";
        cin >> confirmPass;
        if (pass != confirmPass)
            cout << "Forkert password. Pr�v igen." << endl;
    } while (pass != confirmPass);  // Loop indtil passwords matcher

    cout << "Password er korrekt. Dekrypterer database eller s�dan noget" << endl;
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
    std::locale::global(std::locale("")); //Ekstra UTF-8 support for �, �, �

    if (!encrypter->CheckKey("key.txt")) // Tjek om database-fil ikke eksisterer
    {
        showDatabaseSetupPage();
    }
    showUserAuthenticationPage();
    showDatabaseMenu();
    return 0;
}


