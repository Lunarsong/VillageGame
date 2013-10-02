#include "BuildingComponentData.h"


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
	return true;
}
