#include "BuildingComponent.h"
#include "BuildingComponentData.h"
#include <Game/Entities/Components/Rendering/QuadComponent.h>
#include <Game/Entities/Entity.h>

BuildingComponent::BuildingComponent( ComponentData* pComponentData ) : Component( pComponentData )
{
    
}


BuildingComponent::~BuildingComponent(void)
{
}

const HashedString& BuildingComponent::GetType() const
{
    static const HashedString g_hType( "Building" );
    return g_hType;
}

void BuildingComponent::VStart()
{
    m_eBuildingState = Construction;
    
    const BuildingComponentData* pData = (const BuildingComponentData*)m_pData;
    m_fConstructionTimer = pData->ConstructionTime;
    
    QuadComponent* pQuadComponent = m_pOwner->GetComponent<QuadComponent>( QuadComponent::g_hType );
    if ( !pQuadComponent )
    {
        pQuadComponent = new QuadComponent();
        m_pOwner->AddComponent( pQuadComponent );
        pQuadComponent->Release();
    }
    
    else
    {
        m_pOwner->AddComponent( pQuadComponent );
    }
    
    pQuadComponent->SetTexture( pData->FoundationImage );
    pQuadComponent->Start();
}

void BuildingComponent::VUpdate( float fDeltaSeconds )
{
    if ( m_fConstructionTimer > 0.0f )
    {
        const BuildingComponentData* pData = (const BuildingComponentData*)m_pData;
        m_fConstructionTimer -= fDeltaSeconds;
        
        if ( m_fConstructionTimer <= 0.0f )
        {
            QuadComponent* pQuadComponent = m_pOwner->GetComponent<QuadComponent>( QuadComponent::g_hType );
            pQuadComponent->SetTexture( pData->Icon );
        }
        
        else if ( m_eBuildingState == Construction && m_fConstructionTimer <= ( pData->ConstructionTime * 0.5f ) )
        {
            QuadComponent* pQuadComponent = m_pOwner->GetComponent<QuadComponent>( QuadComponent::g_hType );
            m_eBuildingState = HalfConstruction;
            pQuadComponent->SetTexture( pData->ConstructionImage );
        }
    }
}