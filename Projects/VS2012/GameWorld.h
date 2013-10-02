#pragma once

#include "IslandData.h"
#include <AI/Pathfinding/Graph/Implementations/SquarePathfindingGraph.h>
#include <TextureAtlas.h>

using namespace Engine;

class GameWorld
{
public:
	GameWorld(void);
	~GameWorld(void);

private:
	IslandData m_IslandData;
	SquarePathfindingGraph* m_pPathGraph;
	TextureAtlas* m_pAtlas;

	void CreateMinimap();
};

