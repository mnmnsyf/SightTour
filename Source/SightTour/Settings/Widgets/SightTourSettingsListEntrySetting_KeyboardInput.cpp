// Copyright Epic Games, Inc. All Rights Reserved.

#include "Settings/Widgets/SightTourSettingsListEntrySetting_KeyboardInput.h"

#include "CommonUIExtensions.h"
#include "NativeGameplayTags.h"
#include "Settings/CustomSettings/SightTourSettingKeyboardInput.h"
#include "UI/Foundation/LyraButtonBase.h"
#include "Widgets/Misc/GameSettingPressAnyKey.h"
#include "Widgets/Misc/KeyAlreadyBoundWarning.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Engine/LocalPlayer.h"
#include "../UI/SightTourHUD.h"

#define LOCTEXT_NAMESPACE "SightTourSettings"

UE_DEFINE_GAMEPLAY_TAG_STATIC(PressAnyKeyLayer, "UI.Layer.Modal");

void USightTourSettingsListEntrySetting_KeyboardInput::SetSetting(UGameSetting* InSetting)
{
	KeyboardInputSetting = CastChecked<USightTourSettingKeyboardInput>(InSetting);

	Super::SetSetting(InSetting);

	Refresh();
}

void USightTourSettingsListEntrySetting_KeyboardInput::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Button_PrimaryKey->OnClicked().AddUObject(this, &ThisClass::HandlePrimaryKeyClicked);
	Button_SecondaryKey->OnClicked().AddUObject(this, &ThisClass::HandleSecondaryKeyClicked);
	Button_Clear->OnClicked().AddUObject(this, &ThisClass::HandleClearClicked);
	Button_ResetToDefault->OnClicked().AddUObject(this, &ThisClass::HandleResetToDefaultClicked);	
}

void USightTourSettingsListEntrySetting_KeyboardInput::HandlePrimaryKeyClicked()
{
	/*UGameSettingPressAnyKey* PressAnyKeyPanel = CastChecked<UGameSettingPressAnyKey>(
		UCommonUIExtensions::PushContentToLayer_ForPlayer(GetOwningLocalPlayer(), PressAnyKeyLayer, PressAnyKeyPanelClass));*/

	//TODO 临时写法
	ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	APlayerController* PlayerController = GetOwningLocalPlayer()->GetPlayerController(GetWorld());
	ASightTourHUD* HUD = Cast<ASightTourHUD>(PlayerController->GetHUD());

	UGameSettingPressAnyKey* PressAnyKeyPanel = CastChecked<UGameSettingPressAnyKey>(HUD->PushContentToLayer_ForPlayer(LocalPlayer, PressAnyKeyLayer, PressAnyKeyPanelClass));

	PressAnyKeyPanel->OnKeySelected.AddUObject(this, &ThisClass::HandlePrimaryKeySelected, PressAnyKeyPanel);
	PressAnyKeyPanel->OnKeySelectionCanceled.AddUObject(this, &ThisClass::HandleKeySelectionCanceled, PressAnyKeyPanel);
}

void USightTourSettingsListEntrySetting_KeyboardInput::HandleSecondaryKeyClicked()
{
	//TODO 临时写法
	ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	APlayerController* PlayerController = GetOwningLocalPlayer()->GetPlayerController(GetWorld());
	ASightTourHUD* HUD = Cast<ASightTourHUD>(PlayerController->GetHUD());

	UGameSettingPressAnyKey* PressAnyKeyPanel = CastChecked<UGameSettingPressAnyKey>(HUD->PushContentToLayer_ForPlayer(LocalPlayer, PressAnyKeyLayer, PressAnyKeyPanelClass));


	PressAnyKeyPanel->OnKeySelected.AddUObject(this, &ThisClass::HandleSecondaryKeySelected, PressAnyKeyPanel);
	PressAnyKeyPanel->OnKeySelectionCanceled.AddUObject(this, &ThisClass::HandleKeySelectionCanceled, PressAnyKeyPanel);
}

void USightTourSettingsListEntrySetting_KeyboardInput::HandlePrimaryKeySelected(FKey InKey, UGameSettingPressAnyKey* PressAnyKeyPanel)
{
	PressAnyKeyPanel->OnKeySelected.RemoveAll(this);
	ChangeBinding(0, InKey);
}

void USightTourSettingsListEntrySetting_KeyboardInput::HandleSecondaryKeySelected(FKey InKey, UGameSettingPressAnyKey* PressAnyKeyPanel)
{
	PressAnyKeyPanel->OnKeySelected.RemoveAll(this);
	ChangeBinding(1, InKey);
}

void USightTourSettingsListEntrySetting_KeyboardInput::HandlePrimaryDuplicateKeySelected(FKey InKey, UKeyAlreadyBoundWarning* DuplicateKeyPressAnyKeyPanel) const
{
	DuplicateKeyPressAnyKeyPanel->OnKeySelected.RemoveAll(this);
	KeyboardInputSetting->ChangeBinding(0, OriginalKeyToBind);
}

