#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <iostream>
#include <ctime>

#include "BaseHUD.h"
#include "MenuHUD.h"
#include "MiningHUD.h"
#include "EndscreenHUD.h"
#include "GameLogic.h"
#include "ResourceManager.h"

class SpaceGameController
{
private:

	sf::Vector2f _resolution;
	sf::RenderWindow _window;
	sf::Clock _clock;

	GameLogic _gameLogic;
	ResourceManager _resourceManager;

    std::vector<BaseHUD*> _huds;
	std::list<BaseHUD*> _currentHUDs;

public:

	SpaceGameController(const sf::Vector2f& resolution, const bool& fullscreen);

	~SpaceGameController();

	void SetupHUDS();

	void StartGame();

	void AddHUD(const std::string& HUDName);

	void RemoveHUD(const std::string& HUDName);

	sf::RenderWindow* ReturnWindowPtr();

	GameLogic* ReturnGameLogicPtr();

	ResourceManager* ReturnResourceManagerPtr();

	sf::Vector2f ReturnResolution();

	sf::Time ReturnTimeSinceLastFrame();

};

