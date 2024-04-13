// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "WG_EnemyProblem.generated.h"

class UWG_ProblemText;

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API UWG_EnemyProblem : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* CP_CanvasPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HB_ProblemBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HB_ProblemTextBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* I_ProblemBackground;

public:
	void UpdateProblemContext(TArray<FString> Context);

private:
	UWG_ProblemText* LoadProblemText();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Problem", AllowAbstract))
	TSoftClassPtr<class UWG_ProblemText> ProblemTextClass;
};
