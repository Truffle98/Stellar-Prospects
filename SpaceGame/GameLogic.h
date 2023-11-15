#pragma once
#include <vector>
#include <cstdlib>
#include <algorithm>

#include "ResourceManager.h"
#include "AsteroidTile.h"
#include "Spaceship.h"
#include "PerlinNoise.h"

class SpaceGameController;

class GameLogic
{

private:

	SpaceGameController* _controller;
	sf::RenderWindow* _window;
	ResourceManager* _resourceManager;
	FastNoiseLite _noise;
	sf::Sprite _asteroidSprite;

	// Asteroid mining variables
	sf::Vector2f _topLeft;
	sf::Vector2f _bottomRight;
	sf::Vector2f _resolution;
	sf::Vector2i _mapSize;
	std::vector<std::vector<AsteroidTile>> _asteroidTiles;
	std::vector<RockType> _rockTypes;

	// Spaceship variables
	std::vector<Spaceship> _spaceships;
	sf::Sprite _homeShip;

	// Tracking numbers
	int _day;
	int _month;
	int _dayTimer;

	int _volumeDelivered;
	int _profits;

public:

	GameLogic();

	GameLogic(SpaceGameController* controller);

	void SetupGame();

	void SetAsteroidFieldResolution(sf::Vector2f topLeft, sf::Vector2f bottomRight);

	void CreateAsteroidField();

	void RunFrame();

	void DrawAsteroidField();

	void RevealAsteroidTiles(sf::Vector2f location, float radius);

	void ScanAsteroidTiles(sf::Vector2f location, float radius);

	void MineAsteroidTiles(sf::Vector2f location, float radius, Spaceship* ship);

	void CreateSpaceships();

	void CenterSpaceships();

	void DrawSpaceships();

	void DeliverGoods(int volume, int value);

	float CalculateDistance(sf::Vector2f p1, sf::Vector2f p2);

	sf::Vector2f ConvertPixelLocToGamePos(sf::Vector2f location);

	sf::Vector2f ConvertGamePosToPixelLoc(sf::Vector2f location);

	sf::Vector2i ReturnMapSize();

	std::vector<Spaceship>* ReturnShips();

	std::vector<float*> ReturnFloatPtrs();

	std::vector<int*> ReturnIntPtrs();

	std::vector<RockType>* ReturnRockTypes();
};

