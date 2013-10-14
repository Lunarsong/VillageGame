#include "ProducerComponentData.h"
#include <Core/Utils/XmlUtils.h>

using namespace Engine;
ProducerComponentData::ProducerComponentData(void)
{
}


ProducerComponentData::~ProducerComponentData(void)
{
}

void ProducerComponentData::VSerialize( std::ostream &out ) const
{
	throw "Implement me";
}

void ProducerComponentData::VDeserialize( std::istream& in )
{
	throw "Implement me";
}

tinyxml2::XMLElement* ProducerComponentData::VToXML( tinyxml2::XMLElement* pTo ) const
{
	throw "Implement me";
	return NULL;
}

bool ProducerComponentData::VFromXML( tinyxml2::XMLElement* pData )
{
	std::string strValue;
	if ( XmlUtils::ReadTextElement( pData, "Resource", strValue ) == false )
	{
		return false;
	}

	Resource = strValue;

	if ( XmlUtils::ReadTextElement( pData, "Amount", Amount ) == false )
	{
		Amount = 1;
	}

	if ( XmlUtils::ReadTextElement( pData, "ProductionTime", ProductionTime ) == false )
	{
		return false;
	}

	tinyxml2::XMLElement* pIngredient = pData->FirstChildElement( "Ingredient" );
	while ( pIngredient != NULL )
	{
		Ingredient ingredient;

		std::string strValue;
		if ( XmlUtils::ReadTextElement( pIngredient, "Resource", strValue ) == false )
		{
			return false;
		}

		ingredient.Resource = strValue;
		
		if ( XmlUtils::ReadTextElement( pIngredient, "Amount", ingredient.Amount ) == false )
		{
			ingredient.Amount = 1;
		}

		Ingredients.push_back( ingredient );

		pIngredient = pIngredient->NextSiblingElement( "Ingredient" );
	}

	return true;
}
