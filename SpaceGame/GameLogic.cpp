#include "GameLogic.h"
#include "SpaceGameController.h"

GameLogic::GameLogic()
{

}

GameLogic::GameLogic(SpaceGameController* controller)
{
	_controller = controller;
	_window = _controller->ReturnWindowPtr();
	_resourceManager = _controller->ReturnResourceManagerPtr();
	sf::Texture* asteroidTexture = _resourceManager->LoadTexture("Resources/Asteroids/baseRock.png");
	_asteroidSprite = sf::Sprite(*asteroidTexture);
	_homeShip = sf::Sprite(*_resourceManager->LoadTexture("Resources/Spaceships/homeShip.png"));
	_homeShip.setOrigin(_homeShip.getLocalBounds().width / 2.0f, _homeShip.getLocalBounds().height / 2.0f);
}

void GameLogic::SetupGame()
{
	_day = 1;
	_month = 1;
	_dayTimer = 120;

	_volumeDelivered = 0;
	_profits = 0;

	CreateAsteroidField();
	CenterSpaceships();

	for (int i = 0; i < _spaceships.size(); i++)
	{
		_spaceships[i].ResetShip();
	}
}

void GameLogic::SetAsteroidFieldResolution(sf::Vector2f topLeft, sf::Vector2f bottomRight)
{
	_topLeft = topLeft;
	_bottomRight = bottomRight;
	_resolution = sf::Vector2f(_bottomRight.x - _topLeft.x, _bottomRight.y - _topLeft.y);
}

// Creates randomly generated asteroid field, needs params
void GameLogic::CreateAsteroidField()
{
	int mapSizeInt = 100;

	float xMult = 1.0f * _resolution.x / _resolution.y;
	_mapSize = sf::Vector2i(mapSizeInt * xMult, mapSizeInt);

	sf::Vector2f pixelScaleFactor(1.0f * _resolution.x / _mapSize.x, 1.0f * _resolution.y / _mapSize.y);

	const sf::Texture* texture = _asteroidSprite.getTexture();
	float textureSize = texture->getSize().x;

	float scaleFactor = pixelScaleFactor.x / textureSize;
	_asteroidSprite.setScale(scaleFactor, scaleFactor);

	_asteroidTiles.resize(_mapSize.x);

	for (std::vector<AsteroidTile>& col : _asteroidTiles)
	{
		col.resize(_mapSize.y);
	}

	sf::Vector2f drawLocation;
	for (int i = 0; i < _mapSize.x; i++)
	{
		for (int j = 0; j < _mapSize.y; j++)
		{
			drawLocation = sf::Vector2f(i * pixelScaleFactor.x + _topLeft.x, j * pixelScaleFactor.y + _topLeft.y);
			_asteroidTiles[i][j] = AsteroidTile(drawLocation, &_asteroidSprite, _window);
		}
	}

	const int typesN = 3;
	std::vector<AsteroidType> types;
	types.resize(typesN);

	float baseThreshold;
	std::vector<float> thresholds;
	thresholds.resize(3);

	std::string names[5] = { "Dracolite", "Galaxite", "Veloxite", "Luminite", "Lustrarock" };

	sf::Color colors[5] = { sf::Color::Cyan, sf::Color::Red, sf::Color::Blue, sf::Color::Magenta, sf::Color::Green };
	std::string colorNames[5] = { "Cyan", "Red", "Blue", "Magenta", "Green" };

	int volumes[5] = { 2, 5, 8, 12, 15 };
	int values[5] = { 5, 10, 20, 25, 40 };

	sf::Color unscannedColors[3] = { sf::Color(54, 34, 4), sf::Color(111, 111, 111), sf::Color(188, 158, 130) };

	_rockTypes.resize(typesN);
	for (int i = 0; i < typesN; i++)
	{
		baseThreshold = ((rand() % 20) + 60.0f) / 100.0f;
		thresholds[0] = baseThreshold;
		baseThreshold += (rand() % 10) / 100.0f;
		thresholds[1] = baseThreshold;
		baseThreshold += (rand() % 10) / 100.0f;
		thresholds[2] = baseThreshold;

		for (int i = 0; i < typesN; i++)
		{
			int colorIdx = rand() % 5;
			_rockTypes[i] = RockType(names[rand() % 5], colors[colorIdx], colorNames[colorIdx], volumes[rand() % 5], values[rand() % 5]);
		}

		types[i] = AsteroidType(thresholds, _rockTypes, unscannedColors[rand() % 3]);
	}

	float minThreshold;
	float noiseValue;
	for (int runs = 0; runs < typesN; runs++)
	{
		_noise.SetFrequency(5.0f / _mapSize.x);
		_noise.SetSeed(rand());
		minThreshold = types[runs].GetMinThreshold();
		for (int i = 0; i < _mapSize.x; i++)
		{
			for (int j = 0; j < _mapSize.y; j++)
			{
				noiseValue = _noise.GetNoise(float(i), float(j));

				if (noiseValue > minThreshold)
				{
					_asteroidTiles[i][j].FillTile(types[runs].GetRockType(noiseValue), types[runs].GetUnscannedColor());
				}
			}
		}
	}
}

