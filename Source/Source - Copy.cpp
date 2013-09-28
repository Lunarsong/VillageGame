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
shared_ptr<Entity> pEntity;
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

SquarePathfindingGraph* pPathGraph = new SquarePathfindingGraph();
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

using namespace VillageGame;

DialogueTree* pDialogueTree = NULL;
void Start()
{
	ResourceCache::Get()->AddResourceFile( "Working Folder", new DevelopmentResourceZipFile( FileUtils::GetWorkingFolder(), DevelopmentResourceZipFile::Editor ) );
	ResourceCache::Get()->AddResourceFile( "Assets", new DevelopmentResourceZipFile( FileUtils::GetWorkingFolder() + "Assets/", DevelopmentResourceZipFile::Editor ) );

	pDialogueTree = DialogueXmlParser::FromFile( "Dialogue.xml" );

    //BaseApplication::Get()->VSetResolution( 1280, 1024 );
    IslandData island;
    island.Generate( 128, 128 );
 
	pMouseHandler = new myMouseHandler();    
    
    Matrix matTransform;
    matTransform.BuildScale( Vector4( 50.0f, 50.0f, 1.0f ) );
    matTransform.SetPosition( 100.0f, 100.0f, 0.0f );

	Matrix matProjection;
	matProjection.BuildOrthoLH( (float)IRenderer::Get()->VGetScreenWidth(), (float)IRenderer::Get()->VGetScreenHeight(), -1.0f, 1.0f );
	pEntity = Game::CreateEntity( matTransform );
	shared_ptr< CameraComponent > pCameraComponent( new CameraComponent() );
	pEntity->AddComponent( pCameraComponent );
	pCameraComponent->SetProjection( matProjection );
	pCameraComponent->SetPosition( Vector4( 000.1f, 00.0f, 0.0f, 1.0f ) );
	pEntity->Start();
	g_pCamera = pCameraComponent.get();
    

    
   
	TextureAtlas* pAtlas = new TextureAtlas( "terrain_atlas.png" );
	/*pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) );
	pAtlas->AddTexture( Rect( 704, 352, 32, 32 ) );
	pAtlas->AddTexture( Rect( 735, 352, 32, 32 ) );
	pAtlas->AddTexture( Rect( 672, 160, 32, 32 ) );
	pAtlas->AddTexture( Rect( 704, 160, 32, 32 ) );
	pAtlas->AddTexture( Rect( 735, 160, 32, 32 ) );

	// Water
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) );
	pAtlas->AddTexture( Rect( 96, 448, 32, 32 ) );
	pAtlas->AddTexture( Rect( 128, 448, 32, 32 ) );
	pAtlas->AddTexture( Rect( 160, 448, 32, 32 ) );
	pAtlas->AddTexture( Rect( 192, 448, 32, 32 ) );
	pAtlas->AddTexture( Rect( 224, 448, 32, 32 ) );

	// Single tile water puddles
	pAtlas->AddTexture( Rect( 192, 288, 32, 32 ) );
	pAtlas->AddTexture( Rect( 192, 320, 32, 32 ) );

	// Transitions
	pAtlas->AddTexture( Rect( 192, 352, 32, 32 ) );
	pAtlas->AddTexture( Rect( 224, 352, 32, 32 ) );
	pAtlas->AddTexture( Rect( 256, 352, 32, 32 ) );

	pAtlas->AddTexture( Rect( 192, 384, 32, 32 ) );
	pAtlas->AddTexture( Rect( 256, 384, 32, 32 ) );

	pAtlas->AddTexture( Rect( 192, 416, 32, 32 ) );
	pAtlas->AddTexture( Rect( 224, 416, 32, 32 ) );
	pAtlas->AddTexture( Rect( 256, 416, 32, 32 ) );

	//

	pAtlas->AddTexture( Rect( 224, 288, 32, 32 ) );
	pAtlas->AddTexture( Rect( 256, 288, 32, 32 ) );

	pAtlas->AddTexture( Rect( 224, 320, 32, 32 ) );
	pAtlas->AddTexture( Rect( 256, 320, 32, 32 ) );*/
	//

	pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 0	- Pure Grass
	pAtlas->AddTexture( Rect( 224, 416, 32, 32 ) ); // 1	- Grass with Top water only
	pAtlas->AddTexture( Rect( 192, 384, 32, 32 ) ); // 2	- Grass with Water right only
	pAtlas->AddTexture( Rect( 256, 288, 32, 32 ) ); // 3	- Grass with Top and right both water
	pAtlas->AddTexture( Rect( 224, 352, 32, 32 ) ); // 4	- Grass with Water bottom only
	pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 5	- Grass with Water top and bottom only - UNASSIGNED
	pAtlas->AddTexture( Rect( 256, 320, 32, 32 ) ); // 6	- Grass with Water bottom and right only
	pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 7	- Grass with water on bottom, top and right - UNASSIGNED
	pAtlas->AddTexture( Rect( 256, 384, 32, 32 ) ); // 8	- Water left only
	pAtlas->AddTexture( Rect( 224, 288, 32, 32 ) ); // 9	- Grass with water on top and left only
	pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 10	- Grass with water on both left and right - UNASSIGNED
	pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 11	- Grass with water on top, left and right - UNASSIGNED
	pAtlas->AddTexture( Rect( 224, 320, 32, 32 ) ); // 12	- Grass with water on left and bottom
	pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 13	- Grass with water on bottom, left and top - UNASSIGNED
	pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) );	// 14	- Grass with water on left, bottom and right - UNASSIGNED
	pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 15	- Grass with water on left, bottom, top and right (all sides) - UNASSIGNED
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 16	- Here starts water, surrounded by grass
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 17	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 18	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 19	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 20	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 21	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 22	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 23	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 24	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 25	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 26	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 27	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 28	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 29	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 30	- 
	pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 31	- Pure water

	// Diagonal Corner transitions 32-35
	pAtlas->AddTexture( Rect( 192, 352, 32, 32 ) ); // 32 - Top left
	pAtlas->AddTexture( Rect( 256, 352, 32, 32 ) ); // 33 - Top Right
	pAtlas->AddTexture( Rect( 192, 416, 32, 32 ) ); // 34 - Bottom Left
	pAtlas->AddTexture( Rect( 256, 416, 32, 32 ) ); // 35 - Bottom Right

	// Grass Variations 36-41
	pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 36
	pAtlas->AddTexture( Rect( 704, 352, 32, 32 ) ); // 37
	pAtlas->AddTexture( Rect( 735, 352, 32, 32 ) ); // 38
	pAtlas->AddTexture( Rect( 672, 160, 32, 32 ) ); // 39
	pAtlas->AddTexture( Rect( 704, 160, 32, 32 ) ); // 40
	pAtlas->AddTexture( Rect( 735, 160, 32, 32 ) ); // 41
	
	float fWorldScale = 2.0f;
	for ( int i = 0; i < 5; ++i )
	{
		for ( unsigned int y = 0; y < 128; ++y )
		{
			for ( unsigned int x = 0; x < 128; ++x )
			{
				float fHeight = island.GetHeight( (float)x, (float)y );

				float fRight = island.GetHeight( (float)(x+1), (float)(y)  );
				float fLeft = island.GetHeight( (float)(x-1), (float)(y)  );
				float fTop = island.GetHeight( (float)(x), (float)(y+1)  );
				float fBottom = island.GetHeight( (float)(x), (float)(y-1)  );

				float fBottomLeft = island.GetHeight( (float)(x-1), (float)(y-1)  );
				float fBottomRight = island.GetHeight( (float)(x+1), (float)(y-1)  );
				float fTopLeft = island.GetHeight( (float)(x-1), (float)(y+1)  );
				float fTopRight = island.GetHeight( (float)(x+1), (float)(y+1)  );

				int iIndex = 0;
				if ( fHeight <= 0.0f )
				{
					iIndex += 16;
				}

				if ( fLeft <= 0.0f )
				{
					iIndex += 8;
				}

				if ( fRight <= 0.0f )
				{
					iIndex += 2;
				}

				if ( fTop <= 0.0f )
				{
					iIndex += 1;
				}

				if ( fBottom <= 0.0f )
				{
					iIndex += 4;
				}

				// 15, 14, 13, 11, 10, 7, 5
				if ( 
					iIndex == 15 || 
					iIndex == 14 || 
					iIndex == 13 || 
					iIndex == 11 || 
					iIndex == 10 || 
					iIndex == 7 || 
					iIndex == 5 ||
					false
					)
				{
					island.SetHeight( x, y, 0.0f );
				}
			}
		}
	}

	pTexture = IRenderer::CreateTexture();
	unsigned int pMap[128][128];
	for ( int j = 0; j < 128; ++j )
	{
		for ( int i = 0; i < 128; ++i )
		{
			float fColor = island.GetHeight( i, j );
			if ( fColor < 0.0f )
			{
				fColor = 0.0f;
			}

			ColorF color( fColor, fColor, fColor, 1.0f );

			if ( fColor <= 0.0f )
			{
				color = ColorF::BLUE;
			}
			Color colorRGB = color;
			pMap[ j ][ i ] =  colorRGB.RGBA;
		}
	}
	pTexture->VCreate( 128, 128, 4, (char*)pMap[0] );
	UIImage* pImage = new UIImage( pTexture );
	pImage->SetSize( pImage->GetSizeInPixels() * 2.0f );
    pImage->SetSizeType( UICoordinateType::UIScreenScaleMin );
	pImage->SetRelativePosition( Vector3( 0.0f, 1.0f ) );
	pImage->SetAlignment( BottomLeft );
	UserInterface::AddScreen( "Map", pImage );
	pImage->Release();

	matTransform.Identify();
	pEntity = Game::CreateEntity( matTransform );
	RandomNumGen rand;
	pPathGraph->Create( 128 * fWorldScale, 128 * fWorldScale, 32.0f, false );
	shared_ptr< TileMapComponent > pTileMapComponent( new TileMapComponent( 128 * fWorldScale, 128 * fWorldScale, 32.0f, pAtlas->GetTexture(), [&] ( unsigned int x, unsigned int y, RectF& rect )
		{
			float fHeight = island.GetHeight( (float)x / fWorldScale, 127.0f - (float)y / fWorldScale );
			
			float fRight = island.GetHeight( (float)(x+1) / fWorldScale, 127.0f - (float)(y) / fWorldScale );
			float fLeft = island.GetHeight( (float)(x-1) / fWorldScale, 127.0f - (float)(y) / fWorldScale );
			float fTop = island.GetHeight( (float)(x) / fWorldScale, 127.0f - (float)(y+1) / fWorldScale );
			float fBottom = island.GetHeight( (float)(x) / fWorldScale, 127.0f - (float)(y-1) / fWorldScale );

			float fBottomLeft = island.GetHeight( (float)(x-1) / fWorldScale, 127.0f - (float)(y-1) / fWorldScale );
			float fBottomRight = island.GetHeight( (float)(x+1) / fWorldScale, 127.0f - (float)(y-1) / fWorldScale );
			float fTopLeft = island.GetHeight( (float)(x-1) / fWorldScale, 127.0f - (float)(y+1) / fWorldScale );
			float fTopRight = island.GetHeight( (float)(x+1) / fWorldScale, 127.0f - (float)(y+1) / fWorldScale );

			int iIndex = 0;
			if ( fHeight <= 0.0f )
			{
				iIndex += 16;
			}

			if ( fLeft <= 0.0f )
			{
				iIndex += 8;
			}

			if ( fRight <= 0.0f )
			{
				iIndex += 2;
			}

			if ( fTop <= 0.0f )
			{
				iIndex += 1;
			}

			if ( fBottom <= 0.0f )
			{
				iIndex += 4;
			}
			
			if ( iIndex == 0 )
			{
				/*
				// 32 - Top left Grass
				// 33 - Top Right Grass
				// 34 - Bottom Left Grass
				// 35 - Bottom Right Grass
				*/
				if ( fBottomLeft <= 0.0f )
				{
					rect = pAtlas->GetTextureRect( 33 );
				}

				else if ( fTopLeft <= 0.0f )
				{
					rect = pAtlas->GetTextureRect( 35 );
				}

				else if ( fTopRight <= 0.0f )
				{
					rect = pAtlas->GetTextureRect( 34 );
				}

				else if ( fBottomRight <= 0.0f )
				{
					rect = pAtlas->GetTextureRect( 32 );
				}

				else
				{
					// All grass
					rect = pAtlas->GetTextureRect( 36 + rand.RandomInt( 6 ) );
				}

				pPathGraph->GetNode( x, y )->SetBlocked( false );

				return;
				
			}
			
			pPathGraph->GetNode( x, y )->SetBlocked( true );
			rect = pAtlas->GetTextureRect( iIndex );			
			
		}
	) );	
	pEntity->AddComponent( pTileMapComponent );
	pEntity->Start();

	delete pAtlas;

	/*
	//
	// Show which elements are blocked
	//
	ITexture* pBlocked = IRenderer::CreateTexture();
	pBlocked->VCreate( "DialogueCircle.png" );
	matTransform.Identify();
	matTransform.BuildScale( 32.0f, 32.0f, 1.0f );
	Vector3 vMinPosition( -128.0f * 32.0f * 0.5f + 16.0f, -128.0f * 32.0f * 0.5f + 16.0f, 0.0f );
	for ( int x = 0; x < 128; ++x )
	{
		for ( int y = 0; y < 128; ++y )
		{
			if ( pPathGraph->GetNode( x, y )->IsTraversable() == false )
			{
				matTransform.SetPosition( vMinPosition + Vector4( x * 32.0f, y * 32.0f ) );
				pEntity = Game::CreateEntity( matTransform );
				shared_ptr< QuadComponent > pComponent( new QuadComponent() );
				pComponent->SetTexture( pBlocked );
				pEntity->AddComponent( pComponent );
				pEntity->Start();				
			}

		}
	}
	pBlocked->Release();
	//
	//
	//
	*/
    
    matTransform.Identify();
    matTransform.BuildScale( 22.5f, 22.5f, 1.0f );
    matTransform.SetPosition( Vector4( 100.0f, 100.0f ) );
    pEntity = Game::CreateEntity( matTransform );
    shared_ptr<QuadComponent> pQuad( new QuadComponent() );
    pQuad->SetTexture( "Sheep0007.png" );
	
    pEntity->AddComponent( pQuad );
    pEntity->Start();
    
    InputManager::Get()->AddTouchHandler( new myTouchHandler() );
    
    pFont = IRenderer::CreateFont();
    pFont->VCreate( "Arial" );
    
    /*Vector3 vSize;
    pFont->VGetTextSize( "Hello, my name is Mimi.", vSize );
    pUIImage = new UIImage();
	pUIImage->SetTexture( "paper background.png" );
	pUIImage->SetPositionType( UICoordinateType::UIPixels );
    pUIImage->SetSize( 300, 100 );

	UIButtonImage* pButton = new UIButtonImage();
	pUIImage->AddChild( pButton );
	pButton->SetTexture(  "package_development.png" );
	pButton->SetSize( 64, 64 );
	pButton->SetPosition( 200, 50 );
	pButton->SetCallbackFunction( [](UIElement* pCallingElement, void* pArgs )
		{
			UIElement* pElement = UserInterface::GetScreen( "Construction" );
			if ( pElement )
			{
				pElement->SetVisible( !pElement->IsVisible() );
			}
		}
	);
	pButton->Release();

    pLabel = new UILabel( "UI Label here" );
    
    pUI = new UIPanel();
	pUI->SetSize( 300, 100 );
	pUI->AddChild( pLabel );
    pUI->AddChild( pUIImage );
	
    
    pUIImage->Release();
    pLabel->Release();

    
	UserInterface::AddScreen( "Game Menu", pUI );

	pUI->Release();

	pUI = new UIPanel();
	pUI->SetSize( 300, 100 );
	pUIImage = new UIImage();
	pUIImage->SetTexture( "paper background.png" );
	pUIImage->SetPositionType( UICoordinateType::UIPixels );
	pUIImage->SetSize( 300, 100 );
	pUI->AddChild( pUIImage );

	pButton = new UIButtonImage();
	pUIImage->AddChild( pButton );
	pButton->SetTexture(  "package_development.png" );
	pButton->SetSize( 64, 64 );
	pButton->SetPosition( 200, 50 );
	pButton->SetCallbackFunction( []( UIElement* pCallingElement, void* pArgs )
	{
		UIElement* pElement = UserInterface::GetScreen( "Construction" );
		if ( pElement )
		{
			pElement->SetVisible( !pElement->IsVisible() );
		}
	}
	);
	pButton->Release();
	pUIImage->Release();

	pUI->SetVisible( false );
	UserInterface::AddScreen( "Construction", pUI );
	pUI->Release();*/

	Process* pProcess = new DialogueInterface( NULL, "DialogueInterface.xml", "Dialogue.xml" );
	BaseApplication::Get()->AttachProcess( pProcess );
	pProcess->Release();
}

void Update( float fDeltaSeconds )
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

void Render()
{
	IRenderContext* pRenderContext = IRenderer::Get()->VGetMainContext();


	IRenderer* pRenderer = IRenderer::Get();
	Vector3 vScreen( (float)pRenderer->VGetScreenWidth(), (float)pRenderer->VGetScreenHeight(), 1.0f );

	UICoordinates::SetScreen( vScreen, Vector3( 1280.0f, 720.0f, 1.0f ) );

}

void End()
{
	SAFE_RELEASE( pDialogueTree );
	SAFE_RELEASE( pTexture );
    
    pFont->Release();
    pEntity = nullptr;

	delete pMouseHandler;
}