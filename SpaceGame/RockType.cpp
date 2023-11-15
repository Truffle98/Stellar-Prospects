#include "RockType.h"

RockType::RockType()
{

}

RockType::RockType(std::string name, sf::Color color, std::string colorName, int volume, int value)
{
	_name = name;
	_color = color;
	_colorName = colorName;
	_volume = volume;
	_value = value;
}

std::string* RockType::ReturnNamePtr()
{
	return &_name;
}

sf::Color RockType::ReturnColor()
{
	return _color;
}

std::string* RockType::ReturnColorStringPtr()
{
	return &_colorName;
}

int* RockType::ReturnVolumePtr()
{
	return &_volume;
}

int* RockType::ReturnValuePtr()
{
	return &_value;
}
