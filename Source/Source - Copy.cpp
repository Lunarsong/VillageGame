#include "Engine.h"
#include <Rendering/Types/ColorF.h>
#include "Game/Entities/Components/Rendering/QuadComponent.h"
#include "Game/Entities/Components/Rendering/SpriteComponent.h"
#include "UI/UIImage.h"
#include "UI/UILabel.h"
#include <Rendering/Utils/RenderUtils.h>

using namespace Engine;

ITexture*		pTexture = NULL;
shared_ptr<Entity> pEntity;
UIElement* pUI;

#include "IslandData.h"
#include "AI/Pathfinding/Graph/Implementations/SquarePathfindingGraph.h"
#include "AI/Pathfinding/AStar/AStar.h"

SquarePathfindingGraph* pPathGraph = new SquarePathfindingGraph();
PathPlan* pPath = NULL;
shared_ptr<SpriteComponent> pSprites[30][30];
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
                    
                    
                    pSprites[ uiY ][ uiX ]->SetColor( color );
                    
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
                    
                    pSprites[ uiY ][ uiX ]->SetColor( ColorF::GREY );
                    
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

class myMouseHandler : public IMouseHandler
{
public:
	myMouseHandler()
	{
		InputManager::Get()->AddMouseHandler( this );
	}

	~myMouseHandler()
	{
		InputManager::Get()->RemoveMouseHandler( this );
	}

	virtual bool VOnMouseMove( const Vector3& vPosition, const Vector3& vDeltaPosition )
	{
		return false;
	}

	virtual bool VOnMouseButtonDown( const int iButtonIndex, const Vector3& vPosition )
	{

		return false;
	}

