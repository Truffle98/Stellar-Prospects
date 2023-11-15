#pragma once
#include <cmath>

#include "SFML/Graphics.hpp"

class GameLogic;
class ResourceManager;

class Spaceship
{
private:

	// Transform values
	sf::Vector2f _location;
	float _rotation;

	sf::Texture* _idleTexture;
	sf::Texture* _movingTexture;
	sf::Sprite _sprite;

	sf::RenderWindow* _window;
	GameLogic* _gameLogic;

	bool _isNull;
	// Spaceship stats
	float _miningSpeed;
	float _miningRadius;
	float _scanningSpeed;
	float _scanningRadius;
	float _revealRadius;
	float _moveSpeed;
	int _carryCapacity;

	// Storage values
	int _curVolume;
	int _curValue;

	// Operations are numbered as follows
	// 1 - Idle, 2 - Moving, 3 - Scanning, 4 - Mining
	int _curOperation;
	sf::Vector2f _targetLocation;
	int _actionTimer;
	std::string _curStatus;

public:

	Spaceship(std::string spriteName, ResourceManager* manager, sf::RenderWindow* window, GameLogic* gameLogic);

	void ResetShip();

	void RunFrame();

	void DrawSpaceship();

	void SetMoving(sf::Vector2f targetLocation);

	void SetScanning();

	void SetMining();

	void SetReturning();

	void SetStats(float miningSpeed, float miningRadius, float scanningSpeed, float scanningRadius, float revealRadius, float moveSpeed, int carryCapacity);

	void SetTransform(sf::Vector2f newLoc, float newRotation, float newScale);

	bool AddMinerals(int volume, int value);

	std::string* ReturnStatusPtr();

	bool ReturnIsNull();

	std::vector<float*> ReturnFloatPtrs();

	std::vector<int*> ReturnIntPtrs();

};

