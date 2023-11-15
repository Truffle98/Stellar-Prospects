#include "AsteroidType.h"

AsteroidType::AsteroidType()
{

}

AsteroidType::AsteroidType(std::vector<float> thresholds, std::vector<RockType> rockTypes, sf::Color unscannedColor)
{
	_thresholds.resize(thresholds.size());
	_rockTypes.resize(rockTypes.size());
	for (int i = 0; i < thresholds.size(); i++)
	{
		_thresholds[i] = thresholds[i];
		_rockTypes[i] = rockTypes[i];
	}

	_unscannedColor = unscannedColor;
}

float AsteroidType::GetMinThreshold()
{
	return _thresholds[0];
}

RockType AsteroidType::GetRockType(float value)
{
	for (int i = _thresholds.size() - 1; i >= 0; i--)
	{
		if (value > _thresholds[i])
		{
			return _rockTypes[i];
		}
	}
}

sf::Color AsteroidType::GetUnscannedColor()
{
	return _unscannedColor;
}