#include "StorageComponent.h"
#include "StorageComponentData.h"

StorageComponent::StorageComponent( Player* pOwner, const ComponentData* pData ) : Component( pData )
{
	m_pOwnerPlayer = pOwner;
}


StorageComponent::~StorageComponent(void)
{
}

const HashedString& StorageComponent::GetType() const
{
	static const HashedString hType( "Storage" );
	return hType;
}

void StorageComponent::VStart()
{
	const StorageComponentData* pData = (const StorageComponentData*)m_pData;

	for ( auto it : pData->Resources )
	{
		m_pOwnerPlayer->GetStorage().AdjustCapacity( it.Resource, it.Amount );
	}
}

void StorageComponent::VDestroy()
{
	const StorageComponentData* pData = (const StorageComponentData*)m_pData;

	for ( auto it : pData->Resources )
	{
		m_pOwnerPlayer->GetStorage().AdjustCapacity( it.Resource, -it.Amount );
	}
}

void StorageComponent::VUpdate( float fDeltaSeconds )
{

}
