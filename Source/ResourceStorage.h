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
#include <Externals/HashedString/HashedString.h>

class ResourceStorage
{
public:
    ResourceStorage();
    ~ResourceStorage();
    
    enum ResourceChangeOption
    {
        MinimumZero = 0,
        NoMinimum,
        
        ResourceChangeOptionCount
    };
    
	void AdjustCapacity( const HashedString& hResource, int iAdjustAmount );
	int GetCapacity( const HashedString& hResource ) const;

    int GetResourceAmount( const HashedString& hResource ) const;
    int AdjustResourceAmount( const HashedString& hResource, int iAmount, ResourceChangeOption eChangeOption = MinimumZero );
    bool ConsumeResourceAmount( const HashedString& hResource, int iAmount );
    
protected:
	struct ResourceInfo
	{
		ResourceInfo( int iAmountValue = 0, int iCapacityValue = 0 )
		{
			iAmount = iAmountValue;
			iCapacity = iCapacityValue;
		}

		int iAmount;
		int iCapacity;
	};
    std::map< HashedString, ResourceInfo > m_Resources;
};