// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ProblemBase.generated.h"
struct FInstancedStruct;

USTRUCT(BlueprintType)
struct FProblemConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, DisplayName = "敌人ID")
	FName EnemyID;

	UPROPERTY(EditAnywhere, DisplayName = "问题", Meta = (ExcludeBaseStruct, BaseStruct = "/Script/SightTour.ProblemBase"))
	TArray<FInstancedStruct> Problems;
};

USTRUCT()
struct SIGHTTOUR_API FProblemBase
{
	GENERATED_BODY()

public:
	FProblemBase() = default;

	virtual ~FProblemBase() {}

	/** 初始化问题， 处理ProblemContent*/
	virtual void InitProblem() {}

	/** 处理完问题后返回给UI的问题分割数组(按从左到右顺序) */
	virtual TArray<FString> GetUIShowList() { return TArray<FString>(); }

	/** 问题类型名 */
	virtual FName GetProblemTypeName() { return FName(); }

	/** 填充当前第一个还没填写答案的空格，返回是否填充完所有空格 */
	virtual bool FillProblem(FString AnswerStr) { return false; }

	/** 检查问题是否成功回答 */
	virtual bool CheckAllAnswer() { return false; }

	/** 重置问题 */
	virtual void ResetProblem() {}

public:
	UPROPERTY(EditAnywhere, DisplayName = "问题内容")
	FString ProblemContent;

	UPROPERTY(EditAnywhere, DisplayName = "答案分割字符")
	FString SplitChar = FString("?");

	UPROPERTY(EditAnywhere, DisplayName = "问题持续时长")
	float DurationTime = 10.f;

	UPROPERTY(EditAnywhere, DisplayName = "更新问题延迟时长")
	float UpdateDelayTime = 5.f;

	UPROPERTY(EditAnywhere, DisplayName = "伤害值")
	float DamageValue =	10.f;

	UPROPERTY(EditAnywhere, DisplayName = "CorrectAnswer", Meta = (ExcludeBaseStruct, BaseStruct = "/Script/SightTour.PickableItem"))
	TArray<FInstancedStruct> CorrectAnswer;

	UPROPERTY(EditAnywhere, DisplayName = "WrongAnswer", Meta = (ExcludeBaseStruct, BaseStruct = "/Script/SightTour.PickableItem"))
	TArray<FInstancedStruct> WrongAnswer;
};
