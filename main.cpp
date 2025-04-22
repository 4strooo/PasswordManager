#include <iostream>
#include "console.h" // Til bedre output muligheder i konsol (warning, colors osv)
#include "encrypter.h"
#include "database.h"

struct Systems
{
    Console* console;
    Encrypter* encrypter;
    Database* database;
    bool done;
    Systems()
    {
        console = new Console();
        encrypter = new Encrypter(console);
        database = new Database(console);
        done = false;
    }
    ~Systems()
    {
        delete console;
        delete encrypter;
        delete database;
    }
};

void Setup(Systems* sys);
void Login(Systems* sys);
void HomeScreen(Systems* sys);
void GeneratePassword(Systems* sys);
void AddLogin(Systems* sys);
void PrintLogins(Systems* sys);
void FindLogin(Systems* sys);
void Exit(Systems* sys);

void FillLogin(Systems* sys, std::string& title, std::string& email, std::string& name, std::string& pass);

int main() 
{
    Systems* sys = new Systems();
    sys->console->Clear();
    if (!sys->encrypter->CheckKeyFile("key.txt") || !sys->database->CheckDatabaseFile("data.db"))
    {
        Setup(sys);
    }
    Login(sys);
    sys->database->Open();
    HomeScreen(sys);
    sys->database->Close();
    delete sys;
    return 0;
}

void Setup(Systems* sys)
{
    sys->console->Print("No previous data found");
    sys->console->Print("You have to create a master password for the password manager. Make sure you remember it!");
    sys->console->Break();
    sys->console->PrintComplex("Enter password (must be ? characters long):", "8");
    std::string pass = sys->console->GetLine(8);
    sys->console->Clear();
    sys->console->PrintComplex("", "PASSWORD SET");;
    sys->encrypter->GenerateKeys(pass.c_str());
    sys->encrypter->GenerateKeyFile();
    sys->database->CreateDatabase();
    sys->console->PrintComplex("", "SETUP COMPLETE");
    sys->console->Break();
}

void Login(Systems* sys)
{
    sys->console->Print("Enter password to log in:");
    while (true)
    {
        std::string pass = sys->console->GetLine(8);
        sys->console->Print("");
        sys->console->Print("Authenticating...");
        if (sys->encrypter->Login("key.txt", pass.c_str()))
        {
            break;
        }
    }
}

void HomeScreen(Systems* sys)
{
    while (!sys->done) 
    {
        sys->console->Clear();
        sys->console->Print("Enter a number:");
        sys->console->PrintComplex("", "1 - GENERATE PASSWORD");
        sys->console->Print("");
        sys->console->PrintComplex("", "2 - ADD NEW LOGIN");
        sys->console->Print("");
        sys->console->PrintComplex("", "3 - PRINT ALL SAVED LOGINS");
        sys->console->Print("");
        sys->console->PrintComplex("", "4 - EDIT LOGIN");
        sys->console->Print("");
        sys->console->PrintComplex("", "0 - EXIT");
        sys->console->Print("");
        void (*nextFunc[5])(Systems*) = { Exit, GeneratePassword, AddLogin, PrintLogins, FindLogin };
        int num = sys->console->GetNum(0, 4);
        sys->console->Clear();
        nextFunc[num](sys);
    }
}

void Exit(Systems* sys)
{
    sys->console->Print("Exiting...");
    sys->done = true;
}

void GeneratePassword(Systems* sys)
{
    sys->console->PrintComplex("Enter the ??????? amount of characters your password should have:", "minimum");
    int min = sys->console->GetNum(4, 32);
    sys->console->PrintComplex("Enter the ??????? amount of characters your password should have:", "maximum");
    int max = sys->console->GetNum(min, 32);
    sys->console->Print("Enter how many passwords you would like to generate:");
    int amount = sys->console->GetNum(1, 128);
    sys->console->Break();
    std::srand(std::time(0));
    for (int i = 0; i < amount; i++)
    {
        int charCount = (std::rand() % (max - min + 1)) + min;
        std::string result;
        for (int j = 0; j < charCount; j++)
        {
            char c = (std::rand() % ('~' - '!' + 1)) + '!';
            result.push_back(c);
        }
        sys->console->Print(result.c_str());
    }
    sys->console->Break();
    sys->console->PrintComplex("Enter ? to return:", "0");
    sys->console->GetNum(0, 0);
}

