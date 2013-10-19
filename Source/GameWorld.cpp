#include "GameWorld.h"
#include <Game/Game.h>
#include <Core/Math/RandomNumGen.h>
#include <Game/Entities/Components/Rendering/TileMapComponent.h>
#include <UI/UserInterface.h>
#include <TextureData.h>
#include <Core/AssetManager/AssetManager.h>
#include <Game/Entities/Components/Rendering/QuadComponent.h>


GameWorld::GameWorld(void)
{
	m_IslandData.Generate( 512, 512 );

	m_pAtlas = new TextureAtlas( "terrain_atlas.png" );

	m_pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 0	- Pure Grass
	m_pAtlas->AddTexture( Rect( 224, 416, 32, 32 ) ); // 1	- Grass with Top water only
	m_pAtlas->AddTexture( Rect( 192, 384, 32, 32 ) ); // 2	- Grass with Water right only
	m_pAtlas->AddTexture( Rect( 256, 288, 32, 32 ) ); // 3	- Grass with Top and right both water
	m_pAtlas->AddTexture( Rect( 224, 352, 32, 32 ) ); // 4	- Grass with Water bottom only
	m_pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 5	- Grass with Water top and bottom only - UNASSIGNED
	m_pAtlas->AddTexture( Rect( 256, 320, 32, 32 ) ); // 6	- Grass with Water bottom and right only
	m_pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 7	- Grass with water on bottom, top and right - UNASSIGNED
	m_pAtlas->AddTexture( Rect( 256, 384, 32, 32 ) ); // 8	- Water left only
	m_pAtlas->AddTexture( Rect( 224, 288, 32, 32 ) ); // 9	- Grass with water on top and left only
	m_pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 10	- Grass with water on both left and right - UNASSIGNED
	m_pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 11	- Grass with water on top, left and right - UNASSIGNEDa
	m_pAtlas->AddTexture( Rect( 224, 320, 32, 32 ) ); // 12	- Grass with water on left and bottom
	m_pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 13	- Grass with water on bottom, left and top - UNASSIGNED
	m_pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 14	- Grass with water on left, bottom and right - UNASSIGNED
	m_pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 15	- Grass with water on left, bottom, top and right (all sides) - UNASSIGNED
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 16	- Here starts water, surrounded by grass
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 17	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 18	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 19	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 20	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 21	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 22	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 23	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 24	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 25	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 26	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 27	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 28	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 29	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 30	- 
	m_pAtlas->AddTexture( Rect( 224, 384, 32, 32 ) ); // 31	- Pure water

	// Diagonal Corner transitions 32-35
	m_pAtlas->AddTexture( Rect( 192, 352, 32, 32 ) ); // 32 - Top left
	m_pAtlas->AddTexture( Rect( 256, 352, 32, 32 ) ); // 33 - Top Right
	m_pAtlas->AddTexture( Rect( 192, 416, 32, 32 ) ); // 34 - Bottom Left
	m_pAtlas->AddTexture( Rect( 256, 416, 32, 32 ) ); // 35 - Bottom Right

	// Grass Variations 36-41
	m_pAtlas->AddTexture( Rect( 672, 352, 32, 32 ) ); // 36
	m_pAtlas->AddTexture( Rect( 704, 352, 32, 32 ) ); // 37
	m_pAtlas->AddTexture( Rect( 735, 352, 32, 32 ) ); // 38
	m_pAtlas->AddTexture( Rect( 672, 160, 32, 32 ) ); // 39
	m_pAtlas->AddTexture( Rect( 704, 160, 32, 32 ) ); // 40
	m_pAtlas->AddTexture( Rect( 735, 160, 32, 32 ) ); // 41
	
	int iWorldScale = 1;
	for ( int i = 0; i < 5; ++i )
	{
		for ( unsigned int y = 1; y < 511; ++y )
		{
			for ( unsigned int x = 1; x < 511; ++x )
			{
				float fHeight = m_IslandData.GetHeight( x, y );

				float fRight = m_IslandData.GetHeight( (x+1), (y)  );
				float fLeft = m_IslandData.GetHeight( (x-1), (y)  );
				float fTop = m_IslandData.GetHeight( (x), (y+1)  );
				float fBottom = m_IslandData.GetHeight( (x), (y-1)  );

				float fBottomLeft = m_IslandData.GetHeight( (x-1), (y-1)  );
				float fBottomRight = m_IslandData.GetHeight( (x+1), (y-1)  );
				float fTopLeft = m_IslandData.GetHeight( (x-1), (y+1)  );
				float fTopRight = m_IslandData.GetHeight( (x+1), (y+1)  );

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
					m_IslandData.SetHeight( x, y, 0.0f );
				}
			}
		}
	}

	Entity* pEntity = Game::CreateEntity();
	RandomNumGen rand;

	m_pPathGraph = new SquarePathfindingGraph();
	m_pPathGraph->Create( 128 * iWorldScale, 128 * iWorldScale, 32.0, false );
	TileMapComponent* pTileMapComponent( new TileMapComponent( 512 * iWorldScale, 512 * iWorldScale, 32.0f, m_pAtlas->GetTexture(), [&] ( unsigned int x, unsigned int y, RectF& rect )
		{
			float fHeight = m_IslandData.GetHeight( x / iWorldScale, 511 - y / iWorldScale );
			
			float fRight = 0.0f;
			if ( x < 511 )
				fRight = m_IslandData.GetHeight( (x+1) / iWorldScale, 511 - (y) / iWorldScale );

			float fLeft = 0.0f;
			if ( x > 0 )
				fLeft = m_IslandData.GetHeight( (x-1) / iWorldScale, 511 - (y) / iWorldScale );
			
			float fTop = 0.0f;
			if ( y < 511 )
				fTop = m_IslandData.GetHeight( (x) / iWorldScale, 511 - (y+1) / iWorldScale );

			float fBottom = 0.0f;
			if ( y > 0 )
				fBottom = m_IslandData.GetHeight( (x) / iWorldScale, 511 - (y-1) / iWorldScale );

			float fTopRight = 0.0f;
			float fBottomLeft = 0.0f;
			float fBottomRight = 0.0f;
			float fTopLeft = 0.0f;

			if ( y > 0 && x > 0 )
				fBottomLeft = m_IslandData.GetHeight( (x-1) / iWorldScale, 511 - (y-1) / iWorldScale );

			if ( y > 0 && x < 511 )
				fBottomRight = m_IslandData.GetHeight( (x+1) / iWorldScale, 511 - (y-1) / iWorldScale );

			if ( y < 511 && x > 0 )
				fTopLeft = m_IslandData.GetHeight( (x-1) / iWorldScale, 511 - (y+1) / iWorldScale );

			if ( y < 511 && x < 511 )
				fTopRight = m_IslandData.GetHeight( (x+1) / iWorldScale, 511 - (y+1) / iWorldScale );

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
				//
				// 32 - Top left Grass
				// 33 - Top Right Grass
				// 34 - Bottom Left Grass
				// 35 - Bottom Right Grass
				//
				if ( fBottomLeft <= 0.0f )
				{
					rect = m_pAtlas->GetTextureRect( 33 );
				}

				else if ( fTopLeft <= 0.0f )
				{
					rect = m_pAtlas->GetTextureRect( 35 );
				}

				else if ( fTopRight <= 0.0f )
				{
					rect = m_pAtlas->GetTextureRect( 34 );
				}

				else if ( fBottomRight <= 0.0f )
				{
					rect = m_pAtlas->GetTextureRect( 32 );
				}

				else
				{
					// All grass
					rect = m_pAtlas->GetTextureRect( 36 + rand.RandomInt( 6 ) );
				}

				//m_pPathGraph->GetNode( x, y )->SetBlocked( false );

				return;
				
			}
			
			//m_pPathGraph->GetNode( x, y )->SetBlocked( true );
			rect = m_pAtlas->GetTextureRect( iIndex );			
			
		}
	) );	
	pEntity->AddComponent( pTileMapComponent );
	pTileMapComponent->Release();
	pEntity->Start();

	m_IslandData.GenerateBiomes();

	CreateMinimap();
}


