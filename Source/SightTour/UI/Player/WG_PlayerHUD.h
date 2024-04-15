// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "WG_PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API UWG_PlayerHUD : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* PB_Health;

public:
	void SetHealth(float CurrentHealth);
};
