// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ProblemEnemy.h"
#include "Level/ProblemGame/ProblemDefines.h"
#include "Level/ProblemGame/Problem/Problem_Formula.h"
#include "InstancedStruct.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Enemy/WG_EnemyProblem.h"

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

	FTimerDelegate NextTimer;
	NextTimer.BindUObject(this, &AProblemEnemy::UpdateQuestion, FString());
	GetWorldTimerManager().SetTimerForNextTick(NextTimer);
}

void AProblemEnemy::UpdateQuestion(FString AnswerStr)
{
	if (!CurrentProblem.IsValid() && Problems.Num())
	{
		int32 ProblemsIndex = UKismetMathLibrary::RandomIntegerInRange(0, Problems.Num() - 1);
		CurrentProblem = Problems[ProblemsIndex];
	}

	if (CurrentProblem.IsValid())
	{
		if (FProblem_Formula* Problem = CurrentProblem.GetMutablePtr<FProblem_Formula>())
		{
			Problem->FillProblem(AnswerStr);
			//if (Problem->CheckAllAnswer())
			//{
			//	//Update next problem
			//	;
			//}

			if (UUserWidget* Widget = QuestionWidget->GetWidget())
			{
				if (UWG_EnemyProblem* WG_EnemyProblem = Cast<UWG_EnemyProblem>(Widget))
				{
					TArray<FString> ShowList = Problem->GetUIShowList();
					WG_EnemyProblem->UpdateProblemContext(ShowList);
					WG_EnemyProblem->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
}