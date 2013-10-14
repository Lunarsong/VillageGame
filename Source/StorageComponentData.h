#pragma once


#include <Game/Entities/ComponentData.h>
#include <list>

class StorageComponentData : public Engine::ComponentData
{
public:
	StorageComponentData(void);
	~StorageComponentData(void);

	AUTO_SIZE;

	virtual void VSerialize( std::ostream &out ) const ;
	virtual void VDeserialize( std::istream& in );

	virtual tinyxml2::XMLElement* VToXML( tinyxml2::XMLElement* pTo ) const;
	virtual bool VFromXML( tinyxml2::XMLElement* pData );

	struct ResourceStorage
	{
		HashedString	Resource;
		int				Amount;
	};
	std::list<ResourceStorage> Resources;
};

