#include "AsteroidTile.h"
#include <iostream>

AsteroidTile::AsteroidTile()
{

}

AsteroidTile::AsteroidTile(sf::Vector2f drawLocation, sf::Sprite* asteroidSprite, sf::RenderWindow* window)
{
	_drawLocation = drawLocation;

	_empty = true;
	_revealed = false;
	_scanned = false;
	_mined = false;

	_asteroidSprite = asteroidSprite;
	_window = window;
}

AsteroidTile::AsteroidTile(sf::Vector2f drawLocation, sf::Color color, sf::Sprite* asteroidSprite, sf::RenderWindow* window)
{
	_drawLocation = drawLocation;

	sf::Color noiseColor(int(rand() % 10), int(rand() % 10), int(rand() % 10));
	_color = color + noiseColor;
	_unscannedColor = sf::Color(54, 34, 4) + noiseColor;

	_empty = false;
	_revealed = false;
	_scanned = false;
	_mined = false;

	_asteroidSprite = asteroidSprite;
	_window = window;
}

void AsteroidTile::FillTile(RockType type, sf::Color unscannedColor)
{
	_type = type;

	sf::Color noiseColor(int(rand() % 10), int(rand() % 10), int(rand() % 10));
	_color = _type.ReturnColor() + noiseColor;
	_unscannedColor = unscannedColor + noiseColor;

	_empty = false;
}

void AsteroidTile::RunFrame()
{
	_revealed = true;
}

void AsteroidTile::DrawAsteroidTile()
{
	if (_revealed)
	{
		if (_empty)
		{
			return;
		}
		else
		{
			if (_scanned)
			{
				if (_mined)
				{
					_asteroidSprite->setColor(sf::Color(111, 111, 111));
				}
				else
				{
					_asteroidSprite->setColor(_color);
				}
			}
			else
			{
				_asteroidSprite->setColor(_unscannedColor);
			}
		}
	}
	else
	{
		_asteroidSprite->setColor(sf::Color::Black);
	}

	_asteroidSprite->setPosition(_drawLocation);
	_window->draw(*_asteroidSprite);
}

void AsteroidTile::RevealTile()
{
	_revealed = true;
}

void AsteroidTile::ScanTile()
{
	if (_revealed && !_empty)
	{
		_scanned = true;
	}
}

bool AsteroidTile::MineTile()
{
	if (_scanned && !_mined)
	{
		_mined = true;
		return true;
	}

	return false;
}

RockType* AsteroidTile::ReturnTypePtr()
{
	return &_type;
}