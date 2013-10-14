#pragma once

#include <UI/UserInterface.h>
#include <Core/Input/InputHandlers.h>
#include <Core/Input/InputManager.h>
#include "BuildingComponentData.h"
#include "EconomyMenu.h"

class VillageGame;

using namespace Engine;

class GameMenu :
	public IMouseHandler
{
public:
	GameMenu( VillageGame* pGame );

	~GameMenu();
    
    void Update( float fDeltaSeconds );

	virtual bool VOnMouseMove( const Vector3& vPosition, const Vector3& vDeltaPosition );

	virtual bool VOnMouseButtonDown( const int iButtonIndex, const Vector3& vPosition );

	virtual bool VOnMouseButtonUp( const int iButtonIndex, const Vector3& vPosition );

	virtual bool VOnMouseButtonDClick( const int iButtonIndex, const Vector3& vPosition );

	virtual bool VOnMouseWheel( const Vector3& vPosition, const Vector3& vDelta );

private: 
	bool m_bActive;

	VillageGame* m_pGame;
	EconomyMenu* m_pEconomyMenu;

    BuildingComponentData* m_pCurrentBuilding;
    std::vector<BuildingComponentData*> m_pBuildingData;
	UIImage* m_pPlacementImage;

	void MouseToCameraSpace( const Vector3& vMousePosition, Vector3& vOut );
	void MouseToWorldSpace( const Vector3& vMousePosition, Vector3& vOut );

};

