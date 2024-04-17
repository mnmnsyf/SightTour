// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "WG_ItemName.generated.h"

/**
 * 物品描述UI
 */
UCLASS()
class SIGHTTOUR_API UWG_ItemName : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* T_ItemInfo;

public:
	void SetText(FString ProblemText);
};
