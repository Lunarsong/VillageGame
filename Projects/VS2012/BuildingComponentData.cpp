#include "BuildingComponentData.h"
#include <Core/Utils/XmlUtils.h>
#include <Core/AssetManager/AssetManager.h>

BuildingComponentData::BuildingComponentData(void)
{
}


BuildingComponentData::~BuildingComponentData(void)
{
}

void BuildingComponentData::VSerialize( std::ostream &out ) const
{
	throw "Unimplemented";
}

void BuildingComponentData::VDeserialize( std::istream& in )
{
	throw "Unimplemented";
}

tinyxml2::XMLElement* BuildingComponentData::VToXML( tinyxml2::XMLElement* pTo ) const
{
	throw "Unimplemented";

	return NULL;
}

bool BuildingComponentData::VFromXML( tinyxml2::XMLElement* pData )
{
    XmlUtils::ReadTextElement( pData, "Name", Name );
    XmlUtils::ReadTextElement( pData, "ConstructionTime", ConstructionTime );
    
    const char* pIcon = XmlUtils::ReadTextElement( pData, "Icon" );
    if ( pIcon )
    {
        Icon = AssetManager::Get().GetAsset<TextureData>( pIcon );
    }
    
    const char* pConstructionImage = XmlUtils::ReadTextElement( pData, "ConstructionImage" );
    if ( pConstructionImage )
    {
        ConstructionImage = AssetManager::Get().GetAsset<TextureData>( pConstructionImage );
    }
    
    const char* pBuildMenuIcon = XmlUtils::ReadTextElement( pData, "BuildMenuIcon" );
    if ( pBuildMenuIcon )
    {
        BuildMenuIcon = AssetManager::Get().GetAsset<TextureData>( pBuildMenuIcon );
    }
    
    const char* pFoundationImage = XmlUtils::ReadTextElement( pData, "FoundationImage" );
    if ( pFoundationImage )
    {
        FoundationImage = AssetManager::Get().GetAsset<TextureData>( pFoundationImage );
    }
    
    const char* pBuildingImage = XmlUtils::ReadTextElement( pData, "BuildingImage" );
    if ( pBuildingImage )
    {
        BuildingImage = AssetManager::Get().GetAsset<TextureData>( pBuildingImage );
    }
    
	return true;
}