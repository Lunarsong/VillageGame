#pragma once

#include <string>
#include <TextureData.h>
#include <Game/Entities/ComponentData.h>

using namespace Engine;

class BuildingComponentData : public ComponentData
{
public:
	BuildingComponentData(void);
	~BuildingComponentData(void);
    
    AUTO_SIZE;

	std::string				Name;				// Building's Name
	SmartPtr<TextureData>	Icon;				// Building's Icon
	
	SmartPtr<TextureData>	ConstructionImage;	// Image when building under construction
	SmartPtr<TextureData>	BuildMenuIcon;		// Image at build menu

	float ConstructionTime;						// Build time in seconds

	virtual void VSerialize( std::ostream &out ) const;
	virtual void VDeserialize( std::istream& in );

	virtual tinyxml2::XMLElement* VToXML( tinyxml2::XMLElement* pTo ) const;
	virtual bool VFromXML( tinyxml2::XMLElement* pData );
};