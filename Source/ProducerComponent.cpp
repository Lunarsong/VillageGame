//
//  ProducerComponent.cpp
//  VillageGame
//
//  Created by Shanee on 10/14/13.
//
//

#include "ProducerComponent.h"

ProducerComponent::ProducerComponent( Player* pOwner, const ComponentData* pData ) : Component( pData )
{
	m_fTimer = 0.0f;
	m_pOwnerPlayer = pOwner;
}

ProducerComponent::~ProducerComponent()
{

}

void ProducerComponent::VStart()
{
	m_fTimer = 0.0f;
}

void ProducerComponent::VUpdate( float fDeltaSeconds )
{
	const ProducerComponentData* pProducerData = (const ProducerComponentData*)m_pData;
	if ( pProducerData->Ingredients.size() > 0 )
	{
		for ( auto it : pProducerData->Ingredients )
		{
			if ( m_pOwnerPlayer->GetStorage().GetResourceAmount( it.Resource ) < it.Amount )
			{
				return;
			}
		}
	}

	m_fTimer += fDeltaSeconds;

	if ( m_fTimer >= pProducerData->ProductionTime )
	{
		m_fTimer = 0.0f;
		m_pOwnerPlayer->GetStorage().AdjustResourceAmount( pProducerData->Resource, pProducerData->Amount );
	}
}

const HashedString& ProducerComponent::GetType() const
{
	static const HashedString hType( "Producer" );

	return hType;
}
