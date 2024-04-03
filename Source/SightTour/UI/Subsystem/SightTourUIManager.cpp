// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Subsystem/SightTourUIManager.h"
#include "../System/SightTourGameInstance.h"

USightTourUIManager::USightTourUIManager()
{

}

USightTourUIManager* USightTourUIManager::Get()
{
	return USightTourGameInstance::Get()->GetSubsystem<USightTourUIManager>();
}

void USightTourUIManager::PushStreamedContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSoftClassPtr<UCommonActivatableWidget> WidgetClass)
{
	if (!ensure(LocalPlayer) || !ensure(!WidgetClass.IsNull()))
	{
		return;
	}

	const bool bSuspendInputUntilComplete = true;

	check(PrimaryLayout);
	PrimaryLayout->PushWidgetToLayerStackAsync(LayerName, bSuspendInputUntilComplete, WidgetClass);
}

UCommonActivatableWidget* USightTourUIManager::PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	if (!ensure(LocalPlayer) || !ensure(WidgetClass != nullptr))
	{
		return nullptr;
	}

	check(PrimaryLayout);
	return PrimaryLayout->PushWidgetToLayerStack(LayerName, WidgetClass);
}

void USightTourUIManager::AddLayoutToViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	Layout->AddToPlayerScreen(1000);
}

void USightTourUIManager::CreateLayoutWidget(ULocalPlayer* LocalPlayer)
{
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass();
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UPrimaryGameLayout* NewLayoutObject = CreateWidget<UPrimaryGameLayout>(PlayerController, LayoutWidgetClass);

			PrimaryLayout = NewLayoutObject;

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

TSubclassOf<UPrimaryGameLayout> USightTourUIManager::GetLayoutWidgetClass()
{
	check(!PrimaryLayoutClass.IsNull());
	return PrimaryLayoutClass.LoadSynchronous();
}
