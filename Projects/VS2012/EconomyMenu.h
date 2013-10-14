#pragma once

#include <UI/UserInterface.h>
#include "Economy.h"

class VillageGame;
using namespace Engine;
class EconomyMenu
{
public:
	EconomyMenu( VillageGame* pGame );
	~EconomyMenu(void);
    
    void Update( float fDeltaSeconds );

	void Show();
	void Hide();
	void ToggleVisibility();

private:
	UIElement* m_pMenu;
	VillageGame* m_pGame;
    
    struct ResourceUI
    {
        const ResourceComponentData* m_pResource;
        UILabel*    m_pLabel;
    };
    
    std::list<ResourceUI> m_pResourceUIComponents;
};

