#include "Economy.h"
#include <Core/AssetManager/AssetManager.h>
#include <Core/Resources/XmlResource.h>

Economy::Economy(void)
{
	XmlResource* pGameResourcesXml = AssetManager::Get().GetAsset<XmlResource>( "GameResources.xml" );

	if ( pGameResourcesXml )
	{
		tinyxml2::XMLElement* pElement = pGameResourcesXml->GetRoot()->FirstChildElement();
		while ( pElement )
		{
			ResourceComponentData* pResource = new ResourceComponentData();
			pResource->VFromXML( pElement );
			m_pResources[ HashedString( pResource->Name ) ] = pResource;

			pElement = pElement->NextSiblingElement();
		}
	}

	int i = 0;
}


Economy::~Economy(void)
{
	for ( auto it : m_pResources )
	{
		delete it.second;
	}
}

unsigned int Economy::GetNumResources() const
{
	return m_pResources.size();
}

ResourceComponentData* Economy::GetResourceByIndex( unsigned int uiIndex )
{
	std::map< HashedString, ResourceComponentData* >::iterator pIter = m_pResources.begin();
	for ( unsigned int i = 0; i < uiIndex; ++i )
	{
		++pIter;
	}

	return pIter->second;
}
