#pragma once
#include <iostream>

#include "RockType.h"

class AsteroidType
{

private:

	std::vector<float> _thresholds;
	std::vector<RockType> _rockTypes;
	sf::Color _unscannedColor;

public:

	AsteroidType();

	AsteroidType(std::vector<float> thresholds, std::vector<RockType> rockTypes, sf::Color unscannedColor);

	float GetMinThreshold();

	RockType GetRockType(float value);

	sf::Color GetUnscannedColor();
};
