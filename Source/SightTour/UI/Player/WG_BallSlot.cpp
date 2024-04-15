// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/WG_BallSlot.h"
#include "Components/TextBlock.h"
#include "Pickup/Equipment/EquipmentManagerComponent.h"

void UWG_BallSlot::SetBallContent(FString NewContent)
{
	if (T_BallContent)
	{
		T_BallContent->SetVisibility(ESlateVisibility::Visible);
		T_BallContent->SetText(FText::FromString(NewContent));
	}
}

void UWG_BallSlot::ClearBallContent()
{
	if (T_BallContent)
	{
		T_BallContent->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWG_BallSlot::NativeConstruct()
{
	Super::NativeConstruct();
}
