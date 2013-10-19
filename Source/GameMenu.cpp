#include "GameMenu.h"
#include <Game/Game.h>
#include "VillageGame.h"
#include <Game/Entities/Components/Rendering/QuadComponent.h>
#include <Game/Dialogue/UI/DialogueInterface.h>
#include <Core/Application/BaseApplication.h>
#include "BuildingComponent.h"

GameMenu::GameMenu( VillageGame* pGame )
{
	m_pGame = pGame;

	m_bActive = false;

	/*Process* pProcess = new DialogueInterface( NULL, "DialogueInterface.xml", "Dialogue.xml" );
	BaseApplication::Get()->AttachProcess( pProcess );
	pProcess->Release();*/

	UIElement* pGameMenu = UserInterface::AddScreenFromFile( "GameMenu", "GameMenu.xml" );
	pGameMenu->GetElement<UIButtonImage>( "btn_build" )->SetCallbackFunction( [this] ( UIElement* pElement, void* pArgs )
		{
			UIElement* pBuildMenu = UserInterface::GetScreen( "BuildMenu" );
			pBuildMenu->SetVisible( !pBuildMenu->IsVisible() );
			
			if ( m_bActive )
			{
				m_bActive = false;
				m_pPlacementImage->SetVisible( m_bActive );
			}
		}
	);

	pGameMenu->GetElement<UIButtonImage>( "btn_settings" )->SetCallbackFunction( [this] ( UIElement* pElement, void* pArgs )
	{
		m_pEconomyMenu->ToggleVisibility();
	}
	);

	
    
	UIElement* pBuildMenu = UserInterface::AddScreenFromFile( "BuildMenu", "BuildMenu.xml" );
    
	XmlResource* pResource = AssetManager::Get().GetAsset<XmlResource>( "BuildingDefinitions.xml" );
	if ( pResource )
	{
        UIElement* pListElement = pBuildMenu->GetElement( "building_list" );
        
        tinyxml2::XMLElement* pElement = pResource->GetRoot()->FirstChildElement();
        int iCount = 0;
        while ( pElement )
        {
            BuildingComponentData* pData = new BuildingComponentData();
            pData->VFromXML( pElement );
            m_pBuildingData.push_back( pData );
            
            /*
             <ButtonImage>
             <ID>0</ID>
             <Size x="70" y="70" />
             <Position x="5" y="40"/>
             <Texture>HumanFarm.png</Texture>
             <Alignment>Left</Alignment>
             </ButtonImage>
             */
            
            UIButtonImage* pButton = new UIButtonImage();
            pButton->SetTexture( pData->BuildMenuIcon );
            pButton->SetAlignment( Left );
            pButton->SetPosition( 5.0f + (float)iCount * 70.0f , 40.0f );
            pButton->SetSize( 64.0f, 64.0f );
            pButton->SetCallbackArgs( (void*)iCount );
            pButton->SetCallbackFunction( [this] ( UIElement* pElement, void* pArgs )
                                         {
                                             if ( m_bActive && m_pCurrentBuilding == m_pBuildingData[ (int)pArgs ] )
                                             {
                                                 m_bActive = false;
                                                 m_pPlacementImage->SetVisible( m_bActive );
                                             }
                                             
                                             else
                                             {
                                                 m_pCurrentBuilding = m_pBuildingData[ (int)pArgs ];
                                                 m_bActive = true;
                                                 m_pPlacementImage->SetVisible( m_bActive );
                                                 m_pPlacementImage->SetTexture( m_pCurrentBuilding->Icon );
                                                 m_pPlacementImage->SetPosition( InputManager::Get()->GetMousePos() );
                                             }
                                             
                                         }
                                         );
            
            pListElement->AddChild( pButton );
            pButton->Release();
            
            ++iCount;
            pElement = pElement->NextSiblingElement();
        }
	}

    

    
	pBuildMenu->SetVisible( false );

	UIElement* pElement = new UIElement();

	m_pPlacementImage = new UIImage( "HumanFarm.png" );
	m_pPlacementImage->SetAlignment( Center );
	m_pPlacementImage->SetColor( ColorF( 1.0f, 1.0f, 1.0f, 0.7f ) );
	pElement->AddChild( m_pPlacementImage );

	m_pPlacementImage->SetVisible( false );

	UserInterface::AddScreen( "GameHUD", pElement );
	pElement->Release();

	InputManager::Get()->AddMouseHandler( this );

	m_pEconomyMenu = new EconomyMenu( m_pGame );
	m_pEconomyMenu->Hide();
}

