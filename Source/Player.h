#pragma once
#include "ResourceStorage.h"
#include <Externals/HashedString/HashedString.h>

class Player
{
public:
	Player(void);
	~Player(void);

	ResourceStorage& GetStorage();

private:
	HashedString m_Name;

	ResourceStorage m_Resources;
};

