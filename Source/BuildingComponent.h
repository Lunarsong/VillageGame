#pragma once

#include <Game/Entities/Component.h>

using namespace Engine;

class BuildingComponent
    : public Component
{
public:
    enum BuildingState
    {
        Construction,
        HalfConstruction,
        Idle,
        
        BuildingStateCount
    };
    
    BuildingComponent( ComponentData* pData );
	~BuildingComponent(void);
    
    const HashedString& GetType() const;
    
    void VStart();
    
    void VUpdate( float fDeltaSeconds );
    
private:
    BuildingState m_eBuildingState;
    float m_fConstructionTimer;
};

