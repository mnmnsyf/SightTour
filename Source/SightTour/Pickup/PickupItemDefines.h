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

UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Pickup Data", ShortTooltip = "Data asset used to configure a pickup."))
class SIGHTTOUR_API UPickupDefinition : public UDataAsset
{
	GENERATED_BODY()

public:

	//Defines the pickup's actors to spawn, abilities to grant, and tags to add
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup | Equipment")
	TSubclassOf<UObject> InventoryItemDefinition;

	//Visual representation of the pickup
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup | Mesh")
	TObjectPtr<UStaticMesh> DisplayMesh;

	//Sound to play when picked up
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<USoundBase> PickedUpSound;

	//Sound to play when pickup is respawned
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<USoundBase> RespawnedSound;

	//Particle FX to play when picked up
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UNiagaraSystem> PickedUpEffect;

	//Particle FX to play when pickup is respawned
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UNiagaraSystem> RespawnedEffect;
};


UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Weapon Pickup Data", ShortTooltip = "Data asset used to configure a weapon pickup."))
class SIGHTTOUR_API UWeaponPickupDefinition : public UPickupDefinition
{
	GENERATED_BODY()

public:
	//Sets the height of the display mesh above the Weapon spawner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Pickup|Mesh")
	FVector WeaponMeshOffset;

	//Sets the height of the display mesh above the Weapon spawner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Pickup|Mesh")
	FVector WeaponMeshScale = FVector(1.0f, 1.0f, 1.0f);
};