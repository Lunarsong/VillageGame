#include "BuildingComponentData.h"
#include <Core/Utils/XmlUtils.h>

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
        Icon = new TextureData( pIcon, 0.0f, 0.0f, 1.0f, 1.0f );
    }
    
    const char* pConstructionImage = XmlUtils::ReadTextElement( pData, "ConstructionImage" );
    if ( pConstructionImage )
    {
        ConstructionImage = new TextureData( pConstructionImage, 0.0f, 0.0f, 1.0f, 1.0f );
    }
    
    const char* pBuildMenuIcon = XmlUtils::ReadTextElement( pData, "BuildMenuIcon" );
    if ( pBuildMenuIcon )
    {
        BuildMenuIcon = new TextureData( pBuildMenuIcon, 0.0f, 0.0f, 1.0f, 1.0f );
    }
    
	return true;
}
