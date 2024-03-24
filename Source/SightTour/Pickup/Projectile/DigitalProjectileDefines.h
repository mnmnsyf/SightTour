// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/DataTable.h"
#include "DigitalProjectileDefines.generated.h"

UENUM(BlueprintType)
enum class EDigitalProjectType : uint8
{
	None = 0 UMETA(Hidden),
	Addition UMETA(DisplayName = "加法"),
	Subtraction UMETA(DisplayName = "减法"),
	Multiplication UMETA(DisplayName = "乘法"),
	Division UMETA(DisplayName = "除法"),
	Magic UMETA(DisplayName = "魔法"),
	MAX
};

USTRUCT(BlueprintType)
struct FDigitalProjectileConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "小球数字"))
	float BallNumber;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "小球类型"))
	EDigitalProjectType BallType = EDigitalProjectType::None;

	/*UPROPERTY(EditAnywhere, meta = (AllowedClasses = "UserWidget", DisplayName = "数字UI"))
	FSoftObjectPath NumberUI;*/

};