#include "BaseHUD.h"
#include "SpaceGameController.h"

// Prepares a HUD with lots of reference objects
BaseHUD::BaseHUD(const std::string name, SpaceGameController* const& controller)
{
	_name = name;
	_controller = controller;
	_window = controller->ReturnWindowPtr();
	_gameLogic = controller->ReturnGameLogicPtr();
	_resourceManager = controller->ReturnResourceManagerPtr();

	_clearColor = sf::Color(75, 54, 95);

	Setup();
}

// Destroys all dynamically allocated objects
BaseHUD::~BaseHUD()
{

}

// Does basic HUD setup, can be called whenever resolution changes
void BaseHUD::Setup()
{
	sf::Vector2f resolution = _controller->ReturnResolution();

	// Creates FPS counter
	sf::Font* FPSFont = _resourceManager->LoadFont("Resources/Fonts/FPSFont.ttf");
	FPSText.setFont(*FPSFont);
	FPSText.setPosition(0, 0);
	FPSText.setCharacterSize(resolution.x / 50.0f);
	FPSText.setFillColor(sf::Color::White);
}

void BaseHUD::DrawHUD()
{
	DrawFPS();
}

// Basic run frame function, probably never used
bool BaseHUD::RunFrame()
{
	return HandleEvents();
}

// Runs through basic events and buttons, can be overridden to add custom events or keypresses
bool BaseHUD::HandleEvents()
{
	sf::Event event;
	while (_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			_window->close();
		}
	}
	return false;
}

// Calculates and draws the FPS to the screen
void BaseHUD::DrawFPS()
{
	sf::Time timeSinceLastFrame = _controller->ReturnTimeSinceLastFrame();
	int frameCount = (1.0f / timeSinceLastFrame.asSeconds());
	std::string FPSString = std::to_string(frameCount);
	FPSText.setString(FPSString);
	_window->draw(FPSText);
}

std::string BaseHUD::ReturnHUDName()
{
	return _name;
}

sf::Color BaseHUD::ReturnClearColor()
{
	return _clearColor;
}