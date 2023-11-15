#pragma once
#include "BaseHUD.h"
#include "ConsoleBase.h"

class MenuHUD : public BaseHUD
{
private:

	ConsoleBase* _console;

public:
	MenuHUD(std::string name, SpaceGameController* controller);

	~MenuHUD();

	void SetupMenu();

	void DrawHUD() override;

	bool RunFrame() override;

	bool HandleEvents() override;
};

