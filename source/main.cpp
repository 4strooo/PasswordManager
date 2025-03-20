#include <iostream>
#include "console.h"
#include "encrypter.h"

int main()
{
	Console* console = new Console();
	Encrypter* encrypter = new Encrypter();
	if (encrypter->CheckKey("key.txt"))
	{
		console->Print("File exists");
	}
	else
	{
		console->Print("Looks like it's your first time using this program");
		console->Print("You have to create a master password for the password manager, make sure you remember it!");
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
		console->Print("Password set");
		encrypter->GenerateKey(pass.c_str());
		console->GetLine();
	}
}