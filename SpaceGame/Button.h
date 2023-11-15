/*
#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Button
{
private:
	
	sf::RenderWindow *_window;

	sf::Vector2f _startLoc, _endLoc;

	sf::Drawable* _drawable;

	std::function<void()> _func;

public:

	Button(const sf::Vector2f startLoc, const sf::Vector2f size, 
		const sf::Color& color, std::function<void()> func, sf::RenderWindow* const& window);

	Button(const sf::Vector2f startLoc, const sf::Vector2f size, 
		sf::Texture* const& texture, std::function<void()> func, sf::RenderWindow* const& window);

	~Button();

	void DrawButton();

	bool CheckClicked(const sf::Vector2f& clickLoc);

	void CallFunc();

};
*/