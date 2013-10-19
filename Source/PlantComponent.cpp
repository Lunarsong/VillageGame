#include "PlantComponent.h"
#include "PlantComponentData.h"

const HashedString PlantComponent::g_hType( "Tree" );

PlantComponent::PlantComponent(void)
{
	m_fGrowthTimer = 0.0f;
}

PlantComponent::~PlantComponent(void)
{

}

const HashedString& PlantComponent::GetType() const
{
	return g_hType;
}

void PlantComponent::VStart()
{

}

void PlantComponent::VEnd()
{

}

void PlantComponent::VUpdate( float fDeltaSeconds )
{
	UpdateGrowth( fDeltaSeconds );

}

void PlantComponent::UpdateGrowth( float fDeltaSeconds )
{
	// If there is a next growth stage, transition to it when we hit the right time - loop around if needed
	if (m_fTimeToNextStage > 0.0f && m_fGrowthTimer > m_fTimeToNextStage)
	{
		const PlantComponentData* pData = static_cast<const PlantComponentData*>( GetData() );

		m_iGrowthStageIndex++;
		m_iGrowthStageIndex %= pData->GetGrowthStageCount();

		const PlantComponentData::GrowthStage* growthStage = pData->GetGrowthStage( m_iGrowthStageIndex );

		m_fGrowthTimer = 0.0f;
		m_fTimeToNextStage = growthStage->TimeToNextStage.GenerateValue();
	}
}

void PlantComponent::VDisable()
{

}

void PlantComponent::VEnable()
{

}