void GameLogic::RunFrame()
{
	/*
	for (int i = 0; i < _mapSize.x; i++)
	{
		for (int j = 0; j < _mapSize.y; j++)
		{
			// _asteroidTiles[i][j].RunFrame();
		}
	}
	*/

	for (int i = 0; i < _spaceships.size() ; i++)
	{
		_spaceships[i].RunFrame();
	}

	_dayTimer--;
	if (_dayTimer == 0)
	{
		_day += 1;
		_dayTimer = 120;

		if (_day > 30)
		{
			_month += 1;
			_day = 1;

			if (_month > 1)
			{
				_controller->AddHUD("Endscreen");
				_controller->RemoveHUD("Mining");
			}
		}
	}
}

void GameLogic::DrawAsteroidField()
{
	for (int i = 0; i < _mapSize.x; i++)
	{
		for (int j = 0; j < _mapSize.y; j++)
		{
			_asteroidTiles[i][j].DrawAsteroidTile();
		}
	}
}

// Function to reveal an area on the map with a given radius
void GameLogic::RevealAsteroidTiles(sf::Vector2f location, float radius)
{
	location -= sf::Vector2f(1, 1);
	sf::Vector2i topLeft(std::max(floor(location.x - radius), 0.0f), std::max(floor(location.y - radius), 0.0f));
	sf::Vector2i bottomRight(std::min(ceil(location.x + radius), float(_mapSize.x - 1)), std::min(ceil(location.y + radius), float(_mapSize.y - 1)));
	float distance;

	for (int i = topLeft.x; i <= bottomRight.x; i++)
	{
		for (int j = topLeft.y; j <= bottomRight.y; j++)
		{
			distance = CalculateDistance(location, sf::Vector2f(i, j));
			if (distance <= radius)
			{
				_asteroidTiles[i][j].RevealTile();
			}
		}
	}
}

// Function to reveal an area on the map with a given radius
void GameLogic::ScanAsteroidTiles(sf::Vector2f location, float radius)
{
	location -= sf::Vector2f(1, 1);
	sf::Vector2i topLeft(std::max(floor(location.x - radius), 0.0f), std::max(floor(location.y - radius), 0.0f));
	sf::Vector2i bottomRight(std::min(ceil(location.x + radius), float(_mapSize.x - 1)), std::min(ceil(location.y + radius), float(_mapSize.y - 1)));

	float distance;

	for (int i = topLeft.x; i <= bottomRight.x; i++)
	{
		for (int j = topLeft.y; j <= bottomRight.y; j++)
		{
			distance = CalculateDistance(location, sf::Vector2f(i, j));
			if (distance <= radius)
			{
				_asteroidTiles[i][j].ScanTile();
			}
		}
	}
}

