// Copyright Epic Games, Inc. All Rights Reserved.

#include "SightTourSettingScreen.h"

#include "Input/CommonUIInputTypes.h"
#include "Settings/SightTourGameSettingRegistry.h"
#include "Engine/LocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SightTourSettingScreen)

class UGameSettingRegistry;

void USightTourSettingScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &USightTourSettingScreen::HandleBackAction)));
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true, FSimpleDelegate::CreateUObject(this, &USightTourSettingScreen::HandleApplyAction)));
	CancelChangesHandle = RegisterUIActionBinding(FBindUIActionArgs(CancelChangesInputActionData, true, FSimpleDelegate::CreateUObject(this, &USightTourSettingScreen::HandleCancelChangesAction)));
}

UGameSettingRegistry* USightTourSettingScreen::CreateRegistry()
{
	USightTourGameSettingRegistry* NewRegistry = NewObject<USightTourGameSettingRegistry>();

	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		NewRegistry->Initialize(LocalPlayer);
	}

	return NewRegistry;
}

void USightTourSettingScreen::HandleBackAction()
{
	if (AttemptToPopNavigation())
	{
		return;
	}

	ApplyChanges();

	DeactivateWidget();
}

void USightTourSettingScreen::HandleApplyAction()
{
	ApplyChanges();
}

void USightTourSettingScreen::HandleCancelChangesAction()
{
	CancelChanges();
}

void USightTourSettingScreen::OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty)
{
	if (bSettingsDirty)
	{
		if (!GetActionBindings().Contains(ApplyHandle))
		{
			AddActionBinding(ApplyHandle);
		}
		if (!GetActionBindings().Contains(CancelChangesHandle))
		{
			AddActionBinding(CancelChangesHandle);
		}
	}
	else
	{
		RemoveActionBinding(ApplyHandle);
		RemoveActionBinding(CancelChangesHandle);
	}
}