GameWorld::~GameWorld(void)
{
	delete m_pAtlas;
	delete m_pPathGraph;
}

void GameWorld::CreateMinimap()
{
	/*ITexture* pTexture = IRenderer::CreateTexture();
	unsigned int* pMap = new unsigned int[ 512 * 512 ];

	for ( int j = 0; j < 512; ++j )
	{
		for ( int i = 0; i < 512; ++i )
		{
			float fColor = m_IslandData.GetHeight( i, j );
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
			pMap[ j * 512 + i ] =  colorRGB.RGBA;
		}
	}

	pTexture->VCreate( 512, 512, 4, (char*)pMap );
	UIImage* pImage = new UIImage( pTexture );
	pImage->SetSize( pImage->GetSizeInPixels() * 0.5f );
	pImage->SetSizeType( UICoordinateType::UIScreenScaleMin );
	pImage->SetRelativePosition( Vector3( 0.0f, 1.0f ) );
	pImage->SetAlignment( BottomLeft );
	UserInterface::AddScreen( "Map", pImage );
	pImage->Release();

	pTexture->Release();

	delete [] pMap;*/
    
    /*pTexture = IRenderer::CreateTexture();
	pMap = new unsigned int[ 512 * 512 ];
    
	for ( int j = 0; j < 512; ++j )
	{
		for ( int i = 0; i < 512; ++i )
		{
			float fColor = m_IslandData.GetMaskHeight( i, j );
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
			pMap[ j * 512 + i ] =  colorRGB.RGBA;
		}
	}
    
	pTexture->VCreate( 512, 512, 4, (char*)pMap );
	pImage = new UIImage( pTexture );
	pImage->SetSize( pImage->GetSizeInPixels() * 0.5f );
	pImage->SetSizeType( UICoordinateType::UIScreenScaleMin );
	pImage->SetRelativePosition( Vector3( 1.0f, 1.0f ) );
	pImage->SetAlignment( BottomRight );
	UserInterface::AddScreen( "Mask Map", pImage );
	pImage->Release();
    
	pTexture->Release();
    
	delete [] pMap;*/
	{
		ITexture* pTexture = IRenderer::CreateTexture();
		Color color;
		unsigned int* pMap = new unsigned int[ 512 * 512 ];
		for ( int j = 0; j < 512; ++j )
		{
			for ( int i = 0; i < 512; ++i )
			{
				IslandData::Biome eBiome = m_IslandData.GetBiome( i, j );
				PlaceTreeLogic( i, j, eBiome );

				if ( eBiome == IslandData::SeaWater )
				{
					color = Color::BLUE;;
				}

				else if ( eBiome == IslandData::FreshWater )
				{
					color = Color( 0, 191, 255 );
				}
				
				else if ( eBiome == IslandData::Grassland )
				{
					color = Color( 195, 211, 170, 255 );
				}

				else if ( eBiome == IslandData::Snow )
				{
					color = Color::WHITE;
				}

				else if ( eBiome == IslandData::Bare )
				{
					color = Color( 200, 200, 200, 255 );
				}

				else if ( eBiome == IslandData::Scorched )
				{
					color = Color::GREY;
				}

				else if ( eBiome == IslandData::Tundra )
				{
					color = Color( 220, 220, 186, 255 );
				}

				else if ( eBiome == IslandData::Taiga )
				{
					color = Color( 203, 211, 186, 255 );
				}

				else if ( eBiome == IslandData::Shrubland )
				{
					color = Color( 195, 203, 186, 255 );
				}

				else if ( eBiome == IslandData::TemperateDesert )
				{
					color = Color( 227, 231, 201, 255 );
				}

				else if ( eBiome == IslandData::TemperateRainForest )
				{
					color = Color( 163, 195, 167, 255 );
				}

				else if ( eBiome == IslandData::TemperateDecidousForest )
				{
					color = Color( 180, 200, 168, 255 );
				}

				else if ( eBiome == IslandData::TropicalRainForest )
				{
					color = Color( 155, 186, 168, 255 );
				}

				else if ( eBiome == IslandData::TropicalSeasonalForest )
				{
					color = Color( 168, 203, 163, 255 );
				}

				else if ( eBiome == IslandData::SubtropicalDesert )
				{
					color = Color( 232, 220, 198, 255 );
				}
                
                else if ( eBiome == IslandData::Unassigned )
                {
                    throw "Whoops";
                }
                
                else
                {
                    throw "Unhandled biome";
                }

				pMap[ j * 512 + i ] =  color.RGBA;
			}
						
		}
		
		pTexture->VCreate( 512, 512, 4, (char*)pMap );
		UIImage* pImage = new UIImage( pTexture );
		pImage->SetSize( pImage->GetSizeInPixels() * 0.5f );
		pImage->SetSizeType( UICoordinateType::UIScreenScaleMin );
		pImage->SetRelativePosition( Vector3( 0.0f, 1.0f ) );
		pImage->SetAlignment( BottomLeft );
		UserInterface::AddScreen( "BiomeMap", pImage );
		pImage->Release();

		pTexture->Release();

		delete [] pMap;
	}
/*
	{
		ITexture* pTexture = IRenderer::CreateTexture();
		Color color;
		unsigned int pMap[512][512];
		for ( int j = 0; j < 512; ++j )
		{
			for ( int i = 0; i < 512; ++i )
			{
				float fMoisture = m_IslandData.GetMoisture( i, j );
				if ( fMoisture >= 0.95f )
				{
					color = Color( 64, 105, 150 );
				}

				else
				{
					color = Color( 255, 248, 220 );
				}

				static const Color k_MoistureColor = Color( 64, 105, 150 );
				static const Color k_DesertColor = Color( 255, 248, 220 );
				color = Color::Lerp( k_DesertColor, k_MoistureColor, fMoisture );

				color = Color( fMoisture * 255, fMoisture * 255, fMoisture * 255, 255 );
				pMap[ j ][ i ] =  color.RGBA;
			}
		}

		pTexture->VCreate( 512, 512, 4, (char*)pMap[0] );
		UIImage* pImage = new UIImage( pTexture );
		pImage->SetSize( pImage->GetSizeInPixels() * 4.0f );
		pImage->SetSizeType( UICoordinateType::UIScreenScaleMin );
		pImage->SetRelativePosition( Vector3( 0.0f, 1.0f ) );
		pImage->SetAlignment( BottomLeft );
		//UserInterface::AddScreen( "MoistureMap", pImage );
		pImage->Release();

		pTexture->Release();
	}*/
}

