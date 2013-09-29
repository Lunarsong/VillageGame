#pragma once
#include <Rendering/Interfaces/ITexture.h>
#include <Game/Entities/ComponentData.h>

using namespace Engine;

class TreeComponentData :
	public ComponentData
{
public:
	TreeComponentData(void);
	~TreeComponentData(void);

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
	};
};

