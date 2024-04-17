// Fill out your copyright notice in the Description page of Project Settings.

#include "WG_PlayerHUD.h"
#include "Components/ProgressBar.h"
#include "WG_InventoryBar.h"

void UWG_PlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWG_PlayerHUD::SetHealth(float HealthPercent)
{
	if (PB_Health)
	{
		PB_Health->SetVisibility(ESlateVisibility::Visible);
		PB_Health->SetPercent(HealthPercent);
	}
}

//class UWG_InventoryBar* UWG_PlayerHUD::LoadInventoryBar()
//{
//	if (ensure(!InventoryBarClass.IsNull()))
//	{
//		return nullptr;
//	}
//
//	TSubclassOf<UWG_InventoryBar> WidgetClass = InventoryBarClass.LoadSynchronous();
//
//	if (ensure(WidgetClass && !WidgetClass->HasAnyClassFlags(CLASS_Abstract)))
//	{
//		UWG_InventoryBar* NewObject = CreateWidget<UWG_InventoryBar>(GetOwningLocalPlayer()->GetPlayerController(GetWorld()), WidgetClass);
//
//		return NewObject;
//	}
//
//	return nullptr;
//}
