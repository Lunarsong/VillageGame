#include "VillageGame.h"
#include <Game/Game.h>
#include <UI/UserInterface.h>
#include <Core/AssetManager/XmlAssetBundle.h>
#include <Core/AssetManager/AssetManager.h>

VillageGame::VillageGame(void)
{
	
	XmlAssetBundle* pBundle = new XmlAssetBundle();
    XmlResource* pResource = AssetManager::Get().GetAsset< XmlResource >( "Sprites.xml" );
    pBundle->LoadFromXML( pResource->GetRoot() );
	delete pBundle;
}


VillageGame::~VillageGame(void)
{
	m_pCamera->Release();
	delete m_pGameWorld;

	delete m_pGameMenu;

	delete m_pEconomy;
    
	for ( auto it : m_pPlayers )
	{
		delete it.second;
	}
}

void VillageGame::VOnInit( void )
{
    m_pPlayers[ HashedString( "Player" ) ] = new Player();

	m_pEconomy = new Economy();
	m_pGameWorld = new GameWorld();	
	m_pGameMenu = new GameMenu( this );
	

	CreateCamera();
	
}

void VillageGame::VOnUpdate( const float fDeltaSeconds )
{
    m_pGameMenu->Update( fDeltaSeconds );
    
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

Economy* VillageGame::GetEconomy()
{
	return m_pEconomy;
}

Player* VillageGame::GetPlayer( const HashedString& hName )
{
	auto it = m_pPlayers.find( hName );

	if ( it == m_pPlayers.end() )
		return NULL;

	return it->second;
}
