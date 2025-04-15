#include <fstream>
#include <iostream>
#include "encrypter.h"
#include "DES.h"

Encrypter::Encrypter()
{
	encryptionKey = nullptr;
	baseStr = "$0hxLL0%";
}

bool Encrypter::CheckKeyFile(const char* path)
{
	std::fstream file;
	file.open(path, std::ios::in | std::ios::binary);
	if (!file.good())
	{
		return false;
	}
	file.seekg(0, std::ios::end);
	if (file.tellg() == 8)
	{
		file.close();
		return true;
	}
	else
	{
		file.close();
		return false;
	}
}

bool Encrypter::Login(const char* path, const char* pass)
{
	if (!CheckKeyFile(path))
	{
		return false;
	}
	std::fstream file;
	file.open(path, std::ios::in | std::ios::binary);
	unsigned char buffer[8];
	file.read((char*)buffer, 8);
	file.close();
	GenerateKeys(pass);
	unsigned char plain[8];
	DecryptChunk(buffer, plain);
	for (int i = 0; i < 8; i++)
	{
		if (plain[i] != baseStr[i])
		{
			return false;
		}
	}
	return true;
}

void Encrypter::GenerateKeys(const char* pass)
{
	unsigned char password[8];
	for (int i = 0; i < 8; i++)
	{
		password[i] = pass[i];
	}
	unsigned char key[7];
	DES::DES_64to56bit(password, key);
	char32_t firstHalf;
	char32_t secondHalf;
	DES::DES_SplitKey(key, &firstHalf, &secondHalf);
	for (int j = 0; j < 16; j++)
	{
		unsigned char roundKey[6];
		DES::DES_ShiftKeys(&firstHalf, &secondHalf, j);
		DES::DES_GenerateRoundKey(firstHalf, secondHalf, roundKey);
		for (int jj = 0; jj < 6; jj++)
		{
			encryptionKeys[j][jj] = roundKey[jj];
		}
	}
}

void Encrypter::GenerateKeyFile()
{
	std::fstream file;
	file.open("key.txt", std::ios::out | std::ios::binary | std::ios::trunc);
	unsigned char encryptedBase[8];
	EncryptChunk(baseStr, encryptedBase);
	for (int i = 0; i < 8; i++)
	{
		file << encryptedBase[i];
	}
	file.close();
}

void Encrypter::EncryptChunk(const char* chunk, unsigned char output[8])
{
	unsigned char block[8];
	for (int i = 0; i < 8; i++)
	{
		block[i] = chunk[i];
	}
	unsigned char left[4];
	unsigned char right[4];
	DES::DES_InitialPermutation(block, left, right);
	for (int j = 0; j < 16; j++)
	{
		Feistel(left, right, j);
	}
	DES::DES_FinalPermutation(right, left, output);
}

void Encrypter::DecryptChunk(unsigned char chunk[8], unsigned char output[8])
{
	unsigned char left[4];
	unsigned char right[4];
	DES::DES_InitialPermutation(chunk, left, right);
	for (int j = 0; j < 16; j++)
	{
		Feistel(left, right, 15 - j);
	}
	DES::DES_FinalPermutation(right, left, output);
}

void Encrypter::Feistel(unsigned char left[4], unsigned char right[4], int round)
{
	unsigned char expanded[6];
	DES::DES_ExpansionPermutation(right, expanded);
	DES::DES_MixWithKey(encryptionKeys[round], expanded);
	unsigned char sBoxed[4];
	DES::DES_SBoxPermutation(expanded, sBoxed);
	unsigned char pBoxed[4];
	DES::DES_PBoxPermutation(sBoxed, pBoxed);
	DES::DES_XORHalves(left, pBoxed);
	for (int jj = 0; jj < 4; jj++)
	{
		left[jj] = right[jj];
		right[jj] = pBoxed[jj];
	}
}