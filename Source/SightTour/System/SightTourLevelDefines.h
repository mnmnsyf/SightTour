// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/DataTable.h"
#include "InstancedStruct.h"
#include "SightTourLevelDefines.generated.h"


USTRUCT(BlueprintType)
struct SIGHTTOUR_API FSightTourLevelConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString PlayerStartTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EnemyID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextLevelName;

};