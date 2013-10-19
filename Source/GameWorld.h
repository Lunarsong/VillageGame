#pragma once

#include "IslandData.h"
#include <AI/Pathfinding/Graph/Implementations/SquarePathfindingGraph.h>
#include <TextureAtlas.h>
#include <Core/Math/RandomNumGen.h>

using namespace Engine;

class GameWorld
{
public:
	GameWorld(void);
	~GameWorld(void);

private:
	RandomNumGen m_Rand;
	IslandData m_IslandData;
	SquarePathfindingGraph* m_pPathGraph;
	TextureAtlas* m_pAtlas;

	void CreateMinimap();

	void PlaceTreeLogic( unsigned int uX, unsigned int uY, IslandData::Biome eBiome );
};

