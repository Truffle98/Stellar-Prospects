#include "ResourceManager.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	std::cout << "Starting resource manager cleanup" << std::endl;
	for (auto& texture : _textures)
	{
		delete texture.second;
	}

	for (auto& font : _fonts)
	{
		delete font.second;
	}

	for (auto& text : _texts)
	{
		delete text.second;
	}
	std::cout << "Completed resource manager cleanup" << std::endl;
}

sf::Texture* ResourceManager::LoadTexture(std::string path)
{
	auto it = _textures.find(path);
	if (it != _textures.end()) 
	{
		return _textures[path];
	}

	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile(path))
	{
		std::cout << "Failed to load: " << path << std::endl;
		exit(-1);
	}

	_textures[path] = texture;
	return texture;
}

sf::Font* ResourceManager::LoadFont(std::string path)
{
	auto it = _fonts.find(path);
	if (it != _fonts.end())
	{
		return _fonts[path];
	}

	sf::Font* font = new sf::Font();
	if (!font->loadFromFile(path))
	{
		std::cout << "Failed to load: " << path << std::endl;
		exit(-1);
	}

	_fonts[path] = font;
	return font;
}

Text* ResourceManager::LoadText(std::string path, int printSpeed)
{
	auto it = _texts.find(path);
	if (it != _texts.end())
	{
		return _texts[path];
	}

	std::ifstream file(path);
	if (!file) {
		std::cout << "Failed to load: " << path << std::endl;
		exit(-1);
	}
	
	std::string str = "";
	std::string line;
	while (std::getline(file, line)) 
	{
		str += line + '\n';
	}

	str = str.substr(0, str.length() - 1);

	Text* text = new Text(str, printSpeed);
	_texts[path] = text;
	return text;
}