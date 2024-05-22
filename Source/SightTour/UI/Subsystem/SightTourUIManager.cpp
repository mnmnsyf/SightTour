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

void USightTourUIManager::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer))
	{
		CreatePrimaryLayout(LocalPlayer);
	}
}

void USightTourUIManager::PushStreamedContentToLayer_ForPlayer(ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSoftClassPtr<UCommonActivatableWidget> WidgetClass)
{
	if (!ensure(LocalPlayer) || !ensure(!WidgetClass.IsNull()))
	{
		return;
	}

	const bool bSuspendInputUntilComplete = true;

	if (PrimaryLayout == nullptr)
	{
		CreatePrimaryLayout(LocalPlayer);
	}
	PrimaryLayout->PushWidgetToLayerStackAsync(LayerName, bSuspendInputUntilComplete, WidgetClass);
}

UCommonActivatableWidget* USightTourUIManager::PushContentToLayer_ForPlayer(ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	if (!ensure(LocalPlayer) || !ensure(WidgetClass != nullptr))
	{
		return nullptr;
	}

	if (PrimaryLayout == nullptr)
	{
		CreatePrimaryLayout(LocalPlayer);
	}
	return PrimaryLayout->PushWidgetToLayerStack(LayerName, WidgetClass);
}

void USightTourUIManager::FindAndRemoveWidgetFromLayer(UCommonActivatableWidget* ActivatableWidget)
{
	if (PrimaryLayout)
	{
		PrimaryLayout->FindAndRemoveWidgetFromLayer(ActivatableWidget);
	}
}

void USightTourUIManager::CreatePrimaryLayout(ULocalPlayer* LocalPlayer)
{
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass();
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UPrimaryGameLayout* NewLayoutObject = CreateWidget<UPrimaryGameLayout>(PlayerController, LayoutWidgetClass);

			PrimaryLayout = NewLayoutObject;

			//添加到视口
			AddPrimaryGameLayoutToViewport(LocalPlayer, NewLayoutObject);
			//添加Layout栈
			PushHUDLayerToPrimaryLayoutStack();
		}
	}
}

TSubclassOf<UPrimaryGameLayout> USightTourUIManager::GetLayoutWidgetClass()
{
	TSoftClassPtr<UPrimaryGameLayout> PrimaryLayoutClass = USightTourGameInstance::Get()->PrimaryLayoutClass;
	check(!PrimaryLayoutClass.IsNull());
	return PrimaryLayoutClass.LoadSynchronous();
}

void USightTourUIManager::AddPrimaryGameLayoutToViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	Layout->AddToPlayerScreen(1000);
}

void USightTourUIManager::PushHUDLayerToPrimaryLayoutStack()
{
	if (ensure(PrimaryLayout))
	{
		for (const FSightTourHUDLayoutRequest& Entry : USightTourGameInstance::Get()->HUDLayout)
		{
			if (ensure(!Entry.LayoutClass.IsNull()))
			{
				if (TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Entry.LayoutClass.LoadSynchronous())
				{
					PrimaryLayout->PushWidgetToLayerStack(Entry.LayerID, ConcreteWidgetClass);
				}
			}
		}
	}
}
