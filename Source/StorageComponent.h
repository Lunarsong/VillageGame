#pragma once

#include <Game/Entities/Component.h>
#include "Player.h"

using namespace Engine;
class StorageComponent : public Component
{
public:
	StorageComponent( Player* pOwner, const ComponentData* pData );
	~StorageComponent();

	virtual const HashedString& GetType() const;

	virtual void VStart();
	virtual void VDestroy();
	virtual void VUpdate( float fDeltaSeconds );

private:
	Player* m_pOwnerPlayer;
};

