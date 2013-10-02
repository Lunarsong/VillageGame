//
//  IslandData.cpp
//  VillageGame
//
//  Created by Shanee on 9/11/13.
//  Copyright (c) 2013 LunaEngine. All rights reserved.
//

#include "IslandData.h"
#include <stdio.h>
#include "Core/Math/RandomNumGen.h"
#include <vector>
#include <ctime>

IslandData::IslandData()
{
    m_pHeightMap = NULL;
    m_pHeightMaskMap = NULL;
        
    m_uiSizeX = 0;
    m_uiSizeY = 0;
}
    
IslandData::~IslandData()
{
    Destroy();
}
    
void IslandData::Destroy()
{
    if ( m_pHeightMap )
    {
        delete []  m_pHeightMap;
    }
        
    if ( m_pHeightMaskMap )
    {
        delete [] m_pHeightMaskMap;
    }
}
    
void IslandData::Generate( int iWidth, int iHeight )
{
    Destroy();
        
    m_uiSizeY = iHeight;
    m_uiSizeX = iWidth;
        
    m_pHeightMap = new float[ iHeight * iWidth ];
    Engine::SimplexNoise noise;
    noise.SetOctaves( 16.0f );
    noise.SetPersistence( 0.5f );
    noise.SetScale( 0.025f );
    noise.SetBounds( -0.65f, 1.0f );
        
    for ( unsigned int y = 0; y < iHeight; ++y )
    {
        for ( unsigned int x = 0; x < iWidth; ++x )
        {
            (*this)( x ,  y ) = noise.Noise( x, y );
        }
    }
        
    GenerateMaskMap();
        
}
    
void IslandData::Generate( int iWidth, int iHeight, unsigned long ulSeed )
{
    Destroy();
        
    m_pHeightMap = new float[ iHeight * iWidth ];
}
    
enum IslandDataMaskMapDirection
{
    Up,
    Down,
    Left,
    Right
};
    
void IslandData::GenerateMaskMap()
{
    m_pHeightMaskMap = new float[ m_uiSizeX * m_uiSizeY ];        
    for ( unsigned int y = 0; y < m_uiSizeY; ++y )
    {
        for ( unsigned int x = 0; x < m_uiSizeX; ++x )
        {
            m_pHeightMaskMap[ y * m_uiSizeY + x ] = 0.0f;
        }
    }
        
    Engine::RandomNumGen rand;
    rand.SetRandomSeed( time( NULL ) );
        
    std::vector< IslandDataMaskMapDirection > choices;
    choices.reserve( 4 );
    int iNumIterations0 = m_uiSizeX * m_uiSizeY * 0.85f;
    int iNumIterations1 = m_uiSizeX * m_uiSizeY * 0.05f;
    for ( int i = 0; i < iNumIterations0; ++i )
    {
        int x = rand.RandomInt( 15, m_uiSizeX - 16 );
        int y = rand.RandomInt( 15, m_uiSizeY - 16 );

		if ( x < 15 || y < 15 || x > 120 || y > 120 )
		{
			int huh = 0;
		}
            
        for ( int j = 0; j < iNumIterations1; ++j )
        {
            choices.clear();
            m_pHeightMaskMap[ y * m_uiSizeY + x ] += 7.0f;
            if ( m_pHeightMaskMap[ y * m_uiSizeY + x ] > 255.0f )
            {
                m_pHeightMaskMap[ y * m_uiSizeY + x ] = 255.0f;
            }
                
            float fCurrentValue = m_pHeightMaskMap[ y * m_uiSizeY + x ];
                
            if ( ( x - 1 ) > 0 )
            {
                if ( m_pHeightMaskMap[ y * m_uiSizeY + x-1 ] <= fCurrentValue )
                    choices.push_back( Left );
            }
                
            if ( ( x + 1 ) < ( m_uiSizeX - 1 ) )
            {
                if ( m_pHeightMaskMap[ y * m_uiSizeY + x+1 ] <= fCurrentValue )
                    choices.push_back( Right );
            }


            if ( ( y - 1 ) > 0 )
            {
                if ( m_pHeightMaskMap[ (y-1) * m_uiSizeY + x ] <= fCurrentValue )
                    choices.push_back( Down );
            }

                
            if ( ( y + 1 ) < ( m_uiSizeY - 1 ) )
            {
                if ( m_pHeightMaskMap[ (y+1) * m_uiSizeY + x ] <= fCurrentValue )
                    choices.push_back( Up );
            }

            if ( choices.size() == 0 )
                break;
                
            IslandDataMaskMapDirection eDirection = choices[ rand.RandomInt( choices.size() ) ];
            switch ( eDirection )
            {
                case Left:
                    x -= 1;
                    break;
                        
                case Right:
                    x += 1;
                    break;
                        
                case Up:
                    y += 1;
                    break;
                        
                case Down:
                    y -= 1;
                    break;
            }

                
        }
    }

    /*
        for i in range(0, int((width*height)*(.85))):
        x = random.randint(15, self.map_width-16)
        y = random.randint(15, self.map_height-16)
         
        for j in range(0, int((width*height)*(0.05))):
        particle_map[y][x] += 7
        if particle_map[y][x] >= 255:
        particle_map[y][x] == 255
        current_value = particle_map[y][x]
        choices = []
        if x-1 > 0:
        if particle_map[y][x-1] <= current_value:
        choices.append("left")
        if x+1 < self.map_width-1:
        if particle_map[y][x+1] <= current_value:
        choices.append("right")
        if y-1 > 0:
        if particle_map[y-1][x] <= current_value:
        choices.append("up")
        if y+1 < self.map_height-1:
        if particle_map[y+1][x] <= current_value:
        choices.append("down")
         
        if not choices:
        break;
         
        new = random.choice(choices)
        if new == "left":
        x -= 1
        elif new == "right":
        x += 1
        elif new == "up":
        y -= 1
        elif new == "down":
        y += 1
        */
        
}
    
float IslandData::GetHeight( int iX, int iY )
{
    return m_pHeightMap[ iY * m_uiSizeX + iX ] * GetMaskHeight( iX, iY );
}
    
float IslandData::GetMaskHeight( int iX, int iY )
{
	float fMask = m_pHeightMaskMap[ iY * m_uiSizeX + iX ];

	if ( fMask <= 0.0f )
		return 0.0f;

    return ( fMask / 255.0f - 0.5f ) * 2.0f;
}
    
inline
float& IslandData::operator() ( unsigned int iX, unsigned int iY )
{
    return m_pHeightMap[ iY * m_uiSizeX + iX ];
}

void IslandData::SetHeight( int iX, int iY, float fHeight )
{
	m_pHeightMap[ iY * m_uiSizeX + iX ] = fHeight;
}