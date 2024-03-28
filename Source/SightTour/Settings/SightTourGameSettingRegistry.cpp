// Copyright Epic Games, Inc. All Rights Reserved.

#include "SightTourGameSettingRegistry.h"

#include "GameSettingCollection.h"
//#include "LyraSettingsShared.h"
//#include "Player/LyraLocalPlayer.h"

DEFINE_LOG_CATEGORY(LogSightTourGameSettingRegistry);

//--------------------------------------
// USightTourGameSettingRegistry
//--------------------------------------

USightTourGameSettingRegistry::USightTourGameSettingRegistry()
{
}

USightTourGameSettingRegistry* USightTourGameSettingRegistry::Get(ULocalPlayer* InLocalPlayer)
{
	USightTourGameSettingRegistry* Registry = FindObject<USightTourGameSettingRegistry>(InLocalPlayer, TEXT("SightTourGameSettingRegistry"), true);
	if (Registry == nullptr)
	{
		Registry = NewObject<USightTourGameSettingRegistry>(InLocalPlayer, TEXT("SightTourGameSettingRegistry"));
		Registry->Initialize(InLocalPlayer);
	}

	return Registry;
}

bool USightTourGameSettingRegistry::IsFinishedInitializing() const
{
	if (Super::IsFinishedInitializing())
	{
		/*if (ULyraLocalPlayer* LocalPlayer = Cast<ULyraLocalPlayer>(OwningLocalPlayer))
		{
			if (LocalPlayer->GetSharedSettings() == nullptr)
			{
				return false;
			}
		}*/

		return true;
	}

	return false;
}

void USightTourGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	//ULyraLocalPlayer* LyraLocalPlayer = Cast<ULyraLocalPlayer>(InLocalPlayer);

	MouseAndKeyboardSettings = InitializeMouseAndKeyboardSettings(InLocalPlayer);
	RegisterSetting(MouseAndKeyboardSettings);
}

void USightTourGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();
	
	//if (ULyraLocalPlayer* LocalPlayer = Cast<ULyraLocalPlayer>(OwningLocalPlayer))
	//{
	//	// Game user settings need to be applied to handle things like resolution, this saves indirectly
	//	LocalPlayer->GetLocalSettings()->ApplySettings(false);
	//	
	//	LocalPlayer->GetSharedSettings()->ApplySettings();
	//	LocalPlayer->GetSharedSettings()->SaveSettings();
	//}
}

#undef LOCTEXT_NAMESPACE

