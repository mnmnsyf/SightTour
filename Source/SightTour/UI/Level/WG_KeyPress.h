// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "WG_KeyPress.generated.h"

/**
 * 玩家HUD，显示玩家的生命值、物品栏等信息
 */
UCLASS()
class SIGHTTOUR_API UWG_KeyPress : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* T_KeyName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* I_Image;

public:
	void NativeConstruct() override;

	UFUNCTION()
	void SetContent(FText NewKeyName, UMaterialInstance* NewImage);
};
