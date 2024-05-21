// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/WG_InventoryBar.h"
#include "Components/ProgressBar.h"
#include "Pickup/Equipment/EquipmentManagerComponent.h"
#include "WG_BallSlot.h"
#include "Components/HorizontalBox.h"
#include "Engine/Texture2D.h"


void UWG_InventoryBar::NativeConstruct()
{
	Super::NativeConstruct();

	UEquipmentManagerComponent* EquipmentManager = UEquipmentManagerComponent::Get(GetOwningPlayerPawn());
	if (EquipmentManager)
	{
		EquipmentManager->ChangeSlotDelegate.AddDynamic(this, &UWG_InventoryBar::OnBallSlotChange);
		EquipmentManager->PickupBallDelegate.AddDynamic(this, &UWG_InventoryBar::OnBallPickup);
	}

	HB_InventoryBox->ClearChildren();
	for (int32 Index = 0; Index< 5; ++Index)
	{
		UWG_BallSlot* BallSlot = LoadBallSlot();
		if (BallSlot)
		{
			BallSlot->SetVisibility(ESlateVisibility::Visible);
			InventorySlot.Emplace(BallSlot);
			HB_InventoryBox->AddChildToHorizontalBox(BallSlot);
		}
	}

	if (InventorySlot.Num() > 0)
	{
		InventorySlot[0]->OnSelect(true);
	}
}

void UWG_InventoryBar::OnBallPickup(bool bPickup, int32 InSlotIndex, FString BallContent, FName BallType)
{
	check(InventorySlot.IsValidIndex(InSlotIndex));
	if (UTexture2D** BallTexture = BallTypeIconMap.Find(BallType))
	{
		bPickup ? InventorySlot[InSlotIndex]->SetBallContent(BallContent, *BallTexture)
			: InventorySlot[InSlotIndex]->ClearBallContent();
	}
	else
	{
		ensure(false);
	}
}

void UWG_InventoryBar::OnBallSlotChange(int32 LastIndex, int32 NextIndex)
{
	check(InventorySlot.IsValidIndex(LastIndex));
	InventorySlot[LastIndex]->OnSelect(false);
	check(InventorySlot.IsValidIndex(NextIndex));
	InventorySlot[NextIndex]->OnSelect(true);
}

UWG_BallSlot* UWG_InventoryBar::LoadBallSlot()
{
	check(!BallSlotClass.IsNull());
	TSubclassOf<UWG_BallSlot> WidgetClass = BallSlotClass.LoadSynchronous();

	if (ensure(WidgetClass && !WidgetClass->HasAnyClassFlags(CLASS_Abstract)))
	{
		UWG_BallSlot* NewObject = CreateWidget<UWG_BallSlot>(GetOwningLocalPlayer()->GetPlayerController(GetWorld()), WidgetClass);

		return NewObject;
	}

	return nullptr;
}
