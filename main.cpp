#include <iostream>
#include "console.h"
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
void FillLogin(Systems* sys, std::string& title, std::string& email, std::string& name, std::string& pass);
bool Return(Systems* sys, int max);
void HomeScreen(Systems* sys);

void GeneratePassword(Systems* sys);
void AddLogin(Systems* sys);
void PrintLogins(Systems* sys);
void FindLogin(Systems* sys);
void Exit(Systems* sys);

void GenSimple(Systems* sys, int characters, int amount);
void GenNormal(Systems* sys, int characters, int amount);
void GenAdvanced(Systems* sys, int characters, int amount);

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
    sys->console->Print("Enter a master password for the password manager. Make sure you remember it!");
    sys->console->Break();
    sys->console->PrintComplex("Enter password (must be ? characters long):", "8", GREEN);
    std::string pass = sys->console->GetLine(8);
    sys->console->Clear();
    sys->console->PrintCenter("PASSWORD SET", GREEN);
    sys->encrypter->GenerateKeys(pass.c_str());
    sys->encrypter->GenerateKeyFile();
    sys->database->CreateDatabase();
    sys->console->PrintCenter("SETUP COMPLETE", GREEN);
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

void FillLogin(Systems* sys, std::string& title, std::string& email, std::string& name, std::string& pass)
{
    sys->console->PrintComplex("Enter a ????? for the login:", "title", GREEN);
    title = sys->console->GetLine();
    sys->console->PrintComplex("Enter an ????? ???????:", "emailaddress", GREEN);
    email = sys->console->GetEmail();
    sys->console->PrintComplex("Enter a ????????:", "username", GREEN);
    name = sys->console->GetLine();
    sys->console->PrintComplex("Enter a ????????:", "password", GREEN);
    pass = sys->console->GetLine();
}

bool Return(Systems* sys, int max)
{
    sys->console->PrintComplex("Enter ? to return:", "0", RED);
    if (max < 1)
    {
        sys->console->GetNum(0, 0);
        return true;
    }
    if (sys->console->GetNum(0, max) == 0)
    {
        return true;
    }
    return false;
}

void HomeScreen(Systems* sys)
{
    while (!sys->done) 
    {
        sys->console->Clear();
        sys->console->Print("Enter one of the following numbers:");
        sys->console->PrintCenter("1 - GENERATE PASSWORD", GREEN);
        sys->console->Print("");
        sys->console->PrintCenter("2 - ADD NEW LOGIN", GREEN);
        sys->console->Print("");
        sys->console->PrintCenter("3 - PRINT ALL SAVED LOGINS", GREEN);
        sys->console->Print("");
        sys->console->PrintCenter("4 - EDIT LOGIN", GREEN);
        sys->console->Print("");
        sys->console->PrintCenter("0 - EXIT", RED);
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
    void (*genFunc[3])(Systems*, int, int) = { GenSimple, GenNormal, GenAdvanced };
    while (true) 
    {
        sys->console->Print("Enter how many characters your password should have:");
        int charCount = sys->console->GetNum(6, 64);
        sys->console->Print("Enter how many passwords you would like to generate:");
        int amount = sys->console->GetNum(1, 128);
        sys->console->Print("Select password generation method:");
        sys->console->Print("");
        sys->console->PrintComplex("1 - ??????????", "Simplified", RED);
        sys->console->Print("Lowercase letters and numbers");
        sys->console->Print("Easy to read and write");
        sys->console->PrintComplex("Strength: ????", "WEAK", RED);
        sys->console->Print("");
        sys->console->PrintComplex("2 - ??????", "Normal", YELLOW);
        sys->console->Print("ASCII mixed-case letters, numbers and symbols");
        sys->console->PrintComplex("Strength: ??????", "STRONG", YELLOW);
        sys->console->Print("");
        sys->console->PrintComplex("3 - ????????", "Advanced", GREEN);
        sys->console->Print("Extended ASCII mixed-case letters, numbers and symbols");
        sys->console->Print("Extra protection against brute-force attacks");
        sys->console->PrintComplex("Strength: ???????????", "UNBREAKABLE", GREEN);
        sys->console->Print("");
        int num = sys->console->GetNum(1, 3);
        sys->console->Print("Generating...");
        sys->console->Break();
        genFunc[num - 1](sys, charCount, amount);
        sys->console->Break();
        sys->console->PrintComplex("Enter ? to generate more passwords:", "1", GREEN);
        if (Return(sys, 1))
        {
            break;
        }
        sys->console->Clear();
    }
}

void AddLogin(Systems* sys)
{
    while (true) 
    {
        std::string loginInfo[4];
        FillLogin(sys, loginInfo[0], loginInfo[1], loginInfo[2], loginInfo[3]);
        sys->console->Break();
        sys->console->Print("Encrypting login...");
        for (int i = 0; i < 4; i++)
        {
            loginInfo[i] = sys->encrypter->EncryptString(loginInfo[i]);
        }
        sys->console->PrintCenter("ENCRYPTION SUCCESSFUL", GREEN);
        sys->database->InsertLogin(loginInfo);
        sys->console->Break();
        sys->console->PrintComplex("Enter ? to add another login:", "1", GREEN);
        if (Return(sys, 1))
        {
            break;
        }
    }
}

void PrintLogins(Systems* sys)
{
    int num = sys->database->PrepPrintAll();
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
    sys->console->PrintCenter("==== SAVED LOGINS ====", GREEN);
    sys->console->StartTable();
    for (int i = 0; i < num; i++)
    {
        sys->console->AddRow(logins[i], i + 1);
        delete[] logins[i];
    }
    delete[] logins;
    sys->console->CloseTable();
    Return(sys, 0);
}

void FindLogin(Systems* sys)
{
    int max = sys->database->GetLoginCount();
    if (max < 1)
    {
        Return(sys, 0);
        return;
    }
    sys->console->PrintComplex("Enter the ?? for the login you want to edit:", "ID", GREEN);
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
        sys->console->PrintComplex("Enter ? to edit this login:", "1", GREEN);
        if (Return(sys, 1))
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
        sys->console->PrintCenter("ENCRYPTION SUCCESSFUL", GREEN);
        sys->database->UpdateLogin(newInfo, id);
    }
}

