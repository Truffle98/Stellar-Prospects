#include "EndscreenHUD.h"
#include "SpaceGameController.h"

EndscreenHUD::EndscreenHUD(std::string name, SpaceGameController* controller) : BaseHUD(name, controller)
{
    EndscreenHUDSetup();
}

void EndscreenHUD::EndscreenHUDSetup()
{
    sf::Vector2f resolution = _controller->ReturnResolution();

    _clearColor = sf::Color();
    _console = new ConsoleBase(sf::Vector2u(60, 15), sf::Vector2f(0, 0), resolution, _controller);

    _console->AddPlayerFunction(PlayerFunction("exitgame", &ConsoleBase::ExitGame));
    _console->AddPlayerFunction(PlayerFunction("textcolor", &ConsoleBase::ChangeTextColor));
    _console->AddPlayerFunction(PlayerFunction("consolecolor", &ConsoleBase::ChangeConsoleColor));
    _console->AddPlayerFunction(PlayerFunction("startgame", &ConsoleBase::StartGame));
    _console->AddPlayerFunction(PlayerFunction("help", &ConsoleBase::Help));
    _console->AddPlayerFunction(PlayerFunction("back", &ConsoleBase::Back));
    _console->AddPlayerFunction(PlayerFunction("clear", &ConsoleBase::Clear));

    Text* text = _resourceManager->LoadText("Resources/Texts/endGame.txt", -1);

    std::vector<TextPiece> pieces;
    std::vector<int*> intPtrs = _controller->ReturnGameLogicPtr()->ReturnIntPtrs();

    pieces.push_back(TextPiece(intPtrs[2]));
    pieces.push_back(TextPiece(intPtrs[3]));

    text->SetPieces(pieces);
    _console->AddQueue(text);
}

void EndscreenHUD::DrawHUD()
{
    _console->DisplayConsole();

    DrawFPS();
}

bool EndscreenHUD::RunFrame()
{
    _console->ManageText();
    return HandleEvents();
}

bool EndscreenHUD::HandleEvents()
{
    sf::Event event;
    while (_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            _window->close();
        }

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

