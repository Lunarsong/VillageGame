//
//  Player.cpp
//  VillageGame
//
//  Created by Shanee on 10/14/13.
//
//

#include "Player.h"

Player::Player()
{
    
}

Player::~Player()
{
    
}

int Player::GetResourceAmount( const ResourceComponentData* pResource ) const
{
    std::map< const ResourceComponentData*, int >::const_iterator pIter = m_Resources.find( pResource );
    
    if ( pIter == m_Resources.end() )
    {
        return 0;
    }
    
    return pIter->second;
}

int Player::ChangeResourceAmount( const ResourceComponentData* pResource, int iAmount, ResourceChangeOption eChangeOption /*= MinimumZero*/ )
{
    std::map< const ResourceComponentData*, int >::iterator pIter = m_Resources.find( pResource );
    
    if ( pIter == m_Resources.end() )
    {
        m_Resources[ pResource ] = iAmount;
        return iAmount;
    }
    
    pIter->second += iAmount;
    
    if ( eChangeOption == MinimumZero && pIter->second < 0 )
    {
        pIter->second = 0;
    }
    
    return pIter->second;
}

bool Player::ConsumeResourceAmount( const ResourceComponentData* pResource, int iAmount )
{
    std::map< const ResourceComponentData*, int >::iterator pIter = m_Resources.find( pResource );
    
    if ( pIter == m_Resources.end() )
    {
        // Does not have from the resource
        return false;
    }
    
    if ( pIter->second < iAmount )
    {
        // Does not have enough from the resource
        return false;
    }
    
    // Has enough from the resource and consumed it
    pIter->second -= iAmount;
    
    return true;;
}