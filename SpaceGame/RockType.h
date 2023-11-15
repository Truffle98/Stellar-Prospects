#pragma once
#include <SFML/Graphics.hpp>

// Class to hold data about each tile
class RockType
{

private:

	std::string _name;
	sf::Color _color;
	std::string _colorName;
	int _volume;
	int _value;

public:

	RockType();

	RockType(std::string name, sf::Color color, std::string colorName, int volume, int value);

	std::string* ReturnNamePtr();

	sf::Color ReturnColor();

	std::string* ReturnColorStringPtr();

	int* ReturnVolumePtr();

	int* ReturnValuePtr();
};

