#pragma once

#include <string>
#include <TextureData.h>

using namespace Engine;
class ResourceComponentData
{
public:
	ResourceComponentData(void);
	~ResourceComponentData(void);

	std::string				Name;
	SmartPtr<TextureData>	Icon;

	virtual void VSerialize( std::ostream &out ) const;
	virtual void VDeserialize( std::istream& in );

	virtual tinyxml2::XMLElement* VToXML( tinyxml2::XMLElement* pTo ) const;
	virtual bool VFromXML( tinyxml2::XMLElement* pData );
};

