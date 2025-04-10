#include "DES.h"

const int initialPermutation[64] =
{ 
    20, 63, 41, 7, 38, 57, 37, 11, 51, 1, 33, 43, 17, 40, 15, 47,
    27, 64, 9, 59, 3, 45, 49, 42, 62, 18, 6, 52, 30, 48, 58, 5,
    61, 13, 21, 39, 55, 2, 60, 26, 32, 12, 35, 25, 19, 10, 53, 36,
    50, 56, 4, 28, 46, 14, 24, 16, 34, 29, 44, 22, 23, 8, 54, 31
};

const int keyCompressionTable[48] =
{
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

bool DES::GetBit(unsigned char c, int pos)
{
	return (c & (1 << pos)) != 0;
}

bool DES::GetBit(char32_t c, int pos)
{
	return (c & (1 << pos)) != 0;
}

unsigned char DES::ToggleBit(unsigned char c, int pos)
{
	return c ^ (1 << pos);
}

char32_t DES::CircularLeftShift28bit(char32_t c, int amount)
{
	char32_t result = c;
	for (int i = 0; i < amount; i++)
	{
		result = (result << 1);
		if ((result & (1 << 28)) != 0)
		{
			result = result ^ (1 << 28);
			result = result | 1;
		}
	}
	return result;
}

void DES::DES_64to56bit(unsigned char data[8], unsigned char result[7])
{
	int bitCounter = 0;
	for (int i = 0; i < 8; i++)
	{
		unsigned char source = data[i];
		for (int j = 0; j < 8; j++)
		{
			if (j == 7)
			{
				continue;
			}
			unsigned char target = result[bitCounter / 8];
			if (GetBit(target, bitCounter % 8) != GetBit(source, j))
			{
				result[i] = ToggleBit(result[i], bitCounter % 8);
			}
			bitCounter++;
		}
	}
}

void DES::DES_InitialPermutation(unsigned char block[8], unsigned char result[8])
{
	for (int i = 0; i < 8; i++)
	{
		result[i] = block[i];
		for (int j = 0; j < 8; j++)
		{
			int index = initialPermutation[(i * 8) + j] - 1;
			unsigned char source = block[index / 8];
			if(GetBit(result[i], j) != GetBit(source, j % 8))
			{
				result[i] = ToggleBit(result[i], j);
			}
		}
	}
}

void DES::DES_SplitKey(unsigned char data[7], char32_t* firstShift, char32_t* secondShift)
{	
	for (int i = 0; i < 3; i++)
	{
		for (int ii = 0; ii < 8; ii++)
		{
			int bit = (i * 8) + ii;
			if (GetBit(data[i], ii) != GetBit(*firstShift, bit))
			{
				*firstShift ^= (1 << bit);
			}
			if (GetBit(data[i + 4], ii) != GetBit(*secondShift, bit))
			{
				*secondShift ^= (1 << bit);
			}
		}
	}
	for (int j = 0; j < 4; j++)
	{
		if (GetBit(data[3], j) != GetBit(*firstShift, 24 + j))
		{
			*firstShift ^= (1 << (24 + j));
		}
		if (GetBit(data[3], j + 4) != GetBit(*secondShift, 24 + j))
		{
			*secondShift ^= (1 << (24 + j));
		}
	}
}

void DES::DES_ShiftKeys(char32_t* firstHalf, char32_t* secondHalf, int roundIndex)
{
	int shiftAmount = 2;
	if (roundIndex < 2 || roundIndex == 8 || roundIndex == 15)
	{
		shiftAmount--;
	}
	*firstHalf = CircularLeftShift28bit(*firstHalf, shiftAmount);
	*secondHalf = CircularLeftShift28bit(*secondHalf, shiftAmount);
}

void DES::DES_GenerateRoundKey(char32_t firstKey, char32_t secondKey, unsigned char result[6])
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			int bitPos = keyCompressionTable[(i * 8) + j] - 1;
			char32_t sourceHalf = firstKey;
			if (bitPos > 27)
			{
				sourceHalf = secondKey;
				bitPos -= 28;
			}
			if (GetBit(result[i], j) != GetBit(sourceHalf, bitPos))
			{
				result[i] = ToggleBit(result[i], j);
			}
		}
	}
}