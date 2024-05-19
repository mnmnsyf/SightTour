// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ProblemEnemy.h"
#include "Level/ProblemGame/Problem/ProblemBase.h"
#include "Level/ProblemGame/Problem/Problem_Formula.h"
#include "UI/Enemy/WG_EnemyProblem.h"
#include "Player/SightTourCharacter.h"

#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
#include "System/SightTourGameInstance.h"

AProblemEnemy::AProblemEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	QuestionWidget = CreateDefaultSubobject<UWidgetComponent>("QuestionWidget");
	QuestionWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AProblemEnemy::BeginPlay()
{
	Super::BeginPlay();
}

bool AProblemEnemy::InitParam(FName EnemyID)
{
	Super::InitParam(EnemyID);

	//TODO:根据EnemyID初始化问题配置
	InitProblemConfig();

	FTimerDelegate NextTimer;
	NextTimer.BindUObject(this, &AProblemEnemy::UpdateNextQuestion);
	GetWorldTimerManager().SetTimerForNextTick(NextTimer);

	return true;
}


void AProblemEnemy::InitProblemConfig()
{
	USightTourGameInstance* GameInstance = USightTourGameInstance::Get();
	if (IsValid(ProblemConfigTable) && IsValid(GameInstance))
	{
		FString ContextStr;
		TArray<FProblemConfig*> ProblemConfigs;
		ProblemConfigTable->GetAllRows<FProblemConfig>(ContextStr, ProblemConfigs);

		if (ProblemConfigs.IsValidIndex(GameInstance->ProblemEnemyIndex))
		{
			if (FProblemConfig* ProblemConfig = ProblemConfigs[GameInstance->ProblemEnemyIndex])
			{
				Problems = ProblemConfig->Problems;

				if (Problems.Num())
				{
					CurrentProblem = Problems[0];
				}

				GameInstance->ProblemEnemyIndex++;
			}
		}
		else
		{
			// Game Success
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
		FProblemBase* Problem = CurrentProblem.GetMutablePtr<FProblemBase>();
		if (!Problem)
		{
			return;
		}

		//填充答案
		if (Problem->FillProblem(AnswerStr))
		{
			//判断答案是否回答正确
			bool bAnswerRight = Problem->CheckAllAnswer();

			//回答正确敌人扣血
			if (bAnswerRight)
			{
				ReduceHealth(Problem->DamageValue);
			}
			//回答错误玩家扣血
			else
			{
				if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
				{
					if (ASightTourCharacter* STCharacter = Cast<ASightTourCharacter>(Player))
					{
						STCharacter->ReduceHealth(Problem->DamageValue);
					}
				}
			}

			//延迟更新下一个问题和UI
			FTimerHandle QuestionTimer;
			GetWorldTimerManager().SetTimer(QuestionTimer, this, &AProblemEnemy::UpdateNextQuestion, Problem->UpdateDelayTime, false);

		}
		//更新UI
		UpdateQuestionUI(Problem->GetUIShowList());
	}
}

void AProblemEnemy::UpdateNextQuestion()
{
	if (Problems.Num())
	{
		int32 ProblemsIndex = UKismetMathLibrary::RandomIntegerInRange(0, Problems.Num() - 1);
		CurrentProblem = Problems[ProblemsIndex];

		const FProblemBase* CurrentProblemPtr = CurrentProblem.GetMutablePtr<FProblemBase>();
		for (auto It = Problems.CreateIterator(); It; ++It)
		{
			if (const FProblemBase* Problem = It->GetPtr<FProblemBase>())
			{
				if (Problem->ProblemContent == CurrentProblemPtr->ProblemContent)
				{
					It.RemoveCurrent();
					break;
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
		FProblemBase* Problem = CurrentProblem.GetMutablePtr<FProblemBase>();
		if (Problem)
		{
			Problem->InitProblem();
			UpdateQuestionUI(Problem->GetUIShowList());
		}

		//更新地图上的小球
		UpdateProblemDelegate.ExecuteIfBound(*Problem);
	}
}

void AProblemEnemy::UpdateQuestionUI(const TArray<FString>& ShowList)
{
	//更新UI
	if (UUserWidget* Widget = QuestionWidget->GetWidget())
	{
		if (UWG_EnemyProblem* WG_EnemyProblem = Cast<UWG_EnemyProblem>(Widget))
		{
			WG_EnemyProblem->UpdateProblemContext(ShowList);
			WG_EnemyProblem->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

