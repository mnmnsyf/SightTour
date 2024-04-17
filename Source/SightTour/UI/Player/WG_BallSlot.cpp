// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/WG_BallSlot.h"
#include "Components/TextBlock.h"
#include "Pickup/Equipment/EquipmentManagerComponent.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UWG_BallSlot::SetBallContent(FString NewContent, UTexture2D* NewTexture)
{
	if (T_BallContent)
	{
		T_BallContent->SetVisibility(ESlateVisibility::Visible);
		T_BallContent->SetText(FText::FromString(NewContent));
	}
	if (I_BallIcon && NewTexture)
	{
		I_BallIcon->SetVisibility(ESlateVisibility::Visible);
		I_BallIcon->SetBrushFromTexture(NewTexture);
	}
}

void UWG_BallSlot::ClearBallContent()
{
	if (T_BallContent)
	{
		T_BallContent->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (I_BallIcon)
	{
		I_BallIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWG_BallSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (I_BallIcon)
	{
		I_BallIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}