void GameWorld::PlaceTreeLogic( unsigned int uX, unsigned int uY, IslandData::Biome eBiome )
{	
	TextureData* pTreeSprite = AssetManager::Get().GetAsset<TextureData>( "TreeSprite" );
	Matrix matTransform;
	matTransform.BuildScale( 96 / 3, 128 / 3 , 1.0f );
	float fWorldScale = 1.0f;
	float fHalfSize = 512.0f * 32.0f * ( fWorldScale * 0.5f );

	matTransform.SetPosition( uX * 32.0f * fWorldScale - fHalfSize + 16.0f, (511-uY) * 32.0f * fWorldScale - fHalfSize + 16.0f, 0.0f );

	float fChance = 0.0f;

	if ( eBiome == IslandData::SeaWater )
	{

	}

	else if ( eBiome == IslandData::FreshWater )
	{

	}

	else if ( eBiome == IslandData::Grassland )
	{
		fChance = 0.0f;
	}

	else if ( eBiome == IslandData::Snow )
	{

	}

	else if ( eBiome == IslandData::Bare )
	{

	}

	else if ( eBiome == IslandData::Scorched )
	{

	}

	else if ( eBiome == IslandData::Tundra )
	{

	}

	else if ( eBiome == IslandData::Taiga )
	{

	}

	else if ( eBiome == IslandData::Shrubland )
	{

	}

	else if ( eBiome == IslandData::TemperateDesert )
	{
		fChance = 0.0f;
	}

	else if ( eBiome == IslandData::TemperateRainForest )
	{
		fChance = 0.4f;
	}

	else if ( eBiome == IslandData::TemperateDecidousForest )
	{
		fChance = 0.3f;
	}

	else if ( eBiome == IslandData::TropicalRainForest )
	{
		fChance = 0.4f;
	}

	else if ( eBiome == IslandData::TropicalSeasonalForest )
	{
		fChance = 0.4f;
	}

	else if ( eBiome == IslandData::SubtropicalDesert )
	{
		fChance = 0.05f;
	}

	else if ( eBiome == IslandData::Unassigned )
	{
		throw "Whoops";
	}

	else
	{
		throw "Unhandled biome";
	}

	if ( fChance != 0.0f && m_Rand.RandomFloat( 0.0f, 1.0f ) <= fChance )
	{
		for ( int i = -1; i <= 1; ++i )
		{
			for ( int j = -1; j <= 1; ++j )
			{
				if ( i == 0 && j == 0 )
					continue;

				IslandData::Biome eNeighbourBiome;

				eNeighbourBiome = m_IslandData.GetBiome( uX + i, uY + j );
				if ( eNeighbourBiome == IslandData::SeaWater || eNeighbourBiome == IslandData::FreshWater )
					return;

			}		
		}

		Entity* pEntity = Game::CreateEntity( matTransform );
		QuadComponent* pQuad = new QuadComponent();
		pQuad->SetTexture( pTreeSprite );
		pEntity->AddComponent( pQuad );
		pQuad->Release();
		pQuad->Start();
	}


}
