// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/WG_PlayerHealth.h"
#include "Components/ProgressBar.h"

void UWG_PlayerHealth::SetHealth(float HealthPercent)
{
	if (PB_Health)
	{
		PB_Health->SetPercent(HealthPercent);
	}
}
