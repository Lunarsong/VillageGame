//
//  Player.cpp
//  VillageGame
//
//  Created by Shanee on 10/14/13.
//
//

#include "ResourceStorage.h"

ResourceStorage::ResourceStorage()
{
    
}

ResourceStorage::~ResourceStorage()
{
    
}

int ResourceStorage::GetResourceAmount( const HashedString& hResource ) const
{
    std::map< HashedString, ResourceInfo >::const_iterator pIter = m_Resources.find( hResource );
    
    if ( pIter == m_Resources.end() )
    {
        return 0;
    }
    
    return pIter->second.iAmount;
}

int ResourceStorage::AdjustResourceAmount( const HashedString& hResource, int iAmount, ResourceChangeOption eChangeOption /*= MinimumZero*/ )
{
    std::map< HashedString, ResourceInfo >::iterator pIter = m_Resources.find( hResource );
    
    if ( pIter == m_Resources.end() )
    {
        return 0;
    }
    
    pIter->second.iAmount += iAmount;

	if ( pIter->second.iAmount > pIter->second.iCapacity )
	{
		pIter->second.iAmount = pIter->second.iCapacity;
	}
    
    if ( eChangeOption == MinimumZero && pIter->second.iAmount < 0 )
    {
        pIter->second.iAmount = 0;
    }
    
    return pIter->second.iAmount;
}

bool ResourceStorage::ConsumeResourceAmount( const HashedString& hResource, int iAmount )
{
    std::map< HashedString, ResourceInfo >::iterator pIter = m_Resources.find( hResource );
    
    if ( pIter == m_Resources.end() )
    {
        // Does not have from the resource
        return false;
    }
    
    if ( pIter->second.iAmount < iAmount )
    {
        // Does not have enough from the resource
        return false;
    }
    
    // Has enough from the resource and consumed it
    pIter->second.iAmount -= iAmount;
    
    return true;;
}

void ResourceStorage::AdjustCapacity( const HashedString& hResource, int iAdjustAmount )
{
	std::map< HashedString, ResourceInfo >::iterator pIter = m_Resources.find( hResource );

	if ( pIter == m_Resources.end() )
	{
		// Does not have from the resource
		m_Resources[ hResource ] = ResourceInfo( 0, iAdjustAmount );
	}

	else
	{
		pIter->second.iCapacity += iAdjustAmount;
		if ( pIter->second.iCapacity < pIter->second.iAmount )
		{
			pIter->second.iAmount = pIter->second.iCapacity;
		}
	}
}

int ResourceStorage::GetCapacity( const HashedString& hResource ) const
{
	std::map< HashedString, ResourceInfo >::const_iterator pIter = m_Resources.find( hResource );

	if ( pIter == m_Resources.end() )
	{
		return 0;
	}

	return pIter->second.iCapacity;
}
