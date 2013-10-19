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

	void UpdateGrowth( float fDeltaSeconds );

private:
	static const HashedString g_hType;

	float	m_fGrowthTimer;
	float	m_fTimeToNextStage;
	int		m_iGrowthStageIndex;
};

