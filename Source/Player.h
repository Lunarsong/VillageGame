//
//  Player.h
//  VillageGame
//
//  Created by Shanee on 10/14/13.
//
//

#pragma once

#include <map>
#include "ResourceComponentData.h"

class Player
{
public:
    Player();
    ~Player();
    
    enum ResourceChangeOption
    {
        MinimumZero = 0,
        NoMinimum,
        
        ResourceChangeOptionCount
    };
    
    int GetResourceAmount( const ResourceComponentData* pResource ) const;
    int ChangeResourceAmount( const ResourceComponentData* pResource, int iAmount, ResourceChangeOption eChangeOption = MinimumZero );
    bool ConsumeResourceAmount( const ResourceComponentData* pResource, int iAmount );
    
protected:
    std::map< const ResourceComponentData*, int > m_Resources;
};