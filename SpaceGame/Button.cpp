/*
#include "Button.h"

//Buttons have a function and bounds so during the game loop a HUD can check whether a button has been clicked, 
//		and then run the appropriate function


// Creates a button with a filled rectangle
Button::Button(const sf::Vector2f startLoc, const sf::Vector2f size,
	const sf::Color& color, std::function<void()> func, sf::RenderWindow* const& window)
{
	_window = window;
	_func = func;

	_startLoc = startLoc;
	_endLoc = startLoc + size;

	sf::RectangleShape* rect = new sf::RectangleShape();
	rect->setPosition(startLoc);
	rect->setSize(size);
	rect->setFillColor(color);

	_drawable = rect;
}

Button::Button(const sf::Vector2f startLoc, const sf::Vector2f size,
	sf::Texture* const& texture, std::function<void()> func, sf::RenderWindow* const& window)
{
	_window = window;
	_func = func;

	_startLoc = startLoc;
	_endLoc = startLoc + size;
	
	sf::Vector2f playButtonPixels(texture->getSize());
	sf::Vector2f playScaleFactor(size.x / playButtonPixels.x, size.y / playButtonPixels.y);

	sf::Sprite* sprite = new sf::Sprite();
	sprite->setTexture(*texture);
	sprite->setPosition(startLoc);
	sprite->setScale(playScaleFactor);

	_drawable = sprite;
}

Button::~Button()
{
	delete _drawable;
}

void Button::DrawButton()
{
	_window->draw(*_drawable);
}

bool Button::CheckClicked(const sf::Vector2f& clickLoc)
{
	if (_startLoc.x <= clickLoc.x && clickLoc.x <= _endLoc.x)
	{
		if (_startLoc.y <= clickLoc.y && clickLoc.y <= _endLoc.y)
		{
			return true;
		}
	}
	return false;
}

void Button::CallFunc()
{
	_func();
}
*/