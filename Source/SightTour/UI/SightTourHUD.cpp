// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SightTourHUD.h"
#include "PrimaryGameLayout.h"
#include "Engine/LocalPlayer.h"
#include "CommonUIExtensions.h"
#include "PrimaryGameLayout.h"

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

			PrimaryLayout = NewLayoutObject;

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

TSubclassOf<UPrimaryGameLayout> ASightTourHUD::GetLayoutWidgetClass()
{
	return PrimaryLayoutClass.LoadSynchronous();
}

//EDataValidationResult ASightTourHUD::IsDataValid(class FDataValidationContext& Context) const
//{
//	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);
//
//	{
//		int32 EntryIndex = 0;
//		for (const FLyraHUDLayoutRequest& Entry : HUDLayout)
//		{
//			if (Entry.LayoutClass.IsNull())
//			{
//				Result = EDataValidationResult::Invalid;
//				Context.AddError(FText::Format(LOCTEXT("LayoutHasNullClass", "Null WidgetClass at index {0} in Layout"), FText::AsNumber(EntryIndex)));
//			}
//
//			if (!Entry.LayerID.IsValid())
//			{
//				Result = EDataValidationResult::Invalid;
//				Context.AddError(FText::Format(LOCTEXT("LayoutHasNoTag", "LayerID is not set at index {0} in Widgets"), FText::AsNumber(EntryIndex)));
//			}
//
//			++EntryIndex;
//		}
//	}
//}

void ASightTourHUD::AddLayouts()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetOwningPlayerController()->Player))
	{
		for (const FLyraHUDLayoutRequest& Entry : HUDLayout)
		{
			check(!Entry.LayoutClass.IsNull());

			if (TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Entry.LayoutClass.Get())
			{
				//UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, Entry.LayerID, ConcreteWidgetClass);

				 PrimaryLayout->PushWidgetToLayerStack(Entry.LayerID, ConcreteWidgetClass);
			}
		}
	}
}

void ASightTourHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ASightTourHUD::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
{
	CreateLayoutWidget(LocalPlayer);

	AddLayouts();
}
