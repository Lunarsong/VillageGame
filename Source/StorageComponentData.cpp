#include "StorageComponentData.h"
#include <Core/Utils/XmlUtils.h>

using namespace Engine;
StorageComponentData::StorageComponentData(void)
{

}


StorageComponentData::~StorageComponentData(void)
{
}

void StorageComponentData::VSerialize( std::ostream &out ) const
{
	throw "Unimplemented";
}

void StorageComponentData::VDeserialize( std::istream& in )
{
	throw "Unimplemented";
}

tinyxml2::XMLElement* StorageComponentData::VToXML( tinyxml2::XMLElement* pTo ) const
{
	throw "Unimplemented";
	return NULL;
}

bool StorageComponentData::VFromXML( tinyxml2::XMLElement* pData )
{
	tinyxml2::XMLElement* pResource = pData->FirstChildElement( "Resource" );
	if ( !pResource )
	{
		return false;
	}

	while ( pResource )
	{
		ResourceStorage storage;
		std::string strResourceName;
		if ( XmlUtils::ReadTextElement( pResource, "Name", strResourceName ) == false )
		{
			return false;
		}

		storage.Resource = strResourceName;

		if ( XmlUtils::ReadTextElement( pResource, "Amount", storage.Amount ) == false )
		{
			return false;
		}

		Resources.push_back( storage );

		pResource = pResource->NextSiblingElement( "Resource" );
	}
	return true;
}
