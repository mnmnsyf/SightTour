// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyCharacter.h"
#include "InstancedStruct.h"
#include "ProblemEnemy.generated.h"

struct FProblemBase;
class UWidgetComponent;

DECLARE_DELEGATE_OneParam(FUpdateProblemDelegate, FProblemBase);

UCLASS()
class SIGHTTOUR_API AProblemEnemy : public AEnemyCharacter
{
	GENERATED_UCLASS_BODY()

protected:
	void BeginPlay() override;

public:
	//------------------------------ 内部初始化接口 --------------------------------------------
	bool InitParam(FName EnemyID) override;

	void InitProblemConfig();

public:
	//------------------------------ 问题流程接口 --------------------------------------------
	void UpdateQuestion(FString AnswerStr);

	void UpdateNextQuestion();

	void UpdateQuestionUI(const TArray<FString>& ShowList);

public:
	FUpdateProblemDelegate UpdateProblemDelegate;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Category = "问题", DisplayName = "问题配置", RowType = "/Script/SightTour.ProblemConfig"))
	TObjectPtr<UDataTable> ProblemConfigTable;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (Category = "问题", AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> QuestionWidget = nullptr;

	TArray<FInstancedStruct> Problems;

	FInstancedStruct CurrentProblem;

};
