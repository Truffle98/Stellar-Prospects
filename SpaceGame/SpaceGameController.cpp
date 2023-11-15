#include "SpaceGameController.h"

// Runs all initial setup
// Starts by creating window then creates all HUDS
SpaceGameController::SpaceGameController(const sf::Vector2f& resolution, const bool& fullscreen)
{
    srand(time(0));

    sf::Uint32 style = sf::Style::Close;
    if (fullscreen)
    {
        style = sf::Style::Fullscreen;
    }

    _window.create(sf::VideoMode(resolution.x, resolution.y), "Stellar Prospects", style);
    _window.setFramerateLimit(60);
    _window.setSize(sf::Vector2u(resolution.x, resolution.y));
    
    _resolution = sf::Vector2f(_window.getSize());

    _gameLogic = GameLogic(this);

    sf::Vector2f topLeft(0, 0);
    sf::Vector2f bottomRight(_window.getSize().x * 0.75f, _window.getSize().y);
    _gameLogic.SetAsteroidFieldResolution(topLeft, bottomRight);

    _gameLogic.CreateSpaceships();

    SetupHUDS();
}

// Deletes all HUDS
SpaceGameController::~SpaceGameController()
{
    for (BaseHUD* hud : _huds)
    {
        delete hud;
    }
}

// Runs all HUD setup
void SpaceGameController::SetupHUDS()
{
    
    MenuHUD* menuHUD = new MenuHUD("Menu", this);
    _huds.push_back(menuHUD);

    _currentHUDs.push_back(menuHUD);

    MiningHUD* miningHUD = new MiningHUD("Mining", this);
    _huds.push_back(miningHUD);

    EndscreenHUD* endscreenHUD = new EndscreenHUD("Endscreen", this);
    _huds.push_back(endscreenHUD);
}

// Called to begin running window and game
void SpaceGameController::StartGame()
{
    std::list<BaseHUD*>::iterator iter;
    std::list<BaseHUD*>::reverse_iterator rIter;
    BaseHUD* curHUD;
    bool shouldBreak;

    while (_window.isOpen()) 
    {
        iter = _currentHUDs.begin();

        curHUD = *iter;
        _window.clear(curHUD->ReturnClearColor());

        for (; iter != _currentHUDs.end(); ++iter)
        {
            curHUD = *iter;
            curHUD->DrawHUD();
            
        }

        shouldBreak = true;
        for (rIter = _currentHUDs.rbegin(); rIter != _currentHUDs.rend(); ++rIter)
        {
            curHUD = *rIter;
            shouldBreak = curHUD->RunFrame();

            if (shouldBreak)
            {
                break;
            }
        }

        _window.display();
    }

}

// Switches HUD according to it's name
// Will probably change this to have multiple huds up at once
void SpaceGameController::AddHUD(const std::string& HUDName)
{
    for (BaseHUD* hud : _huds)
    {
        if (HUDName == hud->ReturnHUDName())
        {
            _currentHUDs.push_back(hud);
            return;
        }
    }
}

void SpaceGameController::RemoveHUD(const std::string& HUDName)
{
    std::list<BaseHUD*>::iterator iter;
    BaseHUD* hud;

    for (std::list<BaseHUD*>::iterator iter = _currentHUDs.begin(); iter != _currentHUDs.end(); ++iter)
    {
        hud = *iter;
        if (HUDName == hud->ReturnHUDName())
        {
            _currentHUDs.erase(iter);
            break;
        }
    }
}

sf::RenderWindow* SpaceGameController::ReturnWindowPtr()
{
    return &_window;
}

GameLogic* SpaceGameController::ReturnGameLogicPtr()
{
    return &_gameLogic;
}

ResourceManager* SpaceGameController::ReturnResourceManagerPtr()
{
    return &_resourceManager;
}

sf::Vector2f SpaceGameController::ReturnResolution()
{
    return _resolution;
}

sf::Time SpaceGameController::ReturnTimeSinceLastFrame()
{
    return _clock.restart();
}