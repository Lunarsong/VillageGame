#include "Engine.h"
#include <Rendering/Types/ColorF.h>
#include "Game/Entities/Components/Rendering/QuadComponent.h"
#include "Game/Entities/Components/Rendering/SpriteComponent.h"
#include "UI/UIImage.h"
#include "UI/UserInterface.h"
#include "UI/UILabel.h"
#include <Rendering/Utils/RenderUtils.h>

using namespace Engine;

ITexture*		pTexture = NULL;
Entity* pEntity;
UIElement* pUI;

#include "IslandData.h"
#include "AI/Pathfinding/Graph/Implementations/SquarePathfindingGraph.h"
#include "AI/Pathfinding/AStar/AStar.h"
#include <UI/UIButtonImage.h>
#include <Game/Dialogue/DialogueXmlParser.h>
#include <Game/Dialogue/UI/DialogueInterface.h>
#include <Game/Entities/Components/Rendering/TileMapComponent.h>
#include <Game/Entities/Components/Rendering/CameraComponent.h>
#include <TextureAtlas.h>

SquarePathfindingGraph* pPathGraph = NULL;//new SquarePathfindingGraph();
PathPlan* pPath = NULL;
CameraComponent* g_pCamera = NULL;
class myTouchHandler : public ITouchHandler
{
public:
    int iNumTouchs;
    bool bScrolling;
    
    myTouchHandler()
    {
        iNumTouchs = 0;
        bScrolling = false;
    }
    
    ~myTouchHandler()
    {
        
    }
    
    bool VOnTouchStart( const int iTouchIndex, const Vector2& vPosition, const int iPressure )
    {
        ++iNumTouchs;

		return false;
    }
    
    bool VOnTouchEnd( const int iTouchIndex, const Vector2& vPosition, const int iPressure )
    {
        --iNumTouchs;
        
		Vector3 vWorldPosition, vRayDirection;
		//RenderUtils::Project( Vector2( vPosition.x, vPosition.y ), g_pCamera->GetProjection(), g_pCamera->GetView(), vWorldPosition, vRayDirection );
		//vWorldPosition = vWorldPosition + vRayDirection * Vector4( IRenderer::Get()->VGetScreenWidth() * 0.5f, IRenderer::Get()->VGetScreenHeight() * 0.5f );
		float fScale = InputManager::Get()->GetMouseWheel().y;
		if ( fScale == 0.0f )
		{
			fScale = 1.0f;
		}
		else if ( fScale < 0.0f )
		{
			fScale = -fScale * 2.0f;
		}
        
		else
		{
			fScale = 1.0f / ( fScale * 2.0f );
		}
        
		vWorldPosition = g_pCamera->GetPosition() -Vector4( IRenderer::Get()->VGetScreenWidth() * 0.5f * fScale, IRenderer::Get()->VGetScreenHeight() * 0.5f * fScale );
		vWorldPosition.x += vPosition.x * fScale;
		vWorldPosition.y += IRenderer::Get()->VGetScreenHeight() * fScale - vPosition.y * fScale;
		if ( iTouchIndex == 0 )
		{
			if ( pPath )
			{
				delete pPath;
				pPath = NULL;
			}
            
			auto pStartNode = pPathGraph->VFindClosestNode( pEntity->GetTransform().GetPosition() );
			auto pEndNode = pPathGraph->VFindClosestTraversableNode( vWorldPosition );
            
			if ( pStartNode->IsTraversable() && pEndNode->IsTraversable() )
			{
				pPath = AStar::FindPath( pPathGraph, pStartNode, pEndNode );
			}          
            
		}

		return false;
    }
    
    bool VOnTouchCancel( const int iTouchIndex, const Vector2& vPosition, const int iPressure )
    {
        --iNumTouchs;

		return false;
    }
    
    bool VOnTouchMove( const int iTouchIndex, const Vector2& vPosition, const Vector2& vDeltaPosition, const int iPressure )
    {
        if ( iTouchIndex == 0 && iNumTouchs == 1 )
        {
            float fSpeed = InputManager::Get()->GetMouseWheel().y;
			if ( fSpeed < 0.0f )
			{
				fSpeed = -fSpeed;
			}
            
			else
			{
				fSpeed = 1.0f;
			}
			g_pCamera->SetPosition( g_pCamera->GetPosition() + Vector3( -vDeltaPosition.x, vDeltaPosition.y ) * fSpeed );
            
        }
        
        else if ( iNumTouchs == 2 )
        {
            
        }

		return false;
    }
};

class myMouseHandler : public IMouseHandler
{
	bool m_bScroll;
public:
	myMouseHandler()
	{
		m_bScroll = false;
		InputManager::Get()->AddMouseHandler( this );
	}

	~myMouseHandler()
	{
		InputManager::Get()->RemoveMouseHandler( this );
	}

	virtual bool VOnMouseMove( const Vector3& vPosition, const Vector3& vDeltaPosition )
	{
		if ( m_bScroll )
		{
			float fSpeed = InputManager::Get()->GetMouseWheel().y;
			if ( fSpeed < 0.0f )
			{
				fSpeed = -fSpeed;
			}

			else
			{
				fSpeed = 1.0f;
			}
			g_pCamera->SetPosition( g_pCamera->GetPosition() + Vector3( -vDeltaPosition.x, vDeltaPosition.y ) * fSpeed );
		}
		return false;
	}

	virtual bool VOnMouseButtonDown( const int iButtonIndex, const Vector3& vPosition )
	{
		if ( iButtonIndex == 2 )
		{
			m_bScroll = true;
		}

		return false;
	}

