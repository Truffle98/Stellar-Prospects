#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>

#include "PlayerFunction.h"
#include "Text.h"
#include "TextPiece.h"
#include "Spaceship.h"

class SpaceGameController;
class GameLogic;
class ResourceManager;

// Class to do console operations on
class ConsoleBase
{

private:

	sf::Vector2u _charResolution;
	int _numErrorLines;
	sf::Vector2f _topLeft;
	sf::Vector2f _bottomRight;

	sf::Text _text;
	sf::Font* _font;
	sf::RenderTexture _render;
	sf::Sprite _sprite;
	sf::Color _textColor;
	sf::Color _playerColor;
	sf::RectangleShape _clearRect;

	SpaceGameController* _controller;
	sf::RenderWindow* _window;
	GameLogic* _gameLogic;
	ResourceManager* _resourceManager;
	std::vector<PlayerFunction> _playerFuncs;

	char* _consoleText;
	char* _errorText;
	std::stack<Text*> _queuedText;
	Text* _curQueue;
	int _underscoreTimer;
	int _errorTimer;

public:

	ConsoleBase(sf::Vector2u charResolution, sf::Vector2f topLeft, sf::Vector2f bottomRight, SpaceGameController* controller);

	~ConsoleBase();

	void ManageText();

	void DisplayConsole();

	void ClearConsole();

	void ClearError();

	void AddQueue(Text* text);

	void SetError(std::string error);

	void HandleInput(char c);

	void HandleInput(bool deleteAll);

	bool ReadConsoleInput();

	int FindNextChar(std::string str, int start, char c);

	void AddPlayerFunction(PlayerFunction newFunc);

	//bool CheckArgTypes(std::string types[], std::vector<std::string> args);

	bool CheckPlayerFunction(const std::vector<std::string>& args);

	//Player functions
	bool ExitGame(const std::vector<std::string>& args);

	bool ChangeTextColor(const std::vector<std::string>& args);

	bool ChangeConsoleColor(const std::vector<std::string>& args);

	bool StartGame(const std::vector<std::string>& args);

	bool Help(const std::vector<std::string>& args);

	bool Back(const std::vector<std::string>& args);

	bool Clear(const std::vector<std::string>& args);

	bool MoveShip(const std::vector<std::string>& args);

	bool StartScan(const std::vector<std::string>& args);

	bool StartMine(const std::vector<std::string>& args);

	bool ReturnShip(const std::vector<std::string>& args);

	bool MapInfo(const std::vector<std::string>& args);

	bool ShipInfo(const std::vector<std::string>& args);
};

