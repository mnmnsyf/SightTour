// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/SightTourLocalPlayer.h"

#include "AudioMixerBlueprintLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Settings/SightTourSettingsLocal.h"
#include "Settings/SightTourSettingsShared.h"
#include "CommonUserSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SightTourLocalPlayer)

class UObject;

USightTourLocalPlayer::USightTourLocalPlayer()
{
}

void USightTourLocalPlayer::PostInitProperties()
{
	Super::PostInitProperties();

	if (USightTourSettingsLocal* LocalSettings = GetLocalSettings())
	{
		/*LocalSettings->OnAudioOutputDeviceChanged.AddUObject(this, &USightTourLocalPlayer::OnAudioOutputDeviceChanged);*/
	}
}

USightTourSettingsLocal* USightTourLocalPlayer::GetLocalSettings() const
{
	return USightTourSettingsLocal::Get();
}

USightTourSettingsShared* USightTourLocalPlayer::GetSharedSettings() const
{
	if (!SharedSettings)
	{
		// On PC it's okay to use the sync load because it only checks the disk
		// This could use a platform tag to check for proper save support instead
		bool bCanLoadBeforeLogin = PLATFORM_DESKTOP;
		
		if (bCanLoadBeforeLogin)
		{
			SharedSettings = USightTourSettingsShared::LoadOrCreateSettings(this);
		}
		else
		{
			// We need to wait for user login to get the real settings so return temp ones
			SharedSettings = USightTourSettingsShared::CreateTemporarySettings(this);
		}
	}

	return SharedSettings;
}

void USightTourLocalPlayer::LoadSharedSettingsFromDisk(bool bForceLoad)
{
	FUniqueNetIdRepl CurrentNetId = GetCachedUniqueNetId();
	if (!bForceLoad && SharedSettings && CurrentNetId == NetIdForSharedSettings)
	{
		// Already loaded once, don't reload
		return;
	}

	ensure(USightTourSettingsShared::AsyncLoadOrCreateSettings(this, USightTourSettingsShared::FOnSettingsLoadedEvent::CreateUObject(this, &USightTourLocalPlayer::OnSharedSettingsLoaded)));
}

void USightTourLocalPlayer::OnSharedSettingsLoaded(USightTourSettingsShared* LoadedOrCreatedSettings)
{
	// The settings are applied before it gets here
	if (ensure(LoadedOrCreatedSettings))
	{
		// This will replace the temporary or previously loaded object which will GC out normally
		SharedSettings = LoadedOrCreatedSettings;

		NetIdForSharedSettings = GetCachedUniqueNetId();
	}
}