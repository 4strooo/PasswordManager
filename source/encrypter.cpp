#include <fstream>
#include <iostream>
#include "encrypter.h"
#include "DES.h"

Encrypter::Encrypter(Console* cmd)
{
	console = cmd;
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
		console->PrintError("AUTHENTICATION FAILED - Key file corrupted or missing");
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
			console->PrintError("AUTHENTICATION FAILED - Wrong password");
			return false;
		}
	}
	console->PrintComplex("", "LOGIN SUCCESSFUL");
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
	console->Print("Generating encryption keys...");
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

void Encrypter::EncryptChunk(unsigned char chunk[8], unsigned char output[8])
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

std::string Encrypter::EncryptString(std::string str)
{
	std::string result;
	unsigned char chunk[8] = { 0 };
	int chunkIndex = 0;
	for (int i = 0; i < str.length(); i++)
	{
		chunk[chunkIndex] = str[i];
		chunkIndex++;
		if ((i % 8) == 7 || i == (str.length() - 1))
		{
			unsigned char encrypted[8];
			EncryptChunk(chunk, encrypted);
			for (int ii = 0; ii < 8; ii++)
			{
				result.push_back(encrypted[ii]);
				chunk[ii] = '\0';
			}
			chunkIndex = 0;
		}
	}
	return result;
}

uString Encrypter::DecryptString(uString str)
{
	uString result;
	int len = str.length();
	if ((len % 8) != 0)
	{
		console->PrintError("DECRYPTION ERROR - Text improperly encrypted or corrupted");
		return result;
	}
	for (int i = 0; i < len / 8; i++)
	{
		unsigned char chunk[8] = { 0 };
		for (int ii = 0; ii < 8; ii++)
		{
			chunk[ii] = str[ii + (i * 8)];
		}
		unsigned char decrypted[8];
		DecryptChunk(chunk, decrypted);
		for (int ii = 0; ii < 8; ii++)
		{
			result.push_back(decrypted[ii]);
		}
	}
	return result;
}