#pragma once

#include "Core/Process/Process.h"
#include "GameWorld.h"
#include "ScrollCamera.h"
#include "GameMenu.h"
#include "Economy.h"

using namespace Engine;

class VillageGame : public Process
{
public:
	VillageGame(void);
	~VillageGame(void);

	virtual void VOnInit(void);
	virtual void VOnUpdate( const float fDeltaSeconds );

	ScrollCamera*	GetCamera();
	Economy*		GetEconomy();

private:
	GameWorld*		m_pGameWorld;
	Economy*		m_pEconomy;

	ScrollCamera*	m_pCamera;

	GameMenu*		m_pGameMenu;

	void CreateCamera();
};

