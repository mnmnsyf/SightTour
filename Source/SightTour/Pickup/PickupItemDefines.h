// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "PickupItemDefines.generated.h"

class UNiagaraSystem;
class UObject;
class USoundBase;
class UStaticMesh;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None = 0 UMETA(Hidden),
	AdditionGun UMETA(DisplayName = "加法枪"),
	SubtractionGun UMETA(DisplayName = "减法枪"),
	MultiplicationGun UMETA(DisplayName = "乘法枪"),
	DivisionGun UMETA(DisplayName = "除法枪"),
	MagicGun UMETA(DisplayName = "魔法枪"),
	MAX
};