#include "Main.h"
#include "VillageGame.h"
#include <Core/AssetManager/FolderAssetBundle.h>
#include <Core/AssetManager/Loaders/ImageAssetLoader.h>

void Start()
{
	//ResourceCache::Get()->AddResourceFile( "Working Folder", new DevelopmentResourceZipFile( FileUtils::GetWorkingFolder(), DevelopmentResourceZipFile::Editor ) );
	//ResourceCache::Get()->AddResourceFile( "Assets", new DevelopmentResourceZipFile( FileUtils::GetWorkingFolder() + "Assets/", DevelopmentResourceZipFile::Editor ) );
    
    FolderAssetBundle* pBundle = new FolderAssetBundle( FileUtils::GetWorkingFolder() + "Assets/" );
    AssetManager::Get().AddAssetBundle( pBundle );
	pBundle = new FolderAssetBundle( FileUtils::GetWorkingFolder() );
	AssetManager::Get().AddAssetBundle( pBundle );

    AssetManager::Get().RegisterLoader( HashedString( "*.png" ), new ImageAssetLoader() );
    
    AssetManager::Get().GetAsset<ITexture>( "Avatar0.png" );
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