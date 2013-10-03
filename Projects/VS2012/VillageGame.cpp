#include "VillageGame.h"
#include <Game/Game.h>
#include <UI/UserInterface.h>
#include <Core/AssetManager/AssetBundle.h>
#include <Core/AssetManager/AssetManager.h>

VillageGame::VillageGame(void)
{
	
	//AssetBundle* pBundle = new AssetBundle();
	//pBundle->LoadFromXML( XmlResourceLoader::LoadAndReturnRootXmlElement( "Sprites.xml" )) ;
	//pBundle->Release();
}


VillageGame::~VillageGame(void)
{
	m_pCamera->Release();
	delete m_pGameWorld;

	delete m_pGameMenu;
	delete AssetManager::GetPtr();
}

void VillageGame::VOnInit( void )
{
	m_pGameMenu = new GameMenu( this );
	m_pGameWorld = new GameWorld();	

	CreateCamera();

	
}

void VillageGame::VOnUpdate( const float fDeltaSeconds )
{

}
void VillageGame::CreateCamera()
{
	m_pCamera = new ScrollCamera();

	Entity* pEntity = Game::CreateEntity();
	pEntity->AddComponent( m_pCamera );	
	pEntity->Start();
}

ScrollCamera* VillageGame::GetCamera()
{
	return m_pCamera;
}
