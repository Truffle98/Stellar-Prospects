#pragma once
#include "BaseHUD.h"
#include "ConsoleBase.h"

class EndscreenHUD : public BaseHUD
{
private:

	ConsoleBase* _console;

public:

	EndscreenHUD(std::string name, SpaceGameController* controller);

	void EndscreenHUDSetup();

	void DrawHUD() override;

	bool RunFrame() override;

	bool HandleEvents() override;

};

