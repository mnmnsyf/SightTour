// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SightTourHUD.h"
#include "PrimaryGameLayout.h"
#include "Engine/LocalPlayer.h"
#include "CommonUIExtensions.h"
#include "PrimaryGameLayout.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ASightTourHUD::ASightTourHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void ASightTourHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ASightTourHUD::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
{
	CreateLayoutWidget(LocalPlayer);

	PushWidgetToLayerStack();
}

void ASightTourHUD::CreateLayoutWidget(ULocalPlayer* LocalPlayer)
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

void ASightTourHUD::AddLayoutToViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	Layout->AddToPlayerScreen(1000);

#if WITH_EDITOR
	if (GIsEditor && LocalPlayer->IsPrimaryPlayer())
	{
		// So our controller will work in PIE without needing to click in the viewport
		FSlateApplication::Get().SetUserFocusToGameViewport(0);
	}
#endif
}

TSubclassOf<UPrimaryGameLayout> ASightTourHUD::GetLayoutWidgetClass()
{
	check(!PrimaryLayoutClass.IsNull());
	return PrimaryLayoutClass.LoadSynchronous();
}

void ASightTourHUD::PushWidgetToLayerStack()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetOwningPlayerController()->Player))
	{
		for (const FLyraHUDLayoutRequest& Entry : HUDLayout)
		{
			check(!Entry.LayoutClass.IsNull());

			if (TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Entry.LayoutClass.LoadSynchronous())
			{
				 PrimaryLayout->PushWidgetToLayerStack(Entry.LayerID, ConcreteWidgetClass);
			}
		}
	}
}

void ASightTourHUD::PushStreamedContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSoftClassPtr<UCommonActivatableWidget> WidgetClass)
{
	if (!ensure(LocalPlayer) || !ensure(!WidgetClass.IsNull()))
	{
		return;
	}

	const bool bSuspendInputUntilComplete = true;
	PrimaryLayout->PushWidgetToLayerStackAsync(LayerName, bSuspendInputUntilComplete, WidgetClass);
}
