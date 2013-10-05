#include "ResourceComponentData.h"
#include <Core/Utils/XmlUtils.h>
#include <Core/AssetManager/AssetManager.h>

ResourceComponentData::ResourceComponentData(void)
{
}


ResourceComponentData::~ResourceComponentData(void)
{
}

void ResourceComponentData::VSerialize( std::ostream &out ) const
{
	throw "Implement me";
}

void ResourceComponentData::VDeserialize( std::istream& in )
{
	throw "Implement me";
}

tinyxml2::XMLElement* ResourceComponentData::VToXML( tinyxml2::XMLElement* pTo ) const
{
	throw "Implement me";

	return NULL;
}

bool ResourceComponentData::VFromXML( tinyxml2::XMLElement* pData )
{
	XmlUtils::ReadTextElement( pData, "Name", Name );

	const char* pIcon = XmlUtils::ReadTextElement( pData, "Icon" );
	if ( pIcon )
	{
		Icon = AssetManager::Get().GetAsset<TextureData>( pIcon );
	}

	return true;
}