void AddLogin(Systems* sys)
{
    std::string loginInfo[4];
    FillLogin(sys, loginInfo[0], loginInfo[1], loginInfo[2], loginInfo[3]);
    sys->console->Break();
    sys->console->Print("Encrypting login...");
    for (int i = 0; i < 4; i++)
    {
        loginInfo[i] = sys->encrypter->EncryptString(loginInfo[i]);
    }
    sys->console->PrintComplex("", "ENCRYPTION SUCCESSFUL");
    sys->database->InsertLogin(loginInfo);
    sys->console->Break();
    sys->console->PrintComplex("Enter ? to return:", "0");
    sys->console->GetNum(0, 0);
}

void PrintLogins(Systems* sys)
{
    int num = sys->database->PrepPrintAll();
    if (num < 1)
    {
        return;
    }
    uString** logins = new uString*[num];
    for (int i = 0; i < num; i++)
    {
        logins[i] = new uString[4];
        sys->database->GetRaw(logins[i]);
    }
    sys->database->Cleanup();
    sys->console->Print("Decrypting logins...");
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            logins[i][j] = sys->encrypter->DecryptString(logins[i][j]);
        }
    }
    sys->console->Clear();
    sys->console->PrintComplex("", "==== SAVED LOGINS ====");
    sys->console->StartTable();
    for (int i = 0; i < num; i++)
    {
        sys->console->AddRow(logins[i], i + 1);
        delete[] logins[i];
    }
    delete[] logins;
    sys->console->CloseTable();
    sys->console->PrintComplex("Enter ? to return:", "0");
    sys->console->GetNum(0, 0);
}

void FindLogin(Systems* sys)
{
    int max = sys->database->GetLoginCount();
    if (max < 1)
    {
        return;
    }
    sys->console->PrintComplex("Enter the ?? for the login you want to edit:", "ID");
    int id = sys->console->GetNum(1, max);
    while (true) 
    {
        sys->database->PrepFind(id);
        uString login[4];
        sys->database->GetRaw(login);
        sys->database->Cleanup();
        for (int i = 0; i < 4; i++)
        {
            login[i] = sys->encrypter->DecryptString(login[i]);
        }
        sys->console->Break();
        sys->console->StartTable();
        sys->console->AddRow(login, id);
        sys->console->CloseTable();
        sys->console->PrintComplex("Enter ? to edit this login:", "1");
        sys->console->PrintComplex("Enter ? to return:", "0");
        int choice = sys->console->GetNum(0, 1);
        if (choice == 0)
        {
            break;
        }
        sys->console->Break();
        std::string newInfo[4];
        FillLogin(sys, newInfo[0], newInfo[1], newInfo[2], newInfo[3]);
        sys->console->Print("Encrypting login...");
        for (int i = 0; i < 4; i++)
        {
            newInfo[i] = sys->encrypter->EncryptString(newInfo[i]);
        }
        sys->console->PrintComplex("", "ENCRYPTION SUCCESSFUL");
        sys->database->UpdateLogin(newInfo, id);
    }
}

void FillLogin(Systems* sys, std::string& title, std::string& email, std::string& name, std::string& pass)
{
    sys->console->PrintComplex("Enter a ????? for the login:", "title");
    title = sys->console->GetLine();
    sys->console->PrintComplex("Enter an ????? ???????:", "emailaddress");
    email = sys->console->GetEmail();
    sys->console->PrintComplex("Enter a ????????:", "username");
    name = sys->console->GetLine();
    sys->console->PrintComplex("Enter a ????????:", "password");
    pass = sys->console->GetLine();
}


