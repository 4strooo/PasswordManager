#pragma once
#include "console.h"


class Encrypter
{
public:
	Encrypter(Console* cmd);
	bool CheckKeyFile(const char* path);
	bool Login(const char* path, const char* pass);
	void GenerateKeys(const char* pass);
	void GenerateKeyFile();
	void EncryptChunk(unsigned char chunk[8], unsigned char output[8]);
	void DecryptChunk(unsigned char chunk[8], unsigned char output[8]);
	void Feistel(unsigned char left[4], unsigned char right[4], int round);
	std::string EncryptString(std::string str);
	uString DecryptString(uString str);

private:
	Console* console;
	unsigned char encryptionKeys[16][6] = { 0 };
	//$0hxLL0%
	unsigned char baseStr[8] = { '$', '0', 'h', 'x', 'L', 'L', '0', '%' };
};