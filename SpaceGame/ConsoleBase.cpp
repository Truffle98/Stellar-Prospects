#include "ConsoleBase.h"

#include "SpaceGameController.h"

ConsoleBase::ConsoleBase(sf::Vector2u charResolution, sf::Vector2f topLeft, sf::Vector2f bottomRight, SpaceGameController* controller)
{
	_charResolution = charResolution;
	_numErrorLines = (60 / _charResolution.x);
	_topLeft = topLeft;
	_bottomRight = bottomRight;

	_controller = controller;
	_window = _controller->ReturnWindowPtr();
	_gameLogic = _controller->ReturnGameLogicPtr();
	_resourceManager = _controller->ReturnResourceManagerPtr();

	_render.create(200, 400);

	_font = _resourceManager->LoadFont("Resources/Fonts/consoleFont.ttf");
	_text.setFont(*_font);
	_textColor = sf::Color::Red;
	_playerColor = sf::Color::Green;

	sf::Vector2f resolution(_bottomRight.x - _topLeft.x, _bottomRight.y - _topLeft.y);
	_clearRect.setPosition(_topLeft);
	_clearRect.setSize(resolution);
	_clearRect.setFillColor(sf::Color::Black);
	_clearRect.setOutlineThickness(-std::min(resolution.y * 0.005f, resolution.x * 0.005f));
	_clearRect.setOutlineColor(sf::Color::White);

	_consoleText = new char[_charResolution.x];
	_errorText = new char[_charResolution.x];
	_underscoreTimer = 0;
	ClearConsole();
	ClearError();
}

ConsoleBase::~ConsoleBase()
{
	delete _consoleText;
	delete _errorText;
}

void ConsoleBase::ManageText()
{
	_curQueue->ManageText();
}

