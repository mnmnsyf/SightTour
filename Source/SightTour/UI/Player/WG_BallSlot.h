// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "WG_BallSlot.generated.h"

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API UWG_BallSlot : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* T_BallContent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* I_BallIcon;

protected:
	void NativeConstruct() override;

	

public:
	void SetBallContent(FString NewContent, class UTexture2D* NewTexture);

	void ClearBallContent();
};