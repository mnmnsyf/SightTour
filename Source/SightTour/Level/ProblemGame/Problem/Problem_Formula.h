// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ProblemBase.h"
#include "Problem_Formula.generated.h"

USTRUCT()
struct SIGHTTOUR_API FProblem_Formula : public FProblemBase
{
	GENERATED_BODY()
	
	virtual void InitProblem() override;

	virtual TArray<FString> GetUIShowList() override;

	virtual FName GetProblemTypeName() override;

	virtual bool FillProblem(FString AnswerStr) override;

	virtual bool CheckAllAnswer() override;

	virtual void ResetProblem() override;

	/** 表达式求值 */
	int32 ExpressionEvaluation(const TArray<FString>& Expression);

	/** 等号左边的算式 */
	TArray<FString> LeftExpression;

	/** 等号右边的算式 */
	TArray<FString> RightExpression;

	/** 表达式求值 */
	static TArray<TCHAR> Operator;

	bool bInit = false;
};



USTRUCT()
struct SIGHTTOUR_API FProblem_Text : public FProblemBase
{
	GENERATED_BODY()

	virtual void InitProblem() override;

	virtual TArray<FString> GetUIShowList() override;

	virtual bool FillProblem(FString AnswerStr) override;

	virtual bool CheckAllAnswer() override;

	virtual void ResetProblem() override;

	/** 挖掉答案的问题 */
	TArray<FString> ProblemList;

	/** 挖空对应的答案 */
	TMap<int32, FString> AnswerList;

	bool bInit = false;

	bool bAnswerQustion = false;
};
