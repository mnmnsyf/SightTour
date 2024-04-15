// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/WG_InventoryBar.h"
#include "Components/ProgressBar.h"
#include "Pickup/Equipment/EquipmentManagerComponent.h"
#include "WG_BallSlot.h"
#include "Components/HorizontalBox.h"


void UWG_InventoryBar::NativeConstruct()
{
	Super::NativeConstruct();

	UEquipmentManagerComponent* EquipmentManager = UEquipmentManagerComponent::Get(GetOwningPlayerPawn());
	if (EquipmentManager)
	{
		EquipmentManager->ChangeSlotDelegate.AddDynamic(this, &UWG_InventoryBar::BP_OnBallSlotChange);
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
}

void UWG_InventoryBar::OnBallPickup(bool bPickup, int32 InSlotIndex, FString BallContent)
{
	check(InventorySlot.IsValidIndex(InSlotIndex));
	bPickup ? InventorySlot[InSlotIndex]->SetBallContent(BallContent)
			: InventorySlot[InSlotIndex]->ClearBallContent();
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