	virtual bool VOnMouseButtonUp( const int iButtonIndex, const Vector3& vPosition )
	{
		Vector3 vWorldPosition, vRayDirection;
		//RenderUtils::Project( Vector2( vPosition.x, vPosition.y ), g_pCamera->GetProjection(), g_pCamera->GetView(), vWorldPosition, vRayDirection );
		//vWorldPosition = vWorldPosition + vRayDirection * Vector4( IRenderer::Get()->VGetScreenWidth() * 0.5f, IRenderer::Get()->VGetScreenHeight() * 0.5f );
		float fScale = InputManager::Get()->GetMouseWheel().y;
		if ( fScale == 0.0f )
		{
			fScale = 1.0f;
		}
		else if ( fScale < 0.0f )
		{
			fScale = -fScale * 2.0f;
		}

		else
		{
			fScale = 1.0f / ( fScale * 2.0f );
		}

		vWorldPosition = g_pCamera->GetPosition() -Vector4( IRenderer::Get()->VGetScreenWidth() * 0.5f * fScale, IRenderer::Get()->VGetScreenHeight() * 0.5f * fScale );
		vWorldPosition.x += vPosition.x * fScale;
		vWorldPosition.y += IRenderer::Get()->VGetScreenHeight() * fScale - vPosition.y * fScale;
		if ( iButtonIndex == 1 )
		{
			auto pStartNode = pPathGraph->VFindClosestTraversableNode( vWorldPosition );
			Matrix matTransform = pEntity->GetTransform();
			matTransform.SetPosition( pStartNode->GetPosition() );
			pEntity->SetTransform( matTransform );

			if ( pPath )
			{
				delete pPath;
				pPath = NULL;
			}
		}

		else if ( iButtonIndex == 2 )
		{
			m_bScroll = false;
		}

		else if ( iButtonIndex == 0 )
		{
			if ( pPath )
			{   
				delete pPath;
				pPath = NULL;
			}

			auto pStartNode = pPathGraph->VFindClosestNode( pEntity->GetTransform().GetPosition() );
			auto pEndNode = pPathGraph->VFindClosestTraversableNode( vWorldPosition );

			if ( pStartNode->IsTraversable() && pEndNode->IsTraversable() )
			{
				pPath = AStar::FindPath( pPathGraph, pStartNode, pEndNode );
			}          
            
		}
		

		return false;
	}

	virtual bool VOnMouseButtonDClick( const int iButtonIndex, const Vector3& vPosition )
	{

		return false;
	}

	virtual bool VOnMouseWheel( const Vector3& vPosition, const Vector3& vDelta )
	{
		Matrix matScale;
		if ( vDelta.y > 0.0f )
		{
			matScale.BuildScale( 2.0f, 2.0f, 2.0f );
		}
		else
		{
			matScale.BuildScale( 0.5f, 0.5f, 0.5f );
		}
		
		g_pCamera->SetProjection( g_pCamera->GetProjection() * matScale );

		return false;
	}

};

IFont* pFont = NULL;
UIImage* pUIImage;
UILabel* pLabel;
myMouseHandler* pMouseHandler;

DialogueTree* pDialogueTree = NULL;
void Start2()
{
    pPathGraph = new SquarePathfindingGraph();
	pDialogueTree = DialogueXmlParser::FromFile( "Dialogue.xml" );

    //BaseApplication::Get()->VSetResolution( 1280, 1024 );
 
	pMouseHandler = new myMouseHandler();    
    
    Matrix matTransform;
    matTransform.BuildScale( Vector4( 50.0f, 50.0f, 1.0f ) );
    matTransform.SetPosition( 100.0f, 100.0f, 0.0f );

	Matrix matProjection;
	matProjection.BuildOrthoLH( (float)IRenderer::Get()->VGetScreenWidth(), (float)IRenderer::Get()->VGetScreenHeight(), -1.0f, 1.0f );
	pEntity = Game::CreateEntity( matTransform );
	CameraComponent* pCameraComponent( new CameraComponent() );
	pEntity->AddComponent( pCameraComponent );
	pCameraComponent->Release();

	pCameraComponent->SetProjection( matProjection );
	pCameraComponent->SetPosition( Vector4( 000.1f, 00.0f, 0.0f, 1.0f ) );
	pEntity->Start();
	g_pCamera = pCameraComponent;
    g_pCamera->SetClearColor( ColorF::GREY );

    
   
	

    
    matTransform.Identify();
    matTransform.BuildScale( 22.5f, 22.5f, 1.0f );
    matTransform.SetPosition( Vector4( 100.0f, 100.0f ) );
    pEntity = Game::CreateEntity( matTransform );
    QuadComponent* pQuad( new QuadComponent() );
    pQuad->SetTexture( "Sheep0007.png" );
	
    pEntity->AddComponent( pQuad );
	pQuad->Release();
    pEntity->Start();
    
    InputManager::Get()->AddTouchHandler( new myTouchHandler() );


	Process* pProcess = new DialogueInterface( NULL, "DialogueInterface.xml", "Dialogue.xml" );
	BaseApplication::Get()->AttachProcess( pProcess );
	pProcess->Release();
}

void Update2( float fDeltaSeconds )
{
    Matrix mat = pEntity->GetTransform();
	
    if ( pPath )
    {
        if ( pPath->CheckForEnd() == false )
        {
            const Vector3& vPosition = pPath->GetCurrentNodePosition();
            Vector3 vDirection = (vPosition ) - mat.GetPosition();
            vDirection.Normalize();
            mat.SetPosition( mat.GetPosition() + vDirection * fDeltaSeconds * 35.0f );
            pEntity->SetTransform( mat );
            
            pPath->CheckForNextNode( mat.GetPosition()  );
        }
    }    
}

void End2()
{
	SAFE_RELEASE( pDialogueTree );
	SAFE_RELEASE( pTexture );
    
    pFont->Release();
    pEntity = nullptr;

	delete pMouseHandler;
}