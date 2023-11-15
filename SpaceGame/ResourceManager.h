#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
#include <fstream>

#include "Text.h"
#include "TextPiece.h"

class ResourceManager
{
private:

	std::map<std::string, sf::Texture*> _textures;
	// std::map<std::string, sf::Sprite*> _sprites;
	std::map<std::string, sf::Font*> _fonts;
	std::map<std::string, Text*> _texts;

public:

	ResourceManager();

	~ResourceManager();

	sf::Texture* LoadTexture(std::string path);

	sf::Font* LoadFont(std::string path);

	Text* LoadText(std::string path, int printSpeed);
};

