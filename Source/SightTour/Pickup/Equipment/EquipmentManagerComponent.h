// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Projectile/DigitalProjectile.h"
#include "EquipmentManagerComponent.generated.h"

class UObject;
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
	FORCEINLINE UTP_WeaponComponent* GetCurrentEquippedWeapon() const { return CurrentEquippedWeapon; }

	FORCEINLINE TSubclassOf<ADigitalProjectile> GetProjectileClass() const { return HasBullet ? DigitalProjectileClass : nullptr; }

	FORCEINLINE float GetDigitalProjectileValue() const { return DigitalProjectileValue; }

	void UpdateDigitalProjectileValue(const float UpdateValue);

	void EquipWeapon(UTP_WeaponComponent* InEquippedWeapon);
		
protected:
	//数字子弹总伤害
	float DigitalProjectileValue = 1.0f;

	//是否有子弹
	bool HasBullet = false;

	//打出的子弹
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Projectile")
	TSubclassOf<ADigitalProjectile> DigitalProjectileClass;

	//持有的枪械
	UPROPERTY()
	TArray<UTP_WeaponComponent*> EquippedWeapons;

	//现在手持的枪械
	UPROPERTY()
	UTP_WeaponComponent* CurrentEquippedWeapon = nullptr;
};