void GenSimple(Systems* sys, int characters, int amount)
{
    std::string numbers = "23456789";
    std::string letters[3] = { "bcdfghjkmnpqrstvwxyz", "zyxwvtsrqpnmkjhgfdcb", "aeiou" };
    std::srand(std::time(0));
    for (int i = 0; i < amount; i++)
    {
        std::string result;
        for (int j = 0; j < characters; j++)
        {
            int diceroll = std::rand() % 3;
            int index = std::rand() % letters[diceroll].length();
            result.push_back(letters[diceroll][index]);
        }
        for (int j = 0; j < characters / 4; j++)
        {
            int place = (std::rand() % (characters - 2)) + 1;
            int index = std::rand() % numbers.length();
            result[place] = numbers[index];
        }
        sys->console->Print(result.c_str());
        sys->console->Print("");
    }
}

void GenNormal(Systems* sys, int characters, int amount)
{
    std::string charset;
    charset = R"(0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)";
    std::srand(std::time(0));
    for (int i = 0; i < amount; i++)
    {
        std::string result;
        while (result.length() < characters)
        {
            int index = std::rand() % charset.length();
            result.push_back(charset[index]);
        }
        sys->console->Print(result.c_str());
        sys->console->Print("");
    }
}

void GenAdvanced(Systems* sys, int characters, int amount)
{
    std::string charset;
    charset = R"(0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)";
    charset += "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖØÙÚÛÜİŞßàáâãäåæçèéêëìíîïğñòóôõöøùúûüışÿ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿×÷";
    std::srand(std::time(0));
    for (int i = 0; i < amount; i++)
    {
        std::string result;
        int lastIndex = -2;
        for (int j = 0; j < characters; j++)
        {
            int index;
            for (int random;;)
            {
                random = rand();
                index = random % charset.length();
                if (random - index <= -(charset.length()))
                {
                    if (index != lastIndex && index - 1 != lastIndex && index + 1 != lastIndex) 
                    {
                        lastIndex = index;
                        break;
                    }
                }
            }
            result.push_back(charset[index]);
        }

        sys->console->Print(result.c_str());
        sys->console->Print("");
    }
}


