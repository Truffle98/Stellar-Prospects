#include <iostream>

#include "SpaceGameController.h"

int WinMain() 
{
    const sf::Vector2f resolution(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
    
    SpaceGameController controller(resolution, true);

    controller.StartGame();

    return 0;
}