void ConsoleBase::DisplayConsole()
{
	_window->draw(_clearRect);

	sf::Vector2f resolution(_bottomRight.x - _topLeft.x, _bottomRight.y - _topLeft.y);
	sf::Vector2f border(resolution.x * 0.025f, resolution.y * 0.025f);
	float playerConsoleSize = _bottomRight.y - (resolution.y * 0.9f);

	// Text area
	sf::Vector2f textTopLeft(_topLeft.x + border.x, _topLeft.y + border.y);
	sf::Vector2f textBottomRight(_bottomRight.x - border.x, _bottomRight.y - playerConsoleSize - border.y);

	// Player text and error area
	sf::Vector2f playerConsoleTopLeft(textTopLeft.x, textBottomRight.y);
	sf::Vector2f errorTopLeft(textTopLeft.x, resolution.y - (_numErrorLines * border.y * 2.5f));
	sf::Vector2f consoleSize(textBottomRight.x - textTopLeft.x, textBottomRight.y - textTopLeft.y);
	sf::Vector2f charSize(consoleSize.x / _charResolution.x, consoleSize.y / _charResolution.y);

	_text.setCharacterSize(320);
	_text.setPosition(100, 200);

	_text.setString("A");
	sf::FloatRect textRect = _text.getLocalBounds();
	_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	
	_sprite.setScale(charSize.x / 200, charSize.y / 400);

	_text.setFillColor(_textColor);

	// Draw text
	std::string curText = _curQueue->ReturnText();
	int iter = 0, drawIter = 0;
	int row, col;
	
	while (curText[iter] != 0 && drawIter < _charResolution.x * _charResolution.y)
	{
		if (curText[iter] == ' ')
		{
			iter++;
			drawIter++;

			if (_charResolution.x - (drawIter % _charResolution.x) < FindNextChar(curText, iter, ' ') - iter)
			{
				drawIter /= _charResolution.x;
				drawIter++;
				drawIter *= _charResolution.x;
			}

			else if (((drawIter - 1) % _charResolution.x) == 0)
			{
				drawIter--;
			}
		}
		else if (curText[iter] == '\n')
		{
			drawIter /= _charResolution.x;
			drawIter++;
			drawIter *= _charResolution.x;
			iter++;
		}
		else
		{
			_text.setString(curText[iter]);
			_render.clear();
			_render.draw(_text);
			_render.display();

			row = drawIter / _charResolution.x;
			col = drawIter % _charResolution.x;

			_sprite.setPosition(sf::Vector2f(textTopLeft.x + col * charSize.x, textTopLeft.y + row * charSize.y));
			_sprite.setTexture(_render.getTexture());
			_window->draw(_sprite);

			iter++;
			drawIter++;
		}
	}

	// Draw underscore in text
	if (_curQueue->ReturnWriting())
	{
		_text.setString('_');
		_render.clear();
		_render.draw(_text);
		_render.display();

		row = drawIter / _charResolution.x;
		col = drawIter % _charResolution.x;

		_sprite.setPosition(sf::Vector2f(textTopLeft.x + col * charSize.x, textTopLeft.y + row * charSize.y));
		_sprite.setTexture(_render.getTexture());
		_window->draw(_sprite);
	}

	_text.setFillColor(sf::Color::Red);
	// Draw error text
	if (_errorTimer > 0)
	{
		_errorTimer--;

		iter = 0, drawIter = 0;
		while (_errorText[iter] != 0 && iter < _charResolution.x * _numErrorLines)
		{
			if (_errorText[iter] == ' ')
			{
				iter++;
				drawIter++;

				if (_charResolution.x - (drawIter % _charResolution.x) < FindNextChar(_errorText, iter, ' ') - iter)
				{
					drawIter /= _charResolution.x;
					drawIter++;
					drawIter *= _charResolution.x;
				}

				else if (((drawIter - 1) % _charResolution.x) == 0)
				{
					drawIter--;
				}
			}
			else if (_errorText[iter] == '\n')
			{
				drawIter /= _charResolution.x;
				drawIter++;
				drawIter *= _charResolution.x;
				iter++;
			}
			else
			{
				_text.setString(_errorText[iter]);
				_render.clear();
				_render.draw(_text);
				_render.display();

				row = drawIter / _charResolution.x;
				col = drawIter % _charResolution.x;

				_sprite.setPosition(sf::Vector2f(errorTopLeft.x + col * charSize.x, errorTopLeft.y + (row + _numErrorLines - 1) * charSize.y));
				_sprite.setTexture(_render.getTexture());
				_window->draw(_sprite);

				iter++;
				drawIter++;
			}
		}
	}
	else if (_errorTimer == 0)
	{
		ClearError();
		_errorTimer--;
	}

	// Draw player text
	_text.setFillColor(_playerColor);

	iter = 0;
	while (_consoleText[iter] != 0 && iter < _charResolution.x)
	{
		_text.setString(_consoleText[iter]);
		_render.clear();
		_render.draw(_text);
		_render.display();

		col = iter % _charResolution.x;

		_sprite.setPosition(sf::Vector2f(playerConsoleTopLeft.x + col * charSize.x, playerConsoleTopLeft.y));
		_sprite.setTexture(_render.getTexture());
		_window->draw(_sprite);

		iter++;
	}

	_underscoreTimer++;
	_underscoreTimer %= 90;

	// Draw underscore in player text
	if (iter != _charResolution.x && _underscoreTimer < 45)
	{
		_text.setString('_');
		_render.clear();
		_render.draw(_text);
		_render.display();
		
		col = iter % _charResolution.x;

		_sprite.setPosition(sf::Vector2f(playerConsoleTopLeft.x + col * charSize.x, playerConsoleTopLeft.y));
		_sprite.setTexture(_render.getTexture());
		_window->draw(_sprite);
	}	
}

void ConsoleBase::ClearConsole()
{
	for (int i = 0; i < _charResolution.x; i++)
	{
		_consoleText[i] = 0;
	}
}

void ConsoleBase::ClearError()
{
	_errorTimer = -1;
	for (int i = 0; i < _charResolution.x; i++)
	{
		_errorText[i] = 0;
	}
}

void ConsoleBase::AddQueue(Text* text)
{
	_queuedText.push(text);
	_curQueue = _queuedText.top();
}

void ConsoleBase::SetError(std::string error)
{
	ClearError();
	_errorTimer = 600;
	for (int i = 0; i < error.length(); i++)
	{
		_errorText[i] = error[i];
	}
	_errorText[error.length()] = 0;
}

void ConsoleBase::HandleInput(char c)
{
	_underscoreTimer = 0;

	int iter = 0;
	while (_consoleText[iter] != 0)
	{
		iter++;
	}

	if (iter < _charResolution.x)
	{
		_consoleText[iter] = c;
	}
}

void ConsoleBase::HandleInput(bool deleteAll)
{
	_underscoreTimer = 0;

	if (!deleteAll)
	{
		int iter = 0;
		while (_consoleText[iter] != 0 && iter < _charResolution.x)
		{
			iter++;
		}

		if (iter > 0)
		{
			_consoleText[iter - 1] = 0;
		}
	}
	else
	{
		ClearConsole();
	}
}

