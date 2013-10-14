#pragma once

#include "Core/Process/Process.h"
#include "GameWorld.h"
#include "ScrollCamera.h"
#include "GameMenu.h"
#include "Economy.h"
#include "ResourceStorage.h"
#include <Core/Utils/Singleton.h>
#include "Player.h"

using namespace Engine;

class VillageGame : public Process, public Singleton<VillageGame>
{
public:
	VillageGame(void);
	~VillageGame(void);

	virtual void VOnInit(void);
	virtual void VOnUpdate( const float fDeltaSeconds );

	ScrollCamera*	GetCamera();
	Economy*		GetEconomy();
    
    Player* GetPlayer( const HashedString& hName );

private:
	GameWorld*		m_pGameWorld;
	Economy*		m_pEconomy;
    
	std::map< HashedString, Player* > m_pPlayers;

	ScrollCamera*	m_pCamera;

	GameMenu*		m_pGameMenu;

	void CreateCamera();
};