void GameLogic::MineAsteroidTiles(sf::Vector2f location, float radius, Spaceship* ship)
{
	location -= sf::Vector2f(1, 1);
	sf::Vector2i topLeft(std::max(floor(location.x - radius), 0.0f), std::max(floor(location.y - radius), 0.0f));
	sf::Vector2i bottomRight(std::min(ceil(location.x + radius), float(_mapSize.x - 1)), std::min(ceil(location.y + radius), float(_mapSize.y - 1)));

	float distance;

	for (int i = topLeft.x; i <= bottomRight.x; i++)
	{
		for (int j = topLeft.y; j <= bottomRight.y; j++)
		{
			distance = CalculateDistance(location, sf::Vector2f(i, j));
			if (distance <= radius)
			{
				if (_asteroidTiles[i][j].MineTile())
				{
					RockType* type = _asteroidTiles[i][j].ReturnTypePtr();
					if (!ship->AddMinerals(*type->ReturnVolumePtr(), *type->ReturnValuePtr()))
					{
						return;
					}
				}
			}
		}
	}
}

void GameLogic::CreateSpaceships()
{
	for (int i = 0; i < 4; i++)
	{
		Spaceship newShip("ship", _resourceManager, _window, this);
		_spaceships.push_back(newShip);
	}

	_spaceships[0].SetStats(20, 10, 8, 15, 18, 7, 200);
	_spaceships[1].SetStats(12, 25, 15, 8, 15, 4, 1000);
}

void GameLogic::CenterSpaceships()
{
	float angleMult = 2.0f * 3.141592f / _spaceships.size();
	sf::Vector2f center((_mapSize.x / 2.0f) + 0.5, (_mapSize.y / 2.0f) + 0.5);
	for (int i = 0; i < _spaceships.size(); i++)
	{
		float curAngle = angleMult * i;
		sf::Vector2f offset(5.0f * cos(curAngle), 5.0f * sin(curAngle));
		_spaceships[i].SetTransform(offset + center, curAngle * (180.0 / 3.141592f) + 90.0f, ConvertGamePosToPixelLoc(sf::Vector2f(5, 5)).x);
	}

	float newScale = ConvertGamePosToPixelLoc(sf::Vector2f(10, 10)).x;
	newScale /= _homeShip.getTextureRect().width;

	_homeShip.setPosition(ConvertGamePosToPixelLoc(center));
	_homeShip.setScale(newScale, newScale);
}

void GameLogic::DrawSpaceships()
{
	_window->draw(_homeShip);

	for (int i = 0; i < _spaceships.size(); i++)
	{
		_spaceships[i].DrawSpaceship();
	}
}

void GameLogic::DeliverGoods(int volume, int value)
{
	_volumeDelivered += volume;
	_profits += value;
}

float GameLogic::CalculateDistance(sf::Vector2f p1, sf::Vector2f p2)
{
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

sf::Vector2f GameLogic::ConvertPixelLocToGamePos(sf::Vector2f location)
{
	sf::Vector2f pixelScaleFactor(1.0f * _resolution.x / _mapSize.x, 1.0f * _resolution.y / _mapSize.y);

	return sf::Vector2f(location.x / pixelScaleFactor.x, location.y / pixelScaleFactor.y);
}

sf::Vector2f GameLogic::ConvertGamePosToPixelLoc(sf::Vector2f location)
{
	sf::Vector2f pixelScaleFactor(1.0f * _resolution.x / _mapSize.x, 1.0f * _resolution.y / _mapSize.y);

	return sf::Vector2f(location.x * pixelScaleFactor.x, location.y * pixelScaleFactor.y);
}

sf::Vector2i GameLogic::ReturnMapSize()
{
	return _mapSize;
}

std::vector<Spaceship>* GameLogic::ReturnShips()
{
	return &_spaceships;
}

std::vector<float*> GameLogic::ReturnFloatPtrs()
{
	std::vector<float*> ptrs = {  };
	return ptrs;
}

std::vector<int*> GameLogic::ReturnIntPtrs()
{
	std::vector<int*> ptrs = {&_day, &_month, &_volumeDelivered, &_profits, &_mapSize.x, &_mapSize.y };
	return ptrs;
}

std::vector<RockType>* GameLogic::ReturnRockTypes()
{
	return &_rockTypes;
}
