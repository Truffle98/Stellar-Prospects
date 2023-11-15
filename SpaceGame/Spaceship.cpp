#include "Spaceship.h"
#include "GameLogic.h"
#include "SpaceGameController.h"
#include "ResourceManager.h"

Spaceship::Spaceship(std::string spriteName, ResourceManager* manager, sf::RenderWindow* window, GameLogic* gameLogic)
{
	_location = sf::Vector2f(0, 0);
	_rotation = 0;

	_idleTexture = manager->LoadTexture("Resources/Spaceships/" + spriteName + "Idle.png");
	_movingTexture = manager->LoadTexture("Resources/Spaceships/" + spriteName + "Moving.png");

	_sprite = sf::Sprite(*_idleTexture);
	_sprite.setOrigin(_sprite.getLocalBounds().width / 2.0f, _sprite.getLocalBounds().height / 2.0f);

	_window = window;
	_gameLogic = gameLogic;

	_isNull = true;

	_curOperation = 0;
	_targetLocation = sf::Vector2f(0, 0);
	_actionTimer = 0;
	_curStatus = "Null";
}

void Spaceship::ResetShip()
{
	_curVolume = 0;
	_curValue = 0;
}

void Spaceship::RunFrame()
{
	if (_isNull)
	{
		return;
	}

	_gameLogic->RevealAsteroidTiles(_location, _revealRadius);

	switch (_curOperation)
	{
	case 1:
		return;

	case 2:
	{
		float xScale = cos(_rotation);
		float yScale = sin(_rotation);

		float moveSpeed = _moveSpeed / 60.0f;

		sf::Vector2f totalMove(xScale * moveSpeed, yScale * moveSpeed);
		_location += totalMove;

		sf::Vector2f locationDiff = _targetLocation - _location;
		locationDiff = sf::Vector2f(fabsf(locationDiff.x), fabsf(locationDiff.y));
		if (locationDiff.x + locationDiff.y < 0.2)
		{
			_curOperation = 1;
			_curStatus = "Idle";
			_sprite.setTexture(*_idleTexture);
		}

		return;
	}
	case 3:
		
		if (_actionTimer > 0)
		{
			_actionTimer--;
			return;
		}

		_curOperation = 1;
		_curStatus = "Idle";
		_gameLogic->ScanAsteroidTiles(_location, _scanningRadius);
		return;

	case 4:

		if (_actionTimer > 0)
		{
			_actionTimer--;
			return;
		}

		_curOperation = 1;
		_curStatus = "Idle";
		_gameLogic->MineAsteroidTiles(_location, _scanningRadius, this);
		return;

	case 5:

		float xScale = cos(_rotation);
		float yScale = sin(_rotation);

		float moveSpeed = _moveSpeed / 60.0f;

		sf::Vector2f totalMove(xScale * moveSpeed, yScale * moveSpeed);
		_location += totalMove;

		sf::Vector2f locationDiff = _targetLocation - _location;
		locationDiff = sf::Vector2f(fabsf(locationDiff.x), fabsf(locationDiff.y));
		if (locationDiff.x + locationDiff.y < 0.2)
		{
			_gameLogic->DeliverGoods(_curVolume, _curValue);
			ResetShip();
			_curOperation = 1;
			_curStatus = "Idle";
			_sprite.setTexture(*_idleTexture);
		}

		return;
	}
}

void Spaceship::DrawSpaceship()
{
	if (_isNull)
	{
		return;
	}

	sf::Vector2f pixelLoc = _gameLogic->ConvertGamePosToPixelLoc(_location);
	_sprite.setPosition(pixelLoc);
	_window->draw(_sprite);
}

void Spaceship::SetMoving(sf::Vector2f targetLocation)
{
	_targetLocation = targetLocation;

	sf::Vector2f locationDiff = _targetLocation - _location;
	float newRotation = atan2(locationDiff.y, locationDiff.x);

	_rotation = newRotation;
	_sprite.setRotation(_rotation * (180.0f / 3.141592) + 90.0f);
	_sprite.setTexture(*_movingTexture);
	_curOperation = 2;
	_curStatus = "Moving";
}

void Spaceship::SetScanning()
{
	_actionTimer = _scanningSpeed * 60;
	_curOperation = 3;
	_curStatus = "Scanning";
}

void Spaceship::SetMining()
{
	_actionTimer = _miningSpeed * 60;
	_curOperation = 4;
	_curStatus = "Mining";
}

void Spaceship::SetReturning()
{
	sf::Vector2i mapSize = _gameLogic->ReturnMapSize();
	_targetLocation = sf::Vector2f((mapSize.x / 2.0f) + 0.5, (mapSize.y / 2.0f) + 0.5);

	sf::Vector2f locationDiff = _targetLocation - _location;
	float newRotation = atan2(locationDiff.y, locationDiff.x);

	_rotation = newRotation;
	_sprite.setRotation(_rotation * (180.0f / 3.141592) + 90.0f);
	_sprite.setTexture(*_movingTexture);
	_curOperation = 5;
	_curStatus = "Returning";
}

void Spaceship::SetStats(float miningSpeed, float miningRadius, float scanningSpeed, float scanningRadius, float revealRadius, float moveSpeed, int carryCapacity)
{
	_isNull = false;
	_curStatus = "Idle";

	_miningSpeed = miningSpeed;
	_scanningSpeed = scanningSpeed;
	_miningRadius = miningRadius;
	_scanningRadius = scanningRadius;
	_revealRadius = revealRadius;
	_moveSpeed = moveSpeed;
	_carryCapacity = carryCapacity;
}

void Spaceship::SetTransform(sf::Vector2f newLoc, float newRotation, float newScale)
{
	_location = newLoc;
	_rotation = newRotation;

	_sprite.setRotation(_rotation);

	newScale /= _sprite.getTextureRect().width;
	_sprite.setScale(newScale, newScale);
}

bool Spaceship::AddMinerals(int volume, int value)
{
	if (volume > (_carryCapacity - _curVolume))
	{
		return false;
	}

	_curVolume += volume;
	_curValue += value;
	return true;
}

std::string* Spaceship::ReturnStatusPtr()
{
	return &_curStatus;
}

bool Spaceship::ReturnIsNull()
{
	return _isNull;
}

std::vector<float*> Spaceship::ReturnFloatPtrs()
{
	std::vector<float*> ptrs = { &_miningSpeed, &_scanningSpeed, &_miningRadius, &_scanningRadius, 
		&_revealRadius, &_moveSpeed, &_location.x, &_location.y };
	return ptrs;
}

std::vector<int*> Spaceship::ReturnIntPtrs()
{
	std::vector<int*> ptrs = { &_carryCapacity, &_curVolume, &_curValue };
	return ptrs;
}