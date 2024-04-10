// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ProblemEnemy.h"
#include "Level/ProblemGame/ProblemDefines.h"
#include "InstancedStruct.h"


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
}
