// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/DataTable.h"
#include "BigLotteryDefines.generated.h"

USTRUCT(BlueprintType)
struct FBigLotteryConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "小球ID"))
	FColor BallColor = FColor::White;
};