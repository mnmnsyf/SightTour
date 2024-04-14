// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyCharacter.h"
#include "InstancedStruct.h"
#include "ProblemEnemy.generated.h"

struct FProblemBase;
class UWidgetComponent;

/**
 * 
 */

UCLASS()
class SIGHTTOUR_API AProblemEnemy : public AEnemyCharacter
{
	GENERATED_UCLASS_BODY()

protected:
	void BeginPlay() override;

public:
	void UpdateQuestion(FString AnswerStr);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "问题配置", RowType = "/Script/SightTour.ProblemConfig"))
	FDataTableRowHandle ProblemConfigRowHandle;

private:
	TArray<FInstancedStruct> Problems;

	FInstancedStruct CurrentProblem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> QuestionWidget = nullptr;

/*
	+1 -2 *3 /4   *2
	+ 1 - 2 *2 / 4   +3
	+ 4 - 2 * 2 / 4 + 3
*/
};
