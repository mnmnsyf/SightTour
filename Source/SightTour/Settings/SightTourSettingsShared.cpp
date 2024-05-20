#include "Settings/SightTourSettingsShared.h"
#include "System/SightTourGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SightTourSettingsShared)

static FString SHARED_SETTINGS_SLOT_NAME = TEXT("SharedGameSettings");

namespace SightTourSettingsSharedCVars
{
	static float DefaultGamepadLeftStickInnerDeadZone = 0.25f;
	static FAutoConsoleVariableRef CVarGamepadLeftStickInnerDeadZone(
		TEXT("gpad.DefaultLeftStickInnerDeadZone"),
		DefaultGamepadLeftStickInnerDeadZone,
		TEXT("Gamepad left stick inner deadzone")
	);

	static float DefaultGamepadRightStickInnerDeadZone = 0.25f;
	static FAutoConsoleVariableRef CVarGamepadRightStickInnerDeadZone(
		TEXT("gpad.DefaultRightStickInnerDeadZone"),
		DefaultGamepadRightStickInnerDeadZone,
		TEXT("Gamepad right stick inner deadzone")
	);
}

USightTourSettingsShared::USightTourSettingsShared()
{
	/*FInternationalization::Get().OnCultureChanged().AddUObject(this, &ThisClass::OnCultureChanged);*/
}

int32 USightTourSettingsShared::GetLatestDataVersion() const
{
	// 0 = before subclassing ULocalPlayerSaveGame
	// 1 = first proper version
	return 1;
}

USightTourSettingsShared* USightTourSettingsShared::CreateTemporarySettings(const ULocalPlayer* LocalPlayer)
{
	// This is not loaded from disk but should be set up to save
	USightTourSettingsShared* SharedSettings = Cast<USightTourSettingsShared>(CreateNewSaveGameForLocalPlayer(USightTourSettingsShared::StaticClass(), LocalPlayer, SHARED_SETTINGS_SLOT_NAME));

	SharedSettings->ApplySettings();

	return SharedSettings;
}

USightTourSettingsShared* USightTourSettingsShared::LoadOrCreateSettings(const ULocalPlayer* LocalPlayer)
{
	// This will stall the main thread while it loads
	USightTourSettingsShared* SharedSettings = Cast<USightTourSettingsShared>(LoadOrCreateSaveGameForLocalPlayer(USightTourSettingsShared::StaticClass(), LocalPlayer, SHARED_SETTINGS_SLOT_NAME));

	SharedSettings->ApplySettings();

	return SharedSettings;
}

bool USightTourSettingsShared::AsyncLoadOrCreateSettings(const ULocalPlayer* LocalPlayer, FOnSettingsLoadedEvent Delegate)
{
	FOnLocalPlayerSaveGameLoadedNative Lambda = FOnLocalPlayerSaveGameLoadedNative::CreateLambda([Delegate]
	(ULocalPlayerSaveGame* LoadedSave)
		{
			USightTourSettingsShared* LoadedSettings = CastChecked<USightTourSettingsShared>(LoadedSave);

			LoadedSettings->ApplySettings();

			Delegate.ExecuteIfBound(LoadedSettings);
		});

	return ULocalPlayerSaveGame::AsyncLoadOrCreateSaveGameForLocalPlayer(USightTourSettingsShared::StaticClass(), LocalPlayer, SHARED_SETTINGS_SLOT_NAME, Lambda);
}

void USightTourSettingsShared::SaveSettings()
{
	// Schedule an async save because it's okay if it fails
	AsyncSaveGameToSlotForLocalPlayer();

	// TODO_BH: Move this to the serialize function instead with a bumped version number
	if (UEnhancedInputLocalPlayerSubsystem* System = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwningPlayer))
	{
		if (UEnhancedInputUserSettings* InputSettings = System->GetUserSettings())
		{
			InputSettings->AsyncSaveSettings();
		}
	}
}

void USightTourSettingsShared::ApplySettings()
{
	/*ApplySubtitleOptions();
	ApplyBackgroundAudioSettings();
	ApplyCultureSettings();*/

	if (UEnhancedInputLocalPlayerSubsystem* System = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwningPlayer))
	{
		if (UEnhancedInputUserSettings* InputSettings = System->GetUserSettings())
		{
			InputSettings->ApplySettings();
		}
	}
}

float USightTourSettingsShared::GetFusionVisionStrength() const
{
	return FusionVisionStrength;
}

void USightTourSettingsShared::SetFusionVisionStrength(float InFusionVisionStrength)
{
	InFusionVisionStrength = FMath::Clamp(InFusionVisionStrength, 0, 10);
	if (FusionVisionStrength != InFusionVisionStrength)
	{
		FusionVisionStrength = InFusionVisionStrength;

		USightTourGameInstance::Get()->SetFusionVisionStrength(FusionVisionStrength);
	}
}

bool USightTourSettingsShared::GetEnableFusionVision() const
{
	return bEnableFusionVision;
}

void USightTourSettingsShared::EnableFusionVision(bool bEnable)
{
	bEnableFusionVision = bEnable;
	USightTourGameInstance::Get()->EnableFusionVision(bEnable);
}
