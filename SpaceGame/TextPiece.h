#pragma once

#include <string>
#include <cmath>

class TextPiece
{

private:

	// String is 1, float is 2, int is 3
	int _type;
	std::string* _strValue;
	float* _floatValue;
	int* _intValue;

public:
	
	TextPiece(std::string* strValue);

	TextPiece(float* floatValue);

	TextPiece(int* intValue);

	std::string ReturnValue();
};

