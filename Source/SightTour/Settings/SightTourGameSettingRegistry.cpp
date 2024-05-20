// Copyright Epic Games, Inc. All Rights Reserved.

#include "SightTourGameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "Player/SightTourLocalPlayer.h"
#include "SightTourSettingsShared.h"

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
		if (USightTourLocalPlayer* LocalPlayer = Cast<USightTourLocalPlayer>(OwningLocalPlayer))
		{
			if (LocalPlayer->GetSharedSettings() == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

void USightTourGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	MouseAndKeyboardSettings = InitializeMouseAndKeyboardSettings(InLocalPlayer);
	RegisterSetting(MouseAndKeyboardSettings);

	FusionVisionSettings = InitializeFusionVisionSettings(InLocalPlayer);
	RegisterSetting(FusionVisionSettings);
}

void USightTourGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();
	
	if (USightTourLocalPlayer* LocalPlayer = Cast<USightTourLocalPlayer>(OwningLocalPlayer))
	{
		// Game user settings need to be applied to handle things like resolution, this saves indirectly
		LocalPlayer->GetLocalSettings()->ApplySettings(false);
		
		LocalPlayer->GetSharedSettings()->ApplySettings();
		LocalPlayer->GetSharedSettings()->SaveSettings();
	}
}

#undef LOCTEXT_NAMESPACE

