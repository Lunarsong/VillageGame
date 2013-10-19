#pragma once
#include <Rendering/Interfaces/ITexture.h>
#include <Game/Entities/ComponentData.h>
#include <Core/Math/ValueRange.h>
#include <vector>

using namespace Engine;

class PlantComponentData :
	public ComponentData
{
public:
	PlantComponentData(void);
	~PlantComponentData(void);

	enum GrowthType
	{
		Seed = 0,
		Sprout,
		Growing,
		Mature,
		Dying,
		Dead,

		GrowthTypeCount
	};

	struct GrowthStage
	{
		SmartPtr<ITexture>	Texture;
		GrowthType			Type;
		FloatValueRange		TimeToNextStage;
	};

	const GrowthStage* GetGrowthStage ( int iIndex ) const;
	int GetGrowthStageCount() const;

	std::vector<GrowthStage> GrowthStages;
};

