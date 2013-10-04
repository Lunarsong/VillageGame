#pragma once

#include <Game/Entities/Component.h>

using namespace Engine;

class BuildingComponent
    : public Component
{
public:
    BuildingComponent( ComponentData* pData );
	~BuildingComponent(void);
    
    const HashedString& GetType() const;
    
    void VStart();
    
    void VUpdate( float fDeltaSeconds );
    
private:
    float m_fConstructionTimer;
};

