// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ProblemEnemy.h"
#include "Level/ProblemGame/ProblemDefines.h"
#include "Level/ProblemGame/Problem/Problem_Formula.h"
#include "InstancedStruct.h"
#include "Components/WidgetComponent.h"


AProblemEnemy::AProblemEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	QuestionWidget = CreateDefaultSubobject<UWidgetComponent>("QuestionWidget");
	QuestionWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AProblemEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (!ProblemConfigRowHandle.IsNull())
	{
		FString ContextStr;
		if (FProblemConfig* ProblemConfig = ProblemConfigRowHandle.GetRow<FProblemConfig>(ContextStr))
		{
			Problems = ProblemConfig->Problems;

			if (Problems.Num())
			{
				CurrentProblem = Problems[0];
			}
		}
	}

	UpdateQuestion(FString());
}

void AProblemEnemy::UpdateQuestion(FString AnswerStr)
{
	if (CurrentProblem.IsValid())
	{
		if (FProblem_Formula* Problem = CurrentProblem.GetMutablePtr<FProblem_Formula>())
		{
			Problem->FillProblem(AnswerStr);
			if (Problem->CheckAllAnswer())
			{
				//Update next problem
				;
			}

			TArray<FString> ShowList = Problem->GetUIShowList();
		}
	}
}
