#include "EconomyMenu.h"
#include "VillageGame.h"

EconomyMenu::EconomyMenu( VillageGame* pGame )
{
	m_pGame = pGame;
	m_pMenu = UserInterface::AddScreenFromFile( "Economy", "EconomyMenu.xml" );

	Economy* pEconomy = m_pGame->GetEconomy();

	UIElement* pPanel = m_pMenu->GetElement( "panel" );
	unsigned int uiNumResources = pEconomy->GetNumResources();
	float fX = 10.0f;
	float fY = 10.0f;
	Vector3 vPanelSize = pPanel->GetSizeInPixels();

	for ( unsigned int i = 0; i < uiNumResources; ++i )
	{
		if ( ( fY + 40.0f ) >= vPanelSize.y )
		{
			fX += 200.0f;
			fY = 10.0f;
		}

		ResourceComponentData* pResource = pEconomy->GetResourceByIndex( i );
		UIImage* pImage = new UIImage();
		pImage->SetTexture( pResource->Icon );
		pImage->SetSize( 32.0f, 32.0f );
		pImage->SetPosition( fX, fY );

		pPanel->AddChild( pImage );
		pImage->Release();

		UILabel* pLabel = new UILabel( pResource->Name, "Arial", 20 );
        pLabel->SetID( "label_" + pResource->Name + "_amount" );
		pLabel->SetPosition( fX + 40.0f, fY + 16.0f );
		pLabel->SetAlignment( Left );
		pPanel->AddChild( pLabel );
		pLabel->Release();
        
        ResourceUI resourceUI;
        resourceUI.m_pResource = pResource;
        resourceUI.m_pLabel = pLabel;
        m_pResourceUIComponents.push_back( resourceUI );

		fY += 50.0f;
	}
}


EconomyMenu::~EconomyMenu(void)
{
}

void EconomyMenu::Show()
{
	m_pMenu->SetVisible( true );
}

void EconomyMenu::Hide()
{
	m_pMenu->SetVisible( false );
}

void EconomyMenu::ToggleVisibility()
{
	m_pMenu->SetVisible( !m_pMenu->IsVisible() );
}

void EconomyMenu::Update( float fDeltaSeconds )
{
    Player* pPlayer = m_pGame->GetPlayer();
    
    int iResourceIndex = 0;
    for ( auto it : m_pResourceUIComponents )
    {
        if ( iResourceIndex == 1 )
        {
            pPlayer->ChangeResourceAmount( it.m_pResource, 1 );
        }
        
        it.m_pLabel->SetString( it.m_pResource->Name + ": " + ToString( pPlayer->GetResourceAmount( it.m_pResource ) ) );
        
        ++iResourceIndex;
    }
}
