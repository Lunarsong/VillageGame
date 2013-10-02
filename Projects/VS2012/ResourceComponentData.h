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
};

