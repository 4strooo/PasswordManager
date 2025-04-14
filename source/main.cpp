#include <iostream>
#include "console.h"
#include "encrypter.h"

int main()
{
	Console* console = new Console();
	Encrypter* encrypter = new Encrypter();
	if (encrypter->CheckKeyFile("key.txt"))
	{
		console->Print("Enter password to log in");
		std::string pass;
		pass = console->GetLine();
		console->Print("");
		console->Print("Authenticating...");
		if (encrypter->Login("key.txt", pass.c_str()))
		{
			console->Print("Login successful");
		}
	}
	else
	{
		console->Print("Looks like it's your first time using this program");
		console->Print("You have to create a master password for the password manager, make sure you remember it!");
		console->Print("");
		console->Print("Input password (must be 8 characters)");
		std::string pass;
		while (true)
		{
			pass = console->GetLine();
			if (pass.length() == 8)
			{
				break;
			}
			console->Print("Invalid password, try again");
		}
		console->Print("");
		console->Print("Password set");
		console->Print("Generating encryption keys...");
		encrypter->GenerateKeys(pass.c_str());
		console->Print("Saving encryption keys...");
		encrypter->GenerateKeyFile();
		unsigned char encrypted[8];
		unsigned char plain[9];
		encrypter->EncryptChunk("$0hxLL0%", encrypted);
		encrypter->DecryptChunk(encrypted, plain);
		plain[8] = '\0';
		std::cout << plain << std::endl;
		console->GetLine();
	}
}