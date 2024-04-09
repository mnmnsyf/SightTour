// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyCharacter.h"
#include "ProblemEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API AProblemEnemy : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	

private:
	FString CurrentProblem;
};
