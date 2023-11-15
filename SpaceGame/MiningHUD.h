#pragma once
#include "BaseHUD.h"
#include "ConsoleBase.h"

class MiningHUD : public BaseHUD
{

private:

	sf::Vector2f _topLeft;
	sf::Vector2f _bottomRight;

	ConsoleBase* _console;

public:
	MiningHUD(std::string name, SpaceGameController* controller);

	~MiningHUD();

	void MiningHUDSetup();

	void DrawHUD() override;

	bool RunFrame() override;

	bool HandleEvents() override;

	void SetBoundaries(sf::Vector2f topLeft, sf::Vector2f bottomRight);

};

