// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/DataTable.h"
#include "BigLotteryDefines.generated.h"

USTRUCT(BlueprintType)
struct FDigitalProjectileConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "小球数字"))
	float BallNumber;

	/*UPROPERTY(EditAnywhere, meta = (AllowedClasses = "UserWidget", DisplayName = "数字UI"))
	FSoftObjectPath NumberUI;*/

};