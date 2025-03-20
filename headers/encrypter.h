#pragma once

class Encrypter
{
public:
	Encrypter();
	bool CheckKey(const char* path);
	void GenerateKey(const char* pass);
	char* GetKey(char* pass);
	void Encrypt(const char* path, const char* pass);

private:
	char* encryptionKey;
	const char* baseStr;
};