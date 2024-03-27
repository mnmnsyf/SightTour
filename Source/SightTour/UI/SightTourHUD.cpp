// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SightTourHUD.h"
#include "PrimaryGameLayout.h"
#include "Engine/LocalPlayer.h"

ASightTourHUD::ASightTourHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

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

void ASightTourHUD::CreateLayoutWidget(ULocalPlayer* LocalPlayer)
{
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass();
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UPrimaryGameLayout* NewLayoutObject = CreateWidget<UPrimaryGameLayout>(PlayerController, LayoutWidgetClass);

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

TSubclassOf<UPrimaryGameLayout> ASightTourHUD::GetLayoutWidgetClass()
{
	return PrimaryLayoutClass.LoadSynchronous();
}

void ASightTourHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ASightTourHUD::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
{
	CreateLayoutWidget(LocalPlayer);
}