bool ConsoleBase::ReadConsoleInput()
{
	std::string cleanStr = "";

	if (_consoleText[0] == 0)
	{
		_curQueue->FinishText();
		return true;
	}

	int iter = 0;
	while (_consoleText[iter] == ' ')
	{
		iter++;
	}

	while (_consoleText[iter] != 0)
	{
		if (_consoleText[iter] == ' ' && cleanStr[cleanStr.length() - 1] == ' ')
		{
			iter++;
			continue;
		}

		cleanStr += _consoleText[iter];
		iter++;
	}

	for (char& c : cleanStr) 
	{
		c = std::tolower(c);
	}

	std::vector<std::string> args;
	int curStart = 0, nextBreak;
	while (curStart < cleanStr.length())
	{
		nextBreak = FindNextChar(cleanStr, curStart, ' ');
		std::string arg = cleanStr.substr(curStart, nextBreak - curStart);
		args.push_back(arg);
		curStart = nextBreak + 1;
	}

	CheckPlayerFunction(args);

	ClearConsole();
}

int ConsoleBase::FindNextChar(std::string str, int start, char c)
{
	bool inQuotes = false;
	for (int i = start; i < str.length(); i++)
	{
		if (str[i] == '"')
		{
			inQuotes = !inQuotes;
		}

		if (inQuotes)
		{
			continue;
		}

		if (str[i] == c)
		{
			return i;
		}
	}
	return str.length();
}

void ConsoleBase::AddPlayerFunction(PlayerFunction newFunc)
{
	_playerFuncs.push_back(newFunc);
}

/*
bool ConsoleBase::CheckArgTypes(std::string types[], std::vector<std::string> args)
{
	if (types->length() != args.size() - 1)
	{
		return false;
	}

	for (int i = 0; i < types->length(); i++)
	{
		if (types[i] == "int")
		{
			try 
			{
				int intValue = std::stoi(args[i + 1]);
			}
			catch (const std::exception& e) 
			{
				return false;
			}
		}
	}

	return true;
}
*/

bool ConsoleBase::CheckPlayerFunction(const std::vector<std::string>& args)
{
	for (PlayerFunction playerFunc : _playerFuncs)
	{
		if (args[0] == playerFunc.ReturnName())
		{
			return playerFunc.CallFunc(this, args);
		}
	}

	SetError("Function name not recognized in this context");
	return false;
}

// Player used commands
bool ConsoleBase::ExitGame(const std::vector<std::string>& args)
{
	if (args.size() != 1)
	{
		SetError("Incorrect arg count for exitgame function");
		return false;
	}

	std::string exitStr = "Type exitgame again to close game";
	for (int i = 0; i < exitStr.length(); i++)
	{
		if (exitStr[i] != _errorText[i])
		{
			SetError("Type exitgame again to close game");
			return false;
		}
	}

	_window->close();
	return true;
}

bool ConsoleBase::ChangeTextColor(const std::vector<std::string>& args)
{
	std::vector<std::string> params = {"int", "int", "int"};
	if (!PlayerFunction::CheckArgs(args, params))
	{
		SetError("Incorrect arg types for textcolor function");
		return false;
	}

	int intArgs[] = { std::stoi(args[1]), std::stoi(args[2]), std::stoi(args[3]) };
	for (int i = 0; i < 3; i++)
	{
		if (intArgs[i] < 0 || intArgs[i] > 255)
		{
			SetError("Incorrect arg values for textcolor function");
			return false;
		}
	}
	
	_textColor = sf::Color(intArgs[0], intArgs[1], intArgs[2]);
	return true;
}

bool ConsoleBase::ChangeConsoleColor(const std::vector<std::string>& args)
{
	std::vector<std::string> params = { "int", "int", "int" };
	if (!PlayerFunction::CheckArgs(args, params))
	{
		SetError("Incorrect arg types for consolecolor function");
		return false;
	}

	int intArgs[] = { std::stoi(args[1]), std::stoi(args[2]), std::stoi(args[3]) };
	for (int i = 0; i < 3; i++)
	{
		if (intArgs[i] < 0 || intArgs[i] > 255)
		{
			SetError("Incorrect arg values for consolecolor function");
			return false;
		}
	}

	_playerColor = sf::Color(intArgs[0], intArgs[1], intArgs[2]);
	return true;
}

bool ConsoleBase::StartGame(const std::vector<std::string>& args)
{
	if (args.size() != 1)
	{
		SetError("Incorrect arg count for startgame function");
		return false;
	}

	_controller->AddHUD("Mining");
	_controller->RemoveHUD("Menu");

	_controller->ReturnGameLogicPtr()->SetupGame();
	return true;
}

bool ConsoleBase::Help(const std::vector<std::string>& args)
{
	if (args.size() == 1)
	{
		AddQueue(_resourceManager->LoadText("Resources/Texts/help0.txt", 120));
		return true;
	}

	std::vector<std::string> params = { "int" };
	if (!PlayerFunction::CheckArgs(args, params))
	{
		SetError("Incorrect arg types for help function");
		return false;
	}

	int helpIdx = std::stoi(args[1]);

	if (helpIdx < 1 || helpIdx > 3)
	{
		SetError("Unknown help number");
		return false;
	}

	AddQueue(_resourceManager->LoadText("Resources/Texts/help" + std::to_string(helpIdx) + ".txt", 120));

}

bool ConsoleBase::Back(const std::vector<std::string>& args)
{
	if (args.size() != 1)
	{
		SetError("Incorrect arg count for back function");
		return false;
	}

	if (_queuedText.size() > 1)
	{
		_queuedText.pop();
		_curQueue = _queuedText.top();
	}

	return true;
}

bool ConsoleBase::Clear(const std::vector<std::string>& args)
{
	if (args.size() != 1)
	{
		SetError("Incorrect arg count for back function");
		return false;
	}

	while (_queuedText.size() > 1)
	{
		_queuedText.pop();
		
	}

	_curQueue = _queuedText.top();
	return true;
}

bool ConsoleBase::MoveShip(const std::vector<std::string>& args)
{
	std::vector<std::string> params = { "int", "int", "int" };
	if (!PlayerFunction::CheckArgs(args, params))
	{
		SetError("Incorrect arg types for moveship function");
		return false;
	}

	int intArgs[] = { std::stoi(args[1]), std::stoi(args[2]), std::stoi(args[3]) };
	std::vector<Spaceship>* ships = _gameLogic->ReturnShips();

	if (intArgs[0] < 1 || intArgs[0] > ships->size())
	{
		SetError("Unknown ship number");
		return false;
	}

	sf::Vector2i mapSize = _gameLogic->ReturnMapSize();
	if (intArgs[1] < 1 || intArgs[1] > mapSize.x - 1)
	{
		SetError("X value out of range");
		return false;
	}

	if (intArgs[2] < 1 || intArgs[2] > mapSize.y - 1)
	{
		SetError("Y value out of range");
		return false;
	}

	if ((*ships)[intArgs[0] - 1].ReturnIsNull())
	{
		SetError("That ship is null");
		return false;
	}

	(*ships)[intArgs[0] - 1].SetMoving(sf::Vector2f(intArgs[1], intArgs[2]));

	return true;
}

bool ConsoleBase::StartScan(const std::vector<std::string>& args)
{
	std::vector<std::string> params = { "int" };
	if (!PlayerFunction::CheckArgs(args, params))
	{
		SetError("Incorrect arg types for startscan function");
		return false;
	}

	int intArgs[] = { std::stoi(args[1]) };
	std::vector<Spaceship>* ships = _gameLogic->ReturnShips();

	if (intArgs[0] < 1 || intArgs[0] > ships->size())
	{
		SetError("Unknown ship number");
		return false;
	}

	if ((*ships)[intArgs[0] - 1].ReturnIsNull())
	{
		SetError("That ship is null");
		return false;
	}

	(*ships)[intArgs[0] - 1].SetScanning();

	return true;
}

bool ConsoleBase::StartMine(const std::vector<std::string>& args)
{
	std::vector<std::string> params = { "int" };
	if (!PlayerFunction::CheckArgs(args, params))
	{
		SetError("Incorrect arg types for startmine function");
		return false;
	}

	int intArgs[] = { std::stoi(args[1]) };
	std::vector<Spaceship>* ships = _gameLogic->ReturnShips();

	if (intArgs[0] < 1 || intArgs[0] > ships->size())
	{
		SetError("Unknown ship number");
		return false;
	}

	if ((*ships)[intArgs[0] - 1].ReturnIsNull())
	{
		SetError("That ship is null");
		return false;
	}

	(*ships)[intArgs[0] - 1].SetMining();

	return true;
}

bool ConsoleBase::ReturnShip(const std::vector<std::string>& args)
{
	std::vector<std::string> params = { "int" };
	if (!PlayerFunction::CheckArgs(args, params))
	{
		SetError("Incorrect arg types for returnship function");
		return false;
	}

	int intArgs[] = { std::stoi(args[1]) };
	std::vector<Spaceship>* ships = _gameLogic->ReturnShips();

	if (intArgs[0] < 1 || intArgs[0] > ships->size())
	{
		SetError("Unknown ship number");
		return false;
	}

	if ((*ships)[intArgs[0] - 1].ReturnIsNull())
	{
		SetError("That ship is null");
		return false;
	}

	(*ships)[intArgs[0] - 1].SetReturning();

	return true;
}

bool ConsoleBase::MapInfo(const std::vector<std::string>& args)
{
	if (args.size() != 1)
	{
		SetError("Incorrect arg count for mapinfo function");
		return false;
	}

	while (_queuedText.size() > 1)
	{
		_queuedText.pop();
	}

	_curQueue = _queuedText.top();

	Text* text = _resourceManager->LoadText("Resources/Texts/mapInfo.txt", -1);
	std::vector<TextPiece> pieces;
	std::vector<int*> intPtrs = _gameLogic->ReturnIntPtrs();

	pieces.push_back(TextPiece(intPtrs[4]));
	pieces.push_back(TextPiece(intPtrs[5]));

	std::vector<RockType>* rockTypes = _gameLogic->ReturnRockTypes();
	for (int i = 0; i < rockTypes->size(); i++)
	{
		pieces.push_back(TextPiece((*rockTypes)[i].ReturnNamePtr()));
		pieces.push_back(TextPiece((*rockTypes)[i].ReturnColorStringPtr()));
		pieces.push_back(TextPiece((*rockTypes)[i].ReturnVolumePtr()));
		pieces.push_back(TextPiece((*rockTypes)[i].ReturnValuePtr()));
	}

	text->SetPieces(pieces);
	AddQueue(text);

	return true;
}

bool ConsoleBase::ShipInfo(const std::vector<std::string>& args)
{
	std::vector<std::string> params = { "int" };
	if (!PlayerFunction::CheckArgs(args, params))
	{
		SetError("Incorrect arg types for shipinfo function");
		return false;
	}

	int intArgs[] = { std::stoi(args[1]) };
	std::vector<Spaceship>* ships = _gameLogic->ReturnShips();

	if (intArgs[0] < 1 || intArgs[0] > ships->size())
	{
		SetError("Unknown ship number");
		return false;
	}

	if ((*ships)[intArgs[0] - 1].ReturnIsNull())
	{
		SetError("That ship is null");
		return false;
	}

	while (_queuedText.size() > 1)
	{
		_queuedText.pop();
	}

	_curQueue = _queuedText.top();

	Text* text = _resourceManager->LoadText("Resources/Texts/shipInfo.txt", -1);
	std::vector<TextPiece> pieces;

	std::vector<float*> floatPtrs = (*ships)[intArgs[0] - 1].ReturnFloatPtrs();
	std::vector<int*> intPtrs = (*ships)[intArgs[0] - 1].ReturnIntPtrs();

	// Mining speed and radius
	pieces.push_back(TextPiece(floatPtrs[0]));
	pieces.push_back(TextPiece(floatPtrs[2]));

	// Scanning speed and radius
	pieces.push_back(TextPiece(floatPtrs[1]));
	pieces.push_back(TextPiece(floatPtrs[3]));

	// Reveal radius, move speed, carry capacity
	pieces.push_back(TextPiece(floatPtrs[4]));
	pieces.push_back(TextPiece(floatPtrs[5]));
	pieces.push_back(TextPiece(intPtrs[0]));

	// Status
	pieces.push_back(TextPiece((*ships)[intArgs[0] - 1].ReturnStatusPtr()));

	// Location X, Y
	pieces.push_back(TextPiece(floatPtrs[6]));
	pieces.push_back(TextPiece(floatPtrs[7]));

	// Current cargo hold and value
	pieces.push_back(TextPiece(intPtrs[1]));
	pieces.push_back(TextPiece(intPtrs[0]));
	pieces.push_back(TextPiece(intPtrs[2]));

	text->SetPieces(pieces);
	AddQueue(text);

	return true;
}

