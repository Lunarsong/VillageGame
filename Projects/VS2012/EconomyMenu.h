#pragma once

#include <UI/UserInterface.h>

class VillageGame;
using namespace Engine;
class EconomyMenu
{
public:
	EconomyMenu( VillageGame* pGame );
	~EconomyMenu(void);

	void Show();
	void Hide();
	void ToggleVisibility();

private:
	UIElement* m_pMenu;
	VillageGame* m_pGame;
};