GameMenu::~GameMenu()
{
	delete m_pEconomyMenu;
	InputManager::Get()->RemoveMouseHandler( this );

	m_pPlacementImage->Release();

	UserInterface::RemoveScreen( "GameMenu" );
	UserInterface::RemoveScreen( "BuildMenu" );
	UserInterface::RemoveScreen( "GameHUD" );

    for ( auto it : m_pBuildingData )
    {
        it->Release();
    }
}

bool GameMenu::VOnMouseMove( const Vector3& vMousePosition, const Vector3& vDeltaPosition )
{
	if ( m_bActive )
	{
		Vector3 vPosition;
		MouseToWorldSpace( vMousePosition, vPosition );
		Vector3 vAlignedPosition( vPosition.x - (int) vPosition.x % 32, vPosition.y - (int) vPosition.y % 32 );

		Matrix matView = m_pGame->GetCamera()->GetView();
		vPosition = matView.Transform( vAlignedPosition );

		IRenderer* pRenderer = IRenderer::Get();
		Vector3 vResolution( (float)pRenderer->VGetScreenWidth(), (float)pRenderer->VGetScreenHeight() );

		vPosition.y = 1.0f - vPosition.y;
		m_pPlacementImage->SetPosition( vPosition + vResolution * 0.5f );
	}

	return false;
}

bool GameMenu::VOnMouseButtonDown( const int iButtonIndex, const Vector3& vPosition )
{
	return false;
}

bool GameMenu::VOnMouseButtonUp( const int iButtonIndex, const Vector3& vMousePosition )
{
	if ( !m_bActive )
	{
		return false;
	}

	if ( iButtonIndex == 0 )
	{
		Matrix matTransform;

		Vector4 vSize = m_pPlacementImage->GetSize().GetPixels();
		matTransform.BuildScale( vSize );
		Vector3 vPosition;
		MouseToWorldSpace( vMousePosition, vPosition );
		Vector3 vAlignedPosition( vPosition.x - (int) vPosition.x % 32, vPosition.y - (int) vPosition.y % 32 );
		matTransform.SetPosition( vAlignedPosition );

		BuildingComponent* pBuildingComponent = new BuildingComponent( m_pCurrentBuilding );

		Entity* pEntity = Game::CreateEntity( matTransform );
		pEntity->AddComponent( pBuildingComponent );
		pBuildingComponent->Start();
		pBuildingComponent->Release();
        
        m_bActive = false;
        m_pPlacementImage->SetVisible( m_bActive );
	}

	return false;
}

bool GameMenu::VOnMouseButtonDClick( const int iButtonIndex, const Vector3& vPosition )
{
	return false;
}

bool GameMenu::VOnMouseWheel( const Vector3& vPosition, const Vector3& vDelta )
{
	return false;
}

void GameMenu::MouseToCameraSpace( const Vector3& vMousePosition, Vector3& vOut )
{
	IRenderer* pRenderer = IRenderer::Get();

	Matrix matProj = m_pGame->GetCamera()->GetProjection();
	
	float fScreenWidth = (float)pRenderer->VGetScreenWidth();
	float fScreenHeight = (float)pRenderer->VGetScreenHeight();

	vOut.x =  ( ( ( 2.0f * vMousePosition.x ) / fScreenWidth  ) - 1.0f ) / matProj._11;
	vOut.y = -( ( ( 2.0f * vMousePosition.y ) / fScreenHeight ) - 1.0f ) / matProj._22;
	vOut.z =  1.0f;
}

void GameMenu::MouseToWorldSpace( const Vector3& vMousePosition, Vector3& vOut )
{
	MouseToCameraSpace( vMousePosition, vOut );

	Matrix matViewInverse = m_pGame->GetCamera()->GetView().Inverse();
	vOut = matViewInverse.Transform( vOut );
}

void GameMenu::Update( float fDeltaSeconds )
{
    m_pEconomyMenu->Update( fDeltaSeconds );
}