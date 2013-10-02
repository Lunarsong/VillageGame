#include "Main.h"
#include "VillageGame.h"

void Start()
{
	ResourceCache::Get()->AddResourceFile( "Working Folder", new DevelopmentResourceZipFile( FileUtils::GetWorkingFolder(), DevelopmentResourceZipFile::Editor ) );
	ResourceCache::Get()->AddResourceFile( "Assets", new DevelopmentResourceZipFile( FileUtils::GetWorkingFolder() + "Assets/", DevelopmentResourceZipFile::Editor ) );

	VillageGame* pGame = new VillageGame();
	BaseApplication::Get()->AttachProcess( pGame );
	pGame->Release();
}

void Update( float fDeltaSeconds )
{

}

void End()
{

}