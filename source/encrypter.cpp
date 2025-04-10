#include <fstream>
#include <iostream>
#include "encrypter.h"
#include "DES.h"

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
	unsigned char password[9];
	unsigned char passwordCopy[9];
	unsigned char key[8];
	unsigned char subkey[7];
	char32_t firstHalf;
	char32_t secondHalf;
	for (int i = 0; i < 8; i++)
	{
		password[i] = pass[i];
	}
	DES::DES_InitialPermutation(password, passwordCopy);
	password[8] = '\0';
	passwordCopy[8] = '\0';
	DES::DES_64to56bit(password, key);
	DES::DES_SplitKey(key, &firstHalf, &secondHalf);
	key[7] = '\0';
	DES::DES_ShiftKeys(&firstHalf, &secondHalf, 0);
	DES::DES_GenerateRoundKey(firstHalf, secondHalf, subkey);
	subkey[6] = '\0';
	std::cout << password << std::endl;
	std::cout << key << std::endl;
	std::cout << subkey << std::endl;

}