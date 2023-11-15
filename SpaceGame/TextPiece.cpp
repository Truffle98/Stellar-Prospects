#include "TextPiece.h"

TextPiece::TextPiece(std::string* strValue)
{
	_type = 1;
	_strValue = strValue;
}

TextPiece::TextPiece(float* floatValue)
{
	_type = 2;
	_floatValue = floatValue;
}

TextPiece::TextPiece(int* intValue)
{
	_type = 3;
	_intValue = intValue;
}

std::string TextPiece::ReturnValue()
{
	switch (_type)
	{
	case 1:
		return *_strValue;

	case 2:
	{
		int intValue = std::round(*_floatValue);
		return std::to_string(intValue);
	}
	case 3:
		return std::to_string(*_intValue);
	}
	
}