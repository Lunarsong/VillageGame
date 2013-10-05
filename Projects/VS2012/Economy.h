#pragma once

#include "ResourceComponentData.h"
#include <Externals/HashedString/HashedString.h>
#include <map>

class Economy
{
public:
	Economy(void);
	~Economy(void);

	unsigned int GetNumResources() const;
	ResourceComponentData* GetResourceByIndex( unsigned int uiIndex );

private:
	std::map< HashedString, ResourceComponentData* > m_pResources;
};

