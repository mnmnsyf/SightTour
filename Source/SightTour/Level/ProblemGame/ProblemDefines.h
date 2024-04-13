// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/DataTable.h"
#include "ProblemDefines.generated.h"

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