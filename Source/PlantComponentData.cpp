#include "PlantComponentData.h"


PlantComponentData::PlantComponentData(void)
{
}


PlantComponentData::~PlantComponentData(void)
{
}

const PlantComponentData::GrowthStage* PlantComponentData::GetGrowthStage ( int iIndex ) const
{
	assert( iIndex > -1 && iIndex < GrowthStages.size()); 
	return &GrowthStages[iIndex];
}

int PlantComponentData::GetGrowthStageCount() const
{
	return GrowthStages.size();
}
