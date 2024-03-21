// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentManagerComponent.generated.h"

class UTP_WeaponComponent;

/**
 * 装备管理组件，管理玩家装备的枪械和子弹数量
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIGHTTOUR_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetDigitalProjectileValue() const { return DigitalProjectileValue; }

	void UpdateDigitalProjectileValue(const float UpdateValue);
		
protected:
	//数字子弹合计大小
	float DigitalProjectileValue = 1.0f;

	//持有的枪械
	UPROPERTY()
	TArray<UTP_WeaponComponent*> EquippedWeapons;

	//持有的枪械
	UPROPERTY()
	UTP_WeaponComponent* CurrentEquippedWeapon = nullptr;
};
