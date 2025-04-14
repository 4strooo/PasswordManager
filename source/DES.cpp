#include "DES.h"

const int parityDropTable[56] =
{
	57, 49, 41, 33, 25, 17, 9, 1,
	58, 50, 42, 34, 26, 18, 10, 2,
	59, 51, 43, 35, 27, 19, 11, 3,
	60, 52, 44, 36, 63, 55, 47, 39,
	31, 23, 15, 7, 62, 54, 46, 38,
	30, 22, 14, 6, 61, 53, 45, 37,
	29, 21, 13, 5, 28, 20, 12, 4
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

const int initialPermutation[64] =
{
	20, 63, 41, 7, 38, 57, 37, 11, 51, 1, 33, 43, 17, 40, 15, 47,
	27, 64, 9, 59, 3, 45, 49, 42, 62, 18, 6, 52, 30, 48, 58, 5,
	61, 13, 21, 39, 55, 2, 60, 26, 32, 12, 35, 25, 19, 10, 53, 36,
	50, 56, 4, 28, 46, 14, 24, 16, 34, 29, 44, 22, 23, 8, 54, 31
};

const int expansionPermutation[48] =
{
	32, 1, 2, 3, 4, 5, 4, 5,
	6, 7, 8, 9, 8, 9, 10, 11,
	12, 13, 12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 20, 21,
	22, 23, 24, 25, 24, 25, 26, 27,
	28, 29, 28, 29, 30, 31, 32, 1
};

const int sBoxes[8][4][16] =
{
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},
	{
		{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
		{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
		{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
		{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
	},
	{
		{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
		{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
		{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
		{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
	},
	{
		{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
		{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
		{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
		{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
	},
	{
		{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
		{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
		{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
		{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
	},
	{
		{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
		{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
		{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
		{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
	},
	{
		{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
		{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
		{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
		{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
	},
	{
		{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
		{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
		{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
		{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
	}
};

const int pBoxPermutation[32] =
{
	16, 7, 20, 21, 29, 12, 28, 17,
	1, 15, 23, 26, 5, 18, 31, 10,
	2, 8, 24, 14, 32, 27, 3, 9,
	19, 13, 30, 6, 22, 11, 4, 25
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
	for (int i = 0; i < 7; i++)
	{
		result[i] = data[i];
		for (int ii = 0; ii < 8; ii++)
		{
			int bit = parityDropTable[(i * 8) + ii] - 1;
			unsigned char source = data[bit / 8];
			if (GetBit(result[i], ii) != GetBit(source, bit % 8))
			{
				result[i] = ToggleBit(result[i], ii);
			}
		}
	}
}

void DES::DES_InitialPermutation(unsigned char block[8], unsigned char left[4], unsigned char right[4])
{
	unsigned char result[8];
	for (int i = 0; i < 8; i++)
	{
		result[i] = block[i];
		for (int ii = 0; ii < 8; ii++)
		{
			int index = initialPermutation[(i * 8) + ii] - 1;
			unsigned char source = block[index / 8];
			if(GetBit(result[i], ii) != GetBit(source, index % 8))
			{
				result[i] = ToggleBit(result[i], ii);
			}
		}
	}
	for (int j = 0; j < 4; j++)
	{
		left[j] = result[j];
		right[j] = result[j + 4];
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

void DES::DES_ExpansionPermutation(unsigned char block[4], unsigned char result[6])
{
	for (int i = 0; i < 6; i++)
	{
		for (int ii = 0; ii < 8; ii++)
		{
			int bit = expansionPermutation[(i * 8) + ii] - 1;
			unsigned char source = block[bit / 8];
			if (GetBit(result[i], ii) != GetBit(source, bit % 8))
			{
				result[i] = ToggleBit(result[i], ii);
			}
		}
	}
}

void DES::DES_MixWithKey(unsigned char key[6], unsigned char result[6])
{
	for (int i = 0; i < 6; i++)
	{
		result[i] = result[i] ^ key[i];
	}
}

void DES::DES_SBoxPermutation(unsigned char block[6], unsigned char result[4])
{
	int outputs[8];
	for (int i = 0; i < 8; i++)
	{
		int row = 0;
		int headBit = i * 6;
		unsigned char headSource = block[headBit / 8];
		unsigned char tailSource = block[(headBit + 5) / 8];
		if (GetBit(headSource, headBit % 8))
		{
			row++;
		}
		if (GetBit(tailSource, (headBit + 5) % 8))
		{
			row += 2;
		}
		int column = 0;
		for (int ii = 0; ii < 4; ii++)
		{
			headBit++;
			unsigned char source = block[headBit / 8];
			if (GetBit(source, headBit % 8))
			{
				column += (1 << ii);
			}
		}
		outputs[i] = sBoxes[i][row][column];
	}
	for (int j = 0; j < 4; j++)
	{
		result[j] = outputs[j * 2] + (outputs[(j * 2) + 1] << 4);
	}

}

void DES::DES_PBoxPermutation(unsigned char data[4], unsigned char result[4])
{
	for (int i = 0; i < 4; i++)
	{
		result[i] = data[i];
		for (int ii = 0; ii < 8; ii++)
		{
			int bit = pBoxPermutation[(i * 8) + ii] - 1;
			unsigned char source = data[bit / 8];
			if (GetBit(result[i], ii) != GetBit(source, bit % 8))
			{
				result[i] = ToggleBit(result[i], ii);
			}
		}
	}
}

void DES::DES_XORHalves(unsigned char left[4], unsigned char right[4])
{
	for (int i = 0; i < 4; i++)
	{
		right[i] = right[i] ^ left[i];
	}
}

void DES::DES_FinalPermutation(unsigned char left[4], unsigned char right[4], unsigned char result[8])
{
	unsigned char joined[8];
	for (int i = 0; i < 4; i++)
	{
		joined[i] = left[i];
		joined[i + 4] = right[i];
	}
	int inverseTable[64];
	for (int j = 0; j < 64; j++)
	{
		inverseTable[initialPermutation[j] - 1] = j + 1;
	}
	for (int k = 0; k < 8; k++)
	{
		result[k] = joined[k];
		for (int kk = 0; kk < 8; kk++)
		{
			int bit = inverseTable[(k * 8) + kk] - 1;
			unsigned char source = joined[bit / 8];
			if (GetBit(result[k], kk) != GetBit(source, bit % 8))
			{
				result[k] = ToggleBit(result[k], kk);
			}
		}
	}
}