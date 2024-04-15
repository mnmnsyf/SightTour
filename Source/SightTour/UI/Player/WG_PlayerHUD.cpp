// Fill out your copyright notice in the Description page of Project Settings.

#include "WG_PlayerHUD.h"
#include "Components/ProgressBar.h"

void UWG_PlayerHUD::SetHealth(float HealthPercent)
{
	if (PB_Health)
	{
		PB_Health->SetPercent(HealthPercent);
	}
}
