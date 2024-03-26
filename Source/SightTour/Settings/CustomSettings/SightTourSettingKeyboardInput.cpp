// Fill out your copyright notice in the Description page of Project Settings.


#include "SightTourSettingKeyboardInput.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"

class ULocalPlayer;

#define LOCTEXT_NAMESPACE "SightTourSettings"

namespace SightTour::ErrorMessages
{
	static const FText UnknownMappingName = LOCTEXT("SightTourErrors_UnknownMappingName", "Unknown Mapping");
}

USightTourSettingKeyboardInput::USightTourSettingKeyboardInput()
{
	bReportAnalytics = false;
}

void USightTourSettingKeyboardInput::InitializeInputData(const UEnhancedPlayerMappableKeyProfile* KeyProfile, const FKeyMappingRow& MappingData, const FPlayerMappableKeyQueryOptions& InQueryOptions)
{
	check(KeyProfile);

	ProfileIdentifier = KeyProfile->GetProfileIdentifer();
	QueryOptions = InQueryOptions;

	for (const FPlayerKeyMapping& Mapping : MappingData.Mappings)
	{
		// Only add mappings that pass the query filters that have been provided upon creation
		if (!KeyProfile->DoesMappingPassQueryOptions(Mapping, QueryOptions))
		{
			continue;
		}

		ActionMappingName = Mapping.GetMappingName();
		InitialKeyMappings.Add(Mapping.GetSlot(), Mapping.GetCurrentKey());
		const FText& MappingDisplayName = Mapping.GetDisplayName();

		if (!MappingDisplayName.IsEmpty())
		{
			SetDisplayName(MappingDisplayName);
		}
	}

	const FString NameString = TEXT("KBM_Input_") + ActionMappingName.ToString();
	SetDevName(*NameString);
}

FText USightTourSettingKeyboardInput::GetKeyTextFromSlot(const EPlayerMappableKeySlot InSlot) const
{
	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		FPlayerMappableKeyQueryOptions QueryOptionsForSlot = QueryOptions;
		QueryOptionsForSlot.SlotToMatch = InSlot;

		if (const FKeyMappingRow* Row = FindKeyMappingRow())
		{
			for (const FPlayerKeyMapping& Mapping : Row->Mappings)
			{
				if (Profile->DoesMappingPassQueryOptions(Mapping, QueryOptionsForSlot))
				{
					return Mapping.GetCurrentKey().GetDisplayName();
				}
			}
		}
	}

	return EKeys::Invalid.GetDisplayName();
}

void USightTourSettingKeyboardInput::StoreInitial()
{
	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		if (const FKeyMappingRow* Row = FindKeyMappingRow())
		{
			for (const FPlayerKeyMapping& Mapping : Row->Mappings)
			{
				if (Profile->DoesMappingPassQueryOptions(Mapping, QueryOptions))
				{
					ActionMappingName = Mapping.GetMappingName();
					InitialKeyMappings.Add(Mapping.GetSlot(), Mapping.GetCurrentKey());
				}
			}
		}
	}
}

void USightTourSettingKeyboardInput::ResetToDefault()
{
	if (UEnhancedInputUserSettings* Settings = GetUserSettings())
	{
		FMapPlayerKeyArgs Args = {};
		Args.MappingName = ActionMappingName;

		FGameplayTagContainer FailureReason;
		Settings->ResetAllPlayerKeysInRow(Args, FailureReason);

		NotifySettingChanged(EGameSettingChangeReason::Change);
	}
}

void USightTourSettingKeyboardInput::RestoreToInitial()
{
	for (TPair<EPlayerMappableKeySlot, FKey> Pair : InitialKeyMappings)
	{
		ChangeBinding((int32)Pair.Key, Pair.Value);
	}
}

bool USightTourSettingKeyboardInput::ChangeBinding(int32 InKeyBindSlot, FKey NewKey)
{
	if (!NewKey.IsGamepadKey())
	{
		FMapPlayerKeyArgs Args = {};
		Args.MappingName = ActionMappingName;
		Args.Slot = (EPlayerMappableKeySlot)(static_cast<uint8>(InKeyBindSlot));
		Args.NewKey = NewKey;
		// If you want to, you can additionally specify this mapping to only be applied to a certain hardware device or key profile
		//Args.ProfileId =
		//Args.HardwareDeviceId =

		if (UEnhancedInputUserSettings* Settings = GetUserSettings())
		{
			FGameplayTagContainer FailureReason;
			Settings->MapPlayerKey(Args, FailureReason);
			NotifySettingChanged(EGameSettingChangeReason::Change);
		}

		return true;
	}

	return false;
}

void USightTourSettingKeyboardInput::GetAllMappedActionsFromKey(int32 InKeyBindSlot, FKey Key, TArray<FName>& OutActionNames) const
{
	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		Profile->GetMappingNamesForKey(Key, OutActionNames);
	}
}

bool USightTourSettingKeyboardInput::IsMappingCustomized() const
{
	bool bResult = false;

	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		FPlayerMappableKeyQueryOptions QueryOptionsForSlot = QueryOptions;

		if (const FKeyMappingRow* Row = FindKeyMappingRow())
		{
			for (const FPlayerKeyMapping& Mapping : Row->Mappings)
			{
				if (Profile->DoesMappingPassQueryOptions(Mapping, QueryOptionsForSlot))
				{
					bResult |= Mapping.IsCustomized();
				}
			}
		}
	}

	return bResult;
}

FText USightTourSettingKeyboardInput::GetSettingDisplayName() const
{
	if (const FKeyMappingRow* Row = FindKeyMappingRow())
	{
		if (Row->HasAnyMappings())
		{
			return Row->Mappings.begin()->GetDisplayName();
		}
	}

	return SightTour::ErrorMessages::UnknownMappingName;
}

FText USightTourSettingKeyboardInput::GetSettingDisplayCategory() const
{
	if (const FKeyMappingRow* Row = FindKeyMappingRow())
	{
		if (Row->HasAnyMappings())
		{
			return Row->Mappings.begin()->GetDisplayCategory();
		}
	}

	return SightTour::ErrorMessages::UnknownMappingName;
}

const FKeyMappingRow* USightTourSettingKeyboardInput::FindKeyMappingRow() const
{
	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		return Profile->FindKeyMappingRow(ActionMappingName);
	}

	ensure(false);
	return nullptr;
}

UEnhancedPlayerMappableKeyProfile* USightTourSettingKeyboardInput::FindMappableKeyProfile() const
{
	if (UEnhancedInputUserSettings* Settings = GetUserSettings())
	{
		return Settings->GetKeyProfileWithIdentifier(ProfileIdentifier);
	}

	ensure(false);
	return nullptr;
}

UEnhancedInputUserSettings* USightTourSettingKeyboardInput::GetUserSettings() const
{
	// Map the key to the player key profile
	if (UEnhancedInputLocalPlayerSubsystem* System = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		return System->GetUserSettings();
	}

	return nullptr;
}

void USightTourSettingKeyboardInput::OnInitialized()
{
	DynamicDetails = FGetGameSettingsDetails::CreateLambda([this](ULocalPlayer&)
		{
			if (const FKeyMappingRow* Row = FindKeyMappingRow())
			{
				if (Row->HasAnyMappings())
				{
					return FText::Format(LOCTEXT("DynamicDetails_KeyboardInputAction", "Bindings for {0}"), Row->Mappings.begin()->GetDisplayName());
				}
			}
			return FText::GetEmpty();
		});

	Super::OnInitialized();
}
