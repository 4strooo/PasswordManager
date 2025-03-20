#include <fstream>
#include <iostream>
#include "encrypter.h"

Encrypter::Encrypter()
{
	encryptionKey = nullptr;
	baseStr = "$0hxLL0%";
}

bool Encrypter::CheckKey(const char* path)
{
	std::fstream file;
	file.open(path, std::ios::in | std::ios::binary);
	if (!file.good())
	{
		return false;
	}
	file.seekg(0, std::ios::end);
	std::cout << file.tellg() << std::endl;
	if (file.tellg() == 8)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Encrypter::GenerateKey(const char* pass)
{
	std::fstream file;
	file.open("key.txt", std::ios::out);
	file << baseStr;
	file.close();
	Encrypt("key.txt", pass);
}

char* Encrypter::GetKey(char* pass)
{
	return nullptr;
}

void Encrypter::Encrypt(const char* path, const char* pass)
{
	
}