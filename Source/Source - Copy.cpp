#include "Engine.h"
#include <Rendering/Types/ColorF.h>
#include "Game/Entities/Components/Rendering/QuadComponent.h"
#include "Game/Entities/Components/Rendering/SpriteComponent.h"
#include "UI/UIImage.h"
#include "UI/UILabel.h"

using namespace Engine;

ITexture*		pTexture = NULL;
shared_ptr<Entity> pEntity;
UIElement* pUI;

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
            Vector3 pos = pUI->GetPosition();
            pUI->SetPosition( pos.x + vDeltaPosition.x, pos.y + vDeltaPosition.y );
        }
        
        else if ( iNumTouchs == 2 )
        {
            
        }

		return false;
    }
};

IFont* pFont = NULL;
UIImage* pUIImage;
UILabel* pLabel;

#include "IslandData.h"
using namespace VillageGame;
void Start()
{
    BaseApplication::Get()->VSetResolution( 1280, 1024 );
    IslandData island;
    island.Generate( 100, 100 );
    
    
	ResourceCache::Get()->AddResourceFile( "Working Folder", new DevelopmentResourceZipFile( FileUtils::GetWorkingFolder(), DevelopmentResourceZipFile::Editor ) );
    
    
    
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
    pComponent->SetTexture( pTexture );
    pEntity->AddComponent( pComponent );
    pEntity->Start();
    
    
    matTransform.Identify();
    matTransform.SetPosition( Vector4( 00.0f, 0.0f ) );
    pEntity = Game::CreateEntity( matTransform );
    shared_ptr<SpriteComponent> pSprite( new SpriteComponent() );
    pSprite->SetTexture( pTexture );
    //pEntity->AddComponent( pSprite );
    //pEntity->Start();
    
    matTransform.Identify();
    matTransform.SetPosition( Vector4( 100.0f, 100.0f ) );
    pEntity = Game::CreateEntity( matTransform );
    pSprite.reset( new SpriteComponent() );
    pSprite->SetTexture( pTexture );
    pEntity->AddComponent( pSprite );
    pEntity->Start();
    
    InputManager::Get()->AddTouchHandler( new myTouchHandler() );
    
    pFont = IRenderer::CreateFont();
    pFont->VCreate( "Vera" );
    
    Vector3 vSize;
    pFont->VGetTextSize( "Hello, my name is Mimi.", vSize );
    pUIImage = new UIImage( pTexture );
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
	static ColorF color = ColorF::BLACK;

	static float fColorSpeed = 1.0f;
	color.Red += fDeltaSeconds * fColorSpeed;
	color.Green += fDeltaSeconds * fColorSpeed;
	color.Blue += fDeltaSeconds * fColorSpeed;

	if ( color.Red >= 1.0f || color.Red <= 0.0f )
	{
		fColorSpeed *= -1.0f;
	}	

	BaseApplication::Get()->SetClearColor( color );
}

void Render()
{
	IRenderContext* pRenderContext = IRenderer::Get()->VGetMainContext();

    //pFont->VPrint( pRenderContext, "Hi!?h|}{IMOPJ?...", Vector3::ZERO, Vector3::ONE );
    pUI->Draw( pRenderContext );
//    pUIImage->Draw( pRenderContext );
//    pLabel->Draw( pRenderContext );
}

void End()
{
	SAFE_RELEASE( pTexture );
    
    pFont->Release();
    pUI->Release();
}