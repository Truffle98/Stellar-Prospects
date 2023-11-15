#pragma once
#include <SFML/Graphics.hpp>

#include "AsteroidType.h"

class AsteroidTile
{
private:

	sf::Vector2f _drawLocation;
	sf::Color _color;
	sf::Color _unscannedColor;

	RockType _type;

	bool _empty;
	bool _revealed;
	bool _scanned;
	bool _mined;
	
	// More variables for gameplay logic, not important yet

	sf::Sprite* _asteroidSprite;
	sf::RenderWindow* _window;
	
public:

	AsteroidTile();
	
	AsteroidTile(sf::Vector2f drawLocation, sf::Sprite* asteroidSprite, sf::RenderWindow* window);

	AsteroidTile(sf::Vector2f drawLocation, sf::Color color, sf::Sprite* asteroidSprite, sf::RenderWindow* window);

	void FillTile(RockType type, sf::Color unscannedColor);

	void RunFrame();

	void DrawAsteroidTile();

	void RevealTile();

	void ScanTile();

	bool MineTile();

	RockType* ReturnTypePtr();
};

