#pragma once

class Encrypter
{
public:
	Encrypter();
	bool CheckKeyFile(const char* path);
	bool Login(const char* path, const char* pass);
	void GenerateKeys(const char* pass);
	void GenerateKeyFile();
	void EncryptChunk(const char* chunk, unsigned char output[8]);
	void DecryptChunk(unsigned char chunk[8], unsigned char output[8]);
	void Feistel(unsigned char left[4], unsigned char right[4], int round);

private:
	char* encryptionKey;
	unsigned char encryptionKeys[16][6] = { 0 };
	const char* baseStr;
};