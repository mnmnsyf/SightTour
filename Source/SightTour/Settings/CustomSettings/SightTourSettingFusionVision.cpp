// Fill out your copyright notice in the Description page of Project Settings.


#include "SightTourSettingFusionVision.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"

class ULocalPlayer;

#define LOCTEXT_NAMESPACE "SightTourSettings"

namespace SightTour::ErrorMessages
{
	static const FText UnknownMappingName = LOCTEXT("SightTourErrors_UnknownMappingName", "Unknown Mapping");
}

USightTourSettingFusionVision::USightTourSettingFusionVision()
{
	bReportAnalytics = false;
}

void USightTourSettingFusionVision::InitializeInputData(const UEnhancedPlayerMappableKeyProfile* KeyProfile, const FKeyMappingRow& MappingData, const FPlayerMappableKeyQueryOptions& InQueryOptions)
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

FText USightTourSettingFusionVision::GetKeyTextFromSlot(const EPlayerMappableKeySlot InSlot) const
{
	
	return EKeys::Invalid.GetDisplayName();
}

void USightTourSettingFusionVision::StoreInitial()
{
	/*if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
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
	}*/
}

void USightTourSettingFusionVision::ResetToDefault()
{
	/*if (UEnhancedInputUserSettings* Settings = GetUserSettings())
	{
		FMapPlayerKeyArgs Args = {};
		Args.MappingName = ActionMappingName;

		FGameplayTagContainer FailureReason;
		Settings->ResetAllPlayerKeysInRow(Args, FailureReason);

		NotifySettingChanged(EGameSettingChangeReason::Change);
	}*/
}

void USightTourSettingFusionVision::RestoreToInitial()
{
	for (TPair<EPlayerMappableKeySlot, FKey> Pair : InitialKeyMappings)
	{
		ChangeBinding((int32)Pair.Key, Pair.Value);
	}
}

bool USightTourSettingFusionVision::ChangeBinding(int32 InKeyBindSlot, FKey NewKey)
{
	if (!NewKey.IsGamepadKey())
	{
		FMapPlayerKeyArgs Args = {};
		Args.MappingName = ActionMappingName;
		Args.Slot = (EPlayerMappableKeySlot)(static_cast<uint8>(InKeyBindSlot));
		Args.NewKey = NewKey;

		/*if (UEnhancedInputUserSettings* Settings = GetUserSettings())
		{
			FGameplayTagContainer FailureReason;
			Settings->MapPlayerKey(Args, FailureReason);
			NotifySettingChanged(EGameSettingChangeReason::Change);
		}*/

		return true;
	}

	return false;
}

void USightTourSettingFusionVision::GetAllMappedActionsFromKey(int32 InKeyBindSlot, FKey Key, TArray<FName>& OutActionNames) const
{
	/*if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		Profile->GetMappingNamesForKey(Key, OutActionNames);
	}*/
}

bool USightTourSettingFusionVision::IsMappingCustomized() const
{
	bool bResult = false;

	/*if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
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
	}*/

	return bResult;
}

FText USightTourSettingFusionVision::GetSettingDisplayName() const
{
	return SightTour::ErrorMessages::UnknownMappingName;
}

FText USightTourSettingFusionVision::GetSettingDisplayCategory() const
{
	return SightTour::ErrorMessages::UnknownMappingName;
}

void USightTourSettingFusionVision::OnInitialized()
{
	DynamicDetails = FGetGameSettingsDetails::CreateLambda([this](ULocalPlayer&)
		{
			/*if (const FKeyMappingRow* Row = FindKeyMappingRow())
			{
				if (Row->HasAnyMappings())
				{
					return FText::Format(LOCTEXT("DynamicDetails_KeyboardInputAction", "Bindings for {0}"), Row->Mappings.begin()->GetDisplayName());
				}
			}*/
			return FText::GetEmpty();
		});

	Super::OnInitialized();
}
