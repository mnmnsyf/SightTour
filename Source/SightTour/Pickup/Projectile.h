// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/PickupItemBase.h"
#include "Interface/AttractInterface.h"
#include "Projectile.generated.h"

class UTP_WeaponComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class USphereComponent;

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API AProjectile : public APickupItemBase, public IAttractInterface
{
	GENERATED_BODY()
	
public:
	AProjectile();

	void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay();

public:
	void Attract(class UTP_WeaponComponent* WeaponComp) override;

	void Spawn(class UTP_WeaponComponent* WeaponComp) override;

	//获取枪口发射命中位置
	bool GeSpawnDirection(FVector& OutHitDirection);

	//获取视口发射命中位置
	bool TraceUnderCrosshair(FVector& OutHitLocation);

protected:
	void PerMove();

	void StopAndAttach();

protected:
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh = nullptr;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;*/

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Projectile,  meta = (DisplayName = "吸取速度"))
	float AttractSpeed = 10.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Projectile, meta = (DisplayName = "距离阈值"))
	float DistanceThreshold = 50.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Projectile, meta = (DisplayName = "发射力度"))
	float ForceValue = 50.f;

private:
	UPROPERTY()
	UTP_WeaponComponent* OwnerWeapon = nullptr;

	FTimerHandle AttractTimer;

	bool bOpenPhysicSimulate = false;

	bool bMoving = false;
};
