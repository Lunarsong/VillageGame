#pragma once
#include "Game/Entities/Component.h"

using namespace Engine;

class PlantComponent :
	public Component
{
public:
	PlantComponent(void);
	~PlantComponent(void);

	virtual const HashedString& GetType() const;

	virtual void VStart();
	virtual void VEnd();

	virtual void VUpdate( float fDeltaSeconds );

	virtual void VDisable();
	virtual void VEnable();

private:
	static const HashedString g_hType;

};

