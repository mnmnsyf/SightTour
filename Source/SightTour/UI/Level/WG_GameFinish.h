// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "WG_GameFinish.generated.h"

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API UWG_GameFinish : public UCommonActivatableWidget
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
	void SetContent(FText NewKeyName, class UTexture2D* NewImage);
};
