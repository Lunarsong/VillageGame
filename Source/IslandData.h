//
//  IslandData.h
//  VillageGame
//
//  Created by Shanee on 9/11/13.
//  Copyright (c) 2013 LunaEngine. All rights reserved.
//

#pragma once

#include "Core/Utils/SimplexNoise.h"

class IslandData
{
public:
    IslandData();
    ~IslandData();

    void Destroy();

    void Generate( int iWidth, int iHeight );
    void Generate( int iWidth, int iHeight, unsigned long ulSeed );

    float GetHeight( int iX, int iY );
	void SetHeight( int iX, int iY, float fHeight );
    float GetMaskHeight( int iX, int iY );

private:
    float* m_pHeightMap;
    float* m_pHeightMaskMap;
        
    int m_uiSizeX, m_uiSizeY;

    inline float& operator() ( unsigned int iX, unsigned int iY );
        
    void GenerateMaskMap();
};