//
//  ProducerComponent.h
//  VillageGame
//
//  Created by Shanee on 10/14/13.
//
//

#pragma once

#include <Game/Entities/Component.h>
#include "ProducerComponentData.h"
#include "Player.h"

using namespace Engine;
class ProducerComponent : public Component
{
public:
	ProducerComponent( Player* pOwner, const ComponentData* pData );
	~ProducerComponent();

	virtual const HashedString& GetType() const;

	virtual void VStart();
	virtual void VUpdate( float fDeltaSeconds );

private:
	Player*	m_pOwnerPlayer;

	float m_fTimer;
};