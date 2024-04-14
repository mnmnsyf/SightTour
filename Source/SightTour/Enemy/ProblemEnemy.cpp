// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ProblemEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Level/ProblemGame/ProblemDefines.h"
#include "Level/ProblemGame/Problem/Problem_Formula.h"
#include "UI/Enemy/WG_EnemyProblem.h"
#include "Components/WidgetComponent.h"
#include "../SightTourCharacter.h"

AProblemEnemy::AProblemEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	QuestionWidget = CreateDefaultSubobject<UWidgetComponent>("QuestionWidget");
	QuestionWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AProblemEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitProblemConfig();

	FTimerDelegate NextTimer;
	NextTimer.BindUObject(this, &AProblemEnemy::UpdateNextQuestion);
	GetWorldTimerManager().SetTimerForNextTick(NextTimer);
}

void AProblemEnemy::InitProblemConfig()
{
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
}

void AProblemEnemy::UpdateQuestion(FString AnswerStr)
{
	if (!CurrentProblem.IsValid())
	{
		UpdateNextQuestion();
	}

	if (CurrentProblem.IsValid())
	{
		if (FProblem_Formula* Problem = CurrentProblem.GetMutablePtr<FProblem_Formula>())
		{
			//����
			Problem->FillProblem(AnswerStr);

			//�жϴ��Ƿ�ش���ȷ
			bool bAnswerRight = Problem->CheckAllAnswer();

			//�ش����
			if (!bAnswerRight && !Problem->FillProblem(FString()))
			{
				//�ӳٸ�����һ�������UI
				FTimerHandle QuestionTimer;
				GetWorldTimerManager().SetTimer(QuestionTimer, this, &AProblemEnemy::UpdateNextQuestion, 0.1f, false, Problem->UpdateDelayTime);

				//�ش������ҿ�Ѫ
				if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
				{
					if (ASightTourCharacter* STCharacter = Cast<ASightTourCharacter>(Player))
					{
						STCharacter->ReduceHealth(Problem->DamageValue);
					}
				}
			}
			//�ش���ȷ
			else if (bAnswerRight)
			{
				//�ӳٸ�����һ�������UI
				FTimerHandle QuestionTimer;
				GetWorldTimerManager().SetTimer(QuestionTimer, this, &AProblemEnemy::UpdateNextQuestion, 0.1f, false, Problem->UpdateDelayTime);

				//�ش���ȷ���˿�Ѫ
				ReduceHealth(Problem->DamageValue);
			}


			//����UI
			UpdateQuestionUI(Problem->GetUIShowList());
		}
	}
}

void AProblemEnemy::UpdateNextQuestion()
{
	if (Problems.Num())
	{
		int32 ProblemsIndex = UKismetMathLibrary::RandomIntegerInRange(0, Problems.Num() - 1);
		CurrentProblem = Problems[ProblemsIndex];

		const FProblem_Formula* CurrentProblemPtr = CurrentProblem.GetMutablePtr<FProblem_Formula>();
		for (auto It = Problems.CreateIterator(); It; ++It)
		{
			if (const FProblem_Formula* Problem = It->GetPtr<FProblem_Formula>())
			{
				if (Problem->ProblemContent == CurrentProblemPtr->ProblemContent)
				{
					It.RemoveCurrent();
				}
			}
		}
	}
	else
	{
		InitProblemConfig();
	}

	if (CurrentProblem.IsValid())
	{
		if (FProblem_Formula* Problem = CurrentProblem.GetMutablePtr<FProblem_Formula>())
		{
			UpdateQuestionUI(Problem->GetUIShowList());
		}
	}
}

void AProblemEnemy::UpdateQuestionUI(const TArray<FString>& ShowList)
{
	//����UI
	if (UUserWidget* Widget = QuestionWidget->GetWidget())
	{
		if (UWG_EnemyProblem* WG_EnemyProblem = Cast<UWG_EnemyProblem>(Widget))
		{
			WG_EnemyProblem->UpdateProblemContext(ShowList);
			WG_EnemyProblem->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
