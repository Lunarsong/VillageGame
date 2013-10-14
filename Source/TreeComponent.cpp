#include "TreeComponent.h"

const HashedString PlantComponent::g_hType( "Tree" );

PlantComponent::PlantComponent(void)
{

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

}

void PlantComponent::VDisable()
{

}

void PlantComponent::VEnable()
{

}
