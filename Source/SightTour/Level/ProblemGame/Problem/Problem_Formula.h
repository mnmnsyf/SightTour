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


};
