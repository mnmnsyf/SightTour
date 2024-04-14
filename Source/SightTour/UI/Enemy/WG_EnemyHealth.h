// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "WG_EnemyHealth.generated.h"

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API UWG_EnemyHealth : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* I_TextBlank;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* T_ProblemText;

public:
	void SetText(FString ProblemText);
};
