#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class SpaceGameController;
class GameLogic;
class ResourceManager;

class BaseHUD
{

protected:

	std::string _name;
	SpaceGameController* _controller;
	sf::RenderWindow* _window;
	GameLogic* _gameLogic;
	ResourceManager* _resourceManager;
	
	sf::Color _clearColor;
	sf::Text FPSText;


public:

	BaseHUD(const std::string name, SpaceGameController* const& controller);

	~BaseHUD();

	void Setup();

	virtual void DrawHUD();

	virtual bool RunFrame();

	virtual bool HandleEvents();

	void DrawFPS();

	std::string ReturnHUDName();

	sf::Color ReturnClearColor();

};