void USightTourSettingsListEntrySetting_KeyboardInput::HandleSecondaryDuplicateKeySelected(FKey InKey, UKeyAlreadyBoundWarning* DuplicateKeyPressAnyKeyPanel) const
{
	DuplicateKeyPressAnyKeyPanel->OnKeySelected.RemoveAll(this);
	KeyboardInputSetting->ChangeBinding(1, OriginalKeyToBind);
}

void USightTourSettingsListEntrySetting_KeyboardInput::ChangeBinding(int32 InKeyBindSlot, FKey InKey)
{
	OriginalKeyToBind = InKey;
	TArray<FName> ActionsForKey;
	KeyboardInputSetting->GetAllMappedActionsFromKey(InKeyBindSlot, InKey, ActionsForKey);
	if (!ActionsForKey.IsEmpty())
	{
		//TODO 后续用UIManager统一管理
		ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
		APlayerController* PlayerController = GetOwningLocalPlayer()->GetPlayerController(GetWorld());
		ASightTourHUD* HUD = Cast<ASightTourHUD>(PlayerController->GetHUD());

		UKeyAlreadyBoundWarning* KeyAlreadyBoundWarning = CastChecked<UKeyAlreadyBoundWarning>(HUD->PushContentToLayer_ForPlayer(LocalPlayer, PressAnyKeyLayer, KeyAlreadyBoundWarningPanelClass));


		FString ActionNames;
		for (FName ActionName : ActionsForKey)
		{
			ActionNames += ActionName.ToString() += ", ";
		}

		FFormatNamedArguments Args;
		Args.Add(TEXT("InKey"), InKey.GetDisplayName());
		Args.Add(TEXT("ActionNames"), FText::FromString(ActionNames));

		KeyAlreadyBoundWarning->SetWarningText(FText::Format(LOCTEXT("WarningText", "{InKey} is already bound to {ActionNames} are you sure you want to rebind it?"), Args));
		KeyAlreadyBoundWarning->SetCancelText(FText::Format(LOCTEXT("CancelText", "Press escape to cancel, or press {InKey} again to confirm rebinding."), Args));

		if (InKeyBindSlot == 1)
		{
			KeyAlreadyBoundWarning->OnKeySelected.AddUObject(this, &ThisClass::HandleSecondaryDuplicateKeySelected, KeyAlreadyBoundWarning);
		}
		else
		{
			KeyAlreadyBoundWarning->OnKeySelected.AddUObject(this, &ThisClass::HandlePrimaryDuplicateKeySelected, KeyAlreadyBoundWarning);
		}
		KeyAlreadyBoundWarning->OnKeySelectionCanceled.AddUObject(this, &ThisClass::HandleKeySelectionCanceled, KeyAlreadyBoundWarning);
	}
	else
	{
		KeyboardInputSetting->ChangeBinding(InKeyBindSlot, InKey);
	}
}

void USightTourSettingsListEntrySetting_KeyboardInput::HandleKeySelectionCanceled(UGameSettingPressAnyKey* PressAnyKeyPanel)
{
	PressAnyKeyPanel->OnKeySelectionCanceled.RemoveAll(this);
}

void USightTourSettingsListEntrySetting_KeyboardInput::HandleKeySelectionCanceled(UKeyAlreadyBoundWarning* PressAnyKeyPanel)
{
	PressAnyKeyPanel->OnKeySelectionCanceled.RemoveAll(this);
}

void USightTourSettingsListEntrySetting_KeyboardInput::HandleClearClicked()
{
	KeyboardInputSetting->ChangeBinding(0, EKeys::Invalid);
	KeyboardInputSetting->ChangeBinding(1, EKeys::Invalid);
}

void USightTourSettingsListEntrySetting_KeyboardInput::HandleResetToDefaultClicked()
{
	KeyboardInputSetting->ResetToDefault();
}

void USightTourSettingsListEntrySetting_KeyboardInput::OnSettingChanged()
{
	Refresh();
}

void USightTourSettingsListEntrySetting_KeyboardInput::Refresh()
{
	if (ensure(KeyboardInputSetting))
	{
		Button_PrimaryKey->SetButtonText(KeyboardInputSetting->GetKeyTextFromSlot(EPlayerMappableKeySlot::First));
		Button_SecondaryKey->SetButtonText(KeyboardInputSetting->GetKeyTextFromSlot(EPlayerMappableKeySlot::Second));
		
		// Only display the reset to default button if a mapping is customized
		if (ensure(Button_ResetToDefault))
		{
			if (KeyboardInputSetting->IsMappingCustomized())
			{
				Button_ResetToDefault->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				Button_ResetToDefault->SetVisibility(ESlateVisibility::Hidden);
			}
		}		
	}
}

void USightTourSettingsListEntrySetting_KeyboardInput::NativeOnEntryReleased()
{
	Super::NativeOnEntryReleased();

	KeyboardInputSetting = nullptr;
}

#undef LOCTEXT_NAMESPACE
