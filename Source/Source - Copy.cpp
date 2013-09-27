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
class myTouchHandler : public ITouchHandler
{
public:
    int iNumTouchs;
    
    myTouchHandler()
    {
        iNumTouchs = 0;
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
        
		if ( iTouchIndex == 0 )
		{
			if ( pPath )
			{
                pPath->ResetPath();
                while ( pPath->CheckForEnd() == false )
                {
                    const Vector3& vPosition = pPath->GetCurrentNodePosition();
                    unsigned int uiX = (unsigned int)((vPosition.x) / 32.0f );
                    unsigned int uiY = (unsigned int)((vPosition.y) / 32.0f );
                    
                    ColorF color = ColorF::GREEN;
                    if ( ( uiX + uiY ) % 2 == 0 )
                    {
                        color = ColorF::BLUE;
                    }
                    if ( pPathGraph->GetNode( uiX, uiY )->IsTraversable() )
					{
						//pSprites[ uiY ][ uiX ]->SetColor( color );
					}
                    
                    pPath->CheckForNextNode( vPosition );
                }
                
                
				delete pPath;
			}
            
            
			auto pStartNode = pPathGraph->VFindClosestNode( pEntity->GetTransform().GetPosition() );
			auto pEndNode = pPathGraph->VFindClosestNode( vPosition );
			pPath = AStar::FindPath( pPathGraph, pStartNode, pEndNode );
        
			if ( pPath )
            {
                pPath->ResetPath();
                while ( pPath->CheckForEnd() == false )
                {
                    const Vector3& vPosition = pPath->GetCurrentNodePosition();
                    unsigned int uiX = (unsigned int)((vPosition.x) / 32.0f );
                    unsigned int uiY = (unsigned int)((vPosition.y) / 32.0f );
                    
					//pSprites[ uiY ][ uiX ]->SetColor( ColorF::GREY );
                    
                    pPath->CheckForNextNode( vPosition );
                }
                pPath->ResetPath();
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
            Matrix matTransform = pEntity->GetTransform();
            matTransform.SetPosition( Vector4( vPosition.x, vPosition.y ) );
            //pEntity->SetTransform( matTransform );
            
            Vector3 vPosition = IRenderer::Get()->VGetSpriteManager()->GetCameraPosition();
            vPosition.x -= vDeltaPosition.x;
            vPosition.y -= vDeltaPosition.y;
            
            //IRenderer::Get()->VGetSpriteManager()->VSetCameraPosition( vPosition );
            Vector3 pos = pUI->GetTopLeftPosition();
            pUI->SetPosition( pos.x + vDeltaPosition.x, pos.y + vDeltaPosition.y );
        }
        
        else if ( iNumTouchs == 2 )
        {
            
        }

		return false;
    }
};

CameraComponent* g_pCamera = NULL;
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
		vWorldPosition = g_pCamera->GetPosition() -Vector4( IRenderer::Get()->VGetScreenWidth() * 0.5f, IRenderer::Get()->VGetScreenHeight() * 0.5f );
		vWorldPosition.x += vPosition.x;
		vWorldPosition.y += IRenderer::Get()->VGetScreenHeight() - vPosition.y;
		if ( iButtonIndex == 1 )
		{
			

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
			}

			auto pStartNode = pPathGraph->VFindClosestNode( pEntity->GetTransform().GetPosition() );
			auto pEndNode = pPathGraph->VFindClosestNode( vWorldPosition );
			pPath = AStar::FindPath( pPathGraph, pStartNode, pEndNode );
            
            
            
            if ( pPath )
            {
                pPath->ResetPath();
                while ( pPath->CheckForEnd() == false )
                {
                    const Vector3& vPosition = pPath->GetCurrentNodePosition();
                    unsigned int uiX = (unsigned int)((vPosition.x) / 32.0f );
                    unsigned int uiY = (unsigned int)((vPosition.y) / 32.0f );

                    //pSprites[ uiY ][ uiX ]->SetColor( ColorF::GREY );
                    
                    pPath->CheckForNextNode( vPosition );
                }
                pPath->ResetPath();
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
    
    
	shared_ptr<BinaryResource> pResource = ResourceCache::Get()->GetResource<BinaryResource>( "tiles.png");

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
	pImage->SetRelativePosition( Vector3( 0.0f, 1.0f ) );
	pImage->SetAlignment( BottomLeft );
	UserInterface::AddScreen( "Map", pImage );
	pImage->Release();
    
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
    
	pEntity = Game::CreateEntity( matTransform );
    shared_ptr< QuadComponent > pComponent( new QuadComponent() );
    //pComponent->SetTexture( pTexture );
    pEntity->AddComponent( pComponent );
    pEntity->Start();
    
   
	TextureAtlas* pAtlas = new TextureAtlas( "terrain_atlas.png" );
	pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) );
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

	matTransform.Identify();
	pEntity = Game::CreateEntity( matTransform );
	RandomNumGen rand;
	float fWorldScale = 1.0f;
	pPathGraph->Create( 128 * fWorldScale, 128 * fWorldScale, 32.0f, false );
	shared_ptr< TileMapComponent > pTileMapComponent( new TileMapComponent( 128 * fWorldScale, 128 * fWorldScale, 32.0f, pAtlas->GetTexture(), [&] ( unsigned int x, unsigned int y, RectF& rect )
		{
			float fHeight = island.GetHeight( (float)x / fWorldScale, 127.0f - (float)y / fWorldScale );

			if ( fHeight <= 0.0f )
			{
				//rect = pAtlas->GetTextureRect( 6 + rand.RandomInt( 6 ) );
				rect = pAtlas->GetTextureRect( 6 );
			}

			else
			{
				rect = pAtlas->GetTextureRect( rand.RandomInt( 6 ) );
			}
			
		}
	) );	
	pEntity->AddComponent( pTileMapComponent );
	pEntity->Start();

	delete pAtlas;
    
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
            mat.SetPosition( mat.GetPosition() + vDirection * fDeltaSeconds * 10 );
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