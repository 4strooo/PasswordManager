#pragma once

namespace DES 
{
	bool GetBit(unsigned char c, int pos);
	bool GetBit(char32_t c, int pos);
	unsigned char ToggleBit(unsigned char c, int pos);
	char32_t CircularLeftShift28bit(char32_t c, int amount);
	void DES_64to56bit(unsigned char data[8], unsigned char result[7]);
	void DES_SplitKey(unsigned char data[7], char32_t* firstShift, char32_t* secondShift);
	void DES_ShiftKeys(char32_t* firstHalf, char32_t* secondHalf, int roundIndex);
	void DES_GenerateRoundKey(char32_t firstKey, char32_t secondKey, unsigned char result[6]);
	void DES_InitialPermutation(unsigned char block[8], unsigned char left[4], unsigned char right[4]);
	void DES_ExpansionPermutation(unsigned char block[4], unsigned char result[6]);
	void DES_MixWithKey(unsigned char key[6], unsigned char result[6]);
	void DES_SBoxPermutation(unsigned char block[6], unsigned char result[4]);
	void DES_PBoxPermutation(unsigned char data[4], unsigned char result[4]);
	void DES_XORHalves(unsigned char left[4], unsigned char right[4]);
	void DES_FinalPermutation(unsigned char left[4], unsigned char right[4], unsigned char result[8]);
	
}