	virtual bool VOnMouseButtonUp( const int iButtonIndex, const Vector3& vPosition )
	{
		if ( iButtonIndex == 1 )
		{
			auto pNode = pPathGraph->VFindClosestNode( vPosition );
            pNode->SetBlocked( pNode->IsTraversable() );
            
            unsigned int uiX = (unsigned int)((vPosition.x) / 32.0f );
            unsigned int uiY = (unsigned int)((vPosition.y) / 32.0f );
            
            ColorF color = ColorF::GREEN;
            if ( ( uiX + uiY ) % 2 == 0 )
            {
                color = ColorF::BLUE;
            }
            
            if ( pNode->IsTraversable() == false )
            {
                color = ColorF::RED;
            }
            
            pSprites[ uiY ][ uiX ]->SetColor( color );

		}

		else if ( iButtonIndex == 0 )
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

                    
                    pSprites[ uiY ][ uiX ]->SetColor( color );
                    
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

                    pSprites[ uiY ][ uiX ]->SetColor( ColorF::GREY );
                    
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

};

IFont* pFont = NULL;
UIImage* pUIImage;
UILabel* pLabel;
myMouseHandler* pMouseHandler;

using namespace VillageGame;

void Start()
{
	pPathGraph->Create( 30, 30, 32.0f, false );
    //BaseApplication::Get()->VSetResolution( 1280, 1024 );
    IslandData island;
    island.Generate( 100, 100 );
 
	pMouseHandler = new myMouseHandler();
    
	ResourceCache::Get()->AddResourceFile( "Working Folder", new DevelopmentResourceZipFile( FileUtils::GetWorkingFolder(), DevelopmentResourceZipFile::Editor ) );
	ResourceCache::Get()->AddResourceFile( "Assets", new DevelopmentResourceZipFile( FileUtils::GetWorkingFolder() + "Assets/", DevelopmentResourceZipFile::Editor ) );
    
    
    
	shared_ptr<BinaryResource> pResource = ResourceCache::Get()->GetResource<BinaryResource>( "tiles.png");

	pTexture = IRenderer::CreateTexture();
//	pTexture->VCreate( pResource->Buffer(), pResource->Size() );
    unsigned int pMap[100][100];
    for ( int j = 0; j < 100; ++j )
    {
        for ( int i = 0; i < 100; ++i )
        {
            float fColor = island.GetHeight( j, i );
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
    pTexture->VCreate( 100, 100, 4, (char*)pMap[0] );

    
    Matrix matTransform;
    matTransform.BuildScale( Vector4( 50.0f, 50.0f, 1.0f ) );
    matTransform.SetPosition( 100.0f, 100.0f, 0.0f );
    
	pEntity = Game::CreateEntity( matTransform );
    shared_ptr< QuadComponent > pComponent( new QuadComponent() );
    //pComponent->SetTexture( pTexture );
    pEntity->AddComponent( pComponent );
    pEntity->Start();
    
    
    matTransform.Identify();
    matTransform.BuildScale( Vector4( 32.0f, 32.0f, 1.0f ) );
    
    for ( unsigned int y = 0; y < 30; ++y )
	{
		for ( unsigned int x = 0; x < 30; ++x )
		{
			ColorF color = ColorF::GREEN;
			if ( ( x + y ) % 2 == 0 )
			{
				color = ColorF::BLUE;
			}
            
			if ( pPathGraph->GetNode( x, y )->IsTraversable() == false )
			{
				color = ColorF::RED;
			}
            
            matTransform.SetPosition( Vector4( x * 32.0f, y * 32.0f ) );
            pEntity = Game::CreateEntity( matTransform );
            shared_ptr<SpriteComponent> pSprite( new SpriteComponent() );
            pSprites[y][x] = pSprite;
            //pSprite->SetTexture( pTexture );
            pSprite->SetColor( color );
            pEntity->AddComponent( pSprite );
            pEntity->Start();
		}
    }
    
    matTransform.Identify();
    matTransform.BuildScale( 0.25f, 0.25f, 1.0f );
    matTransform.SetPosition( Vector4( 100.0f, 100.0f ) );
    pEntity = Game::CreateEntity( matTransform );
    shared_ptr<SpriteComponent> pSprite( new SpriteComponent() );
    pSprite->SetTexture( "Sheep0007.png" );
    pEntity->AddComponent( pSprite );
    pEntity->Start();
    
    InputManager::Get()->AddTouchHandler( new myTouchHandler() );
    
    pFont = IRenderer::CreateFont();
    pFont->VCreate( "Arial" );
    
    Vector3 vSize;
    pFont->VGetTextSize( "Hello, my name is Mimi.", vSize );
    pUIImage = new UIImage( pTexture );
	pUIImage->SetPosition( 50.0f, 50.0f );
	pUIImage->SetPositionType( UICoordinateType::UIPixels );
    pUIImage->SetSize( 500, 500 );
    pLabel = new UILabel( "UI Label here" );
    
    pUI = new UIElement();
    pUI->AddChild( pLabel );
    pUI->AddChild( pUIImage );
    
    pUIImage->Release();
    pLabel->Release();
    
}

void Update( float fDeltaSeconds )
{
    Matrix mat = pEntity->GetTransform();
    mat.SetPosition( mat.GetPosition() + Vector4( 1.0f, 1.0f, 0.0f, 0.0f ) * fDeltaSeconds * 5 );
    pEntity->SetTransform( mat );
	
    if ( pPath )
    {
        if ( pPath->CheckForEnd() == false )
        {
            const Vector3& vPosition = pPath->GetCurrentNodePosition();
            Vector3 vDirection = (vPosition + Vector3( 16.0f, 16.0f ) ) - mat.GetPosition();
            vDirection.Normalize();
            mat.SetPosition( mat.GetPosition() + vDirection * fDeltaSeconds * 10 );
            pEntity->SetTransform( mat );
            RenderUtils::DrawRectangle( vPosition, Vector2( 32.0f, 32.0f ), ColorF::GREY );
            
            pPath->CheckForNextNode( mat.GetPosition() - Vector3( 16.0f, 16.0f ) );
        }
    }
    
}

void Render()
{
	IRenderContext* pRenderContext = IRenderer::Get()->VGetMainContext();

	for ( unsigned int y = 0; y < 30; ++y )
	{
		for ( unsigned int x = 0; x < 30; ++x )
		{
			ColorF color = ColorF::GREEN;
			if ( ( x + y ) % 2 == 0 )
			{
				color = ColorF::BLUE;
			}

			if ( pPathGraph->GetNode( x, y )->IsTraversable() == false )
			{
				color = ColorF::RED;
			}
            
		}

		
	}

	IRenderer* pRenderer = IRenderer::Get();
	Vector3 vScreen( (float)pRenderer->VGetScreenWidth(), (float)pRenderer->VGetScreenHeight(), 1.0f );

	UICoordinates::SetScreen( vScreen, Vector3( 1280.0f, 720.0f, 1.0f ) );

    //pUI->Draw( pRenderContext );
}

void End()
{
	SAFE_RELEASE( pTexture );
    
    pFont->Release();
    pUI->Release();
    
    for ( int x = 0; x < 30; ++x )
    {
        for ( int y = 0; y < 30; ++y )
        {
            pSprites[y][x] = nullptr;
        }
    }

	delete pMouseHandler;
}