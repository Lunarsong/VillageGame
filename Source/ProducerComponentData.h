#pragma once

#include <Game/Entities/ComponentData.h>

using namespace Engine;
class ProducerComponentData : public ComponentData
{
public:
	ProducerComponentData(void);
	~ProducerComponentData(void);

	AUTO_SIZE;

	virtual void VSerialize( std::ostream &out ) const ;
	virtual void VDeserialize( std::istream& in );

	virtual tinyxml2::XMLElement* VToXML( tinyxml2::XMLElement* pTo ) const;
	virtual bool VFromXML( tinyxml2::XMLElement* pData );

	struct Ingredient
	{
		HashedString	Resource;
		int				Amount;
	};

	HashedString	Resource;
	int				Amount;
	float			ProductionTime;

	std::list< Ingredient > Ingredients;
};

