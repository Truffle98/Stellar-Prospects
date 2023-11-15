#include "MenuHUD.h"
#include "SpaceGameController.h"

// MenuHUD specific setup
MenuHUD::MenuHUD(std::string name, SpaceGameController* controller) : BaseHUD(name, controller)
{
    SetupMenu();
}

MenuHUD::~MenuHUD()
{
    delete _console;
}

// Sets up all text, sprites, and buttons for the menu, can be called whenever resolution changes
void MenuHUD::SetupMenu()
{
    sf::Vector2f resolution = _controller->ReturnResolution();

    _clearColor = sf::Color();
    _console = new ConsoleBase(sf::Vector2u(60, 15), sf::Vector2f(0,0), resolution, _controller);

    _console->AddPlayerFunction(PlayerFunction("exitgame", &ConsoleBase::ExitGame));
    _console->AddPlayerFunction(PlayerFunction("textcolor", &ConsoleBase::ChangeTextColor));
    _console->AddPlayerFunction(PlayerFunction("consolecolor", &ConsoleBase::ChangeConsoleColor));
    _console->AddPlayerFunction(PlayerFunction("startgame", &ConsoleBase::StartGame));
    _console->AddPlayerFunction(PlayerFunction("help", &ConsoleBase::Help));
    _console->AddPlayerFunction(PlayerFunction("back", &ConsoleBase::Back));
    _console->AddPlayerFunction(PlayerFunction("clear", &ConsoleBase::Clear));

    Text* text = _resourceManager->LoadText("Resources/Texts/gameStart.txt", 60);
    _console->AddQueue(text);
}

void MenuHUD::DrawHUD()
{
    _console->DisplayConsole();

    DrawFPS();
}

bool MenuHUD::RunFrame()
{
    _console->ManageText();
    return HandleEvents();
}

bool MenuHUD::HandleEvents()
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
                (event.text.unicode >= '0' && event.text.unicode <= '9')  || event.text.unicode == ' ' || event.text.unicode == '"')
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

