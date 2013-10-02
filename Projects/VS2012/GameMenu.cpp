#include "GameMenu.h"
#include <Game/Game.h>
#include "VillageGame.h"
#include <Game/Entities/Components/Rendering/QuadComponent.h>

GameMenu::GameMenu( VillageGame* pGame )
{
	m_pGame = pGame;

	UserInterface::AddScreenFromFile( "GameMenu", "GameMenu.xml" );
	UserInterface::AddScreenFromFile( "BuildMenu", "BuildMenu.xml" );

	UIElement* pElement = new UIElement();

	m_pPlacementImage = new UIImage( "HumanFarm.png" );
	m_pPlacementImage->SetAlignment( Center );
	m_pPlacementImage->SetColor( ColorF( 1.0f, 1.0f, 1.0f, 0.7f ) );
	pElement->AddChild( m_pPlacementImage );

	UserInterface::AddScreen( "GameHUD", pElement );
	pElement->Release();

	InputManager::Get()->AddMouseHandler( this );
}

GameMenu::~GameMenu()
{
	InputManager::Get()->RemoveMouseHandler( this );

	m_pPlacementImage->Release();

	UserInterface::RemoveScreen( "GameMenu" );
	UserInterface::RemoveScreen( "BuildMenu" );
	UserInterface::RemoveScreen( "GameHUD" );
}

bool GameMenu::VOnMouseMove( const Vector3& vMousePosition, const Vector3& vDeltaPosition )
{
	Vector3 vPosition;
	MouseToWorldSpace( vMousePosition, vPosition );
	Vector3 vAlignedPosition( vPosition.x - (int) vPosition.x % 32, vPosition.y - (int) vPosition.y % 32 );

	Matrix matView = m_pGame->GetCamera()->GetView();
	vPosition = matView.Transform( vAlignedPosition );

	IRenderer* pRenderer = IRenderer::Get();
	Vector3 vResolution( pRenderer->VGetScreenWidth(), pRenderer->VGetScreenHeight() );

	vPosition.y = 1.0f - vPosition.y;
	m_pPlacementImage->SetPosition( vPosition + vResolution * 0.5f );

	return false;
}

bool GameMenu::VOnMouseButtonDown( const int iButtonIndex, const Vector3& vPosition )
{
	return false;
}

bool GameMenu::VOnMouseButtonUp( const int iButtonIndex, const Vector3& vMousePosition )
{
	if ( iButtonIndex == 0 )
	{
		Matrix matTransform;

		Vector4 vSize = m_pPlacementImage->GetSize().GetPixels();
		matTransform.BuildScale( vSize );
		Vector3 vPosition;
		MouseToWorldSpace( vMousePosition, vPosition );
		Vector3 vAlignedPosition( vPosition.x - (int) vPosition.x % 32, vPosition.y - (int) vPosition.y % 32 );
		matTransform.SetPosition( vAlignedPosition );

		QuadComponent* pQuadComponent = new QuadComponent();
		pQuadComponent->SetTexture( "HumanFarm.png" );	

		Entity* pEntity = Game::CreateEntity( matTransform );
		pEntity->AddComponent( pQuadComponent );
		pQuadComponent->Start();
		pQuadComponent->Release();
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