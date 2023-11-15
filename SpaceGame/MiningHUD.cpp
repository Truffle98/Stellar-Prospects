#include "MiningHUD.h"
#include "SpaceGameController.h"

// MiningHUD specific setup
MiningHUD::MiningHUD(std::string name, SpaceGameController* controller) : BaseHUD(name, controller)
{
    // _clearColor = sf::Color(0, 0, 0);
	MiningHUDSetup();


}

MiningHUD::~MiningHUD()
{
	delete _console;
}

void MiningHUD::MiningHUDSetup()
{
	sf::Vector2f topLeft(_window->getSize().x * 0.75f, 0);
	sf::Vector2f bottomRight(_window->getSize().x, _window->getSize().y);

	//sf::Vector2f topLeft(0, 0);
	//sf::Vector2f bottomRight(_window->getSize().x, _window->getSize().y);

	_console = new ConsoleBase(sf::Vector2u(30, 30), topLeft, bottomRight, _controller);

	_console->AddPlayerFunction(PlayerFunction("exitgame", &ConsoleBase::ExitGame));
	_console->AddPlayerFunction(PlayerFunction("help", &ConsoleBase::Help));
	_console->AddPlayerFunction(PlayerFunction("back", &ConsoleBase::Back));
	_console->AddPlayerFunction(PlayerFunction("clear", &ConsoleBase::Clear));
	_console->AddPlayerFunction(PlayerFunction("moveship", &ConsoleBase::MoveShip));
	_console->AddPlayerFunction(PlayerFunction("startscan", &ConsoleBase::StartScan));
	_console->AddPlayerFunction(PlayerFunction("startmine", &ConsoleBase::StartMine));
	_console->AddPlayerFunction(PlayerFunction("returnship", &ConsoleBase::ReturnShip));
	_console->AddPlayerFunction(PlayerFunction("mapinfo", &ConsoleBase::MapInfo));
	_console->AddPlayerFunction(PlayerFunction("shipinfo", &ConsoleBase::ShipInfo));

	Text* infoText = _resourceManager->LoadText("Resources/Texts/basicInfo.txt", -1);
	
	std::vector<Spaceship>* ships = _gameLogic->ReturnShips();
	std::vector<TextPiece> pieces;
	
	std::vector<int*> ptrs = _gameLogic->ReturnIntPtrs();
	pieces.push_back(TextPiece(ptrs[0]));
	pieces.push_back(TextPiece(ptrs[1]));

	for (int i = 0; i < 4; i++)
	{
		pieces.push_back(TextPiece((*ships)[i].ReturnStatusPtr()));
	}

	pieces.push_back(TextPiece(ptrs[2]));
	pieces.push_back(TextPiece(ptrs[3]));

	infoText->SetPieces(pieces);
	_console->AddQueue(infoText);

}

void MiningHUD::DrawHUD()
{
	_gameLogic->DrawAsteroidField();
	_gameLogic->DrawSpaceships();

	_console->DisplayConsole();	

    DrawFPS();
}

bool MiningHUD::RunFrame()
{
    _gameLogic->RunFrame();

	_console->ManageText();

	return HandleEvents();
}

bool MiningHUD::HandleEvents()
{
	sf::Event event;
	while (_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			_window->close();
		}

		/*
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			sf::Vector2f clickLoc(event.mouseButton.x, event.mouseButton.y);
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f clickTileLoc = _gameLogic->ConvertPixelLocToGamePos(clickLoc);
				_gameLogic->RevealAsteroidTiles(clickTileLoc, 10);
			}

			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				sf::Vector2f clickTileLoc = _gameLogic->ConvertPixelLocToGamePos(clickLoc);
				_gameLogic->ScanAsteroidTiles(clickTileLoc, 10);
			}
		}
		*/

		else if (event.type == sf::Event::TextEntered)
		{

			if ((event.text.unicode >= 'A' && event.text.unicode <= 'Z') || (event.text.unicode >= 'a' && event.text.unicode <= 'z') ||
				(event.text.unicode >= '0' && event.text.unicode <= '9') || event.text.unicode == ' ' || event.text.unicode == '"')
			{
				_console->HandleInput(static_cast<char>(event.text.unicode));
			}

			else if (event.text.unicode == 8)
			{
				_console->HandleInput(false);
			}

			else if (event.text.unicode == 127)
			{
				_console->HandleInput(true);
			}

			else if (event.text.unicode == 13)
			{
				_console->ReadConsoleInput();
			}
		}
	}
	return false;
}

void MiningHUD::SetBoundaries(sf::Vector2f topLeft, sf::Vector2f bottomRight)
{
	_topLeft = topLeft;
	_bottomRight = bottomRight;
}