// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "WG_EnemyProblem.generated.h"

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API UWG_EnemyDialogue : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* CanvasPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HB_ProblemBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* I_ProblemBackground;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* I_AnswerBlank;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* T_Dialogue;

public:
	void UpdateProblemContext(TArray<FString> Context);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (Categories = "Problem"))
	TSoftClassPtr<class UImage> AnswerBlankImageClass;
};
