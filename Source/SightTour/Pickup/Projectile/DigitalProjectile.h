// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/PickupItemBase.h"
#include "Interface/AttractInterface.h"
#include "Engine/DataTable.h"
#include "DigitalProjectile.generated.h"

class UTP_WeaponComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class USphereComponent;

/**
 * 数字子弹
 */
UCLASS()
class SIGHTTOUR_API ADigitalProjectile : public APickupItemBase, public IAttractInterface
{
	GENERATED_BODY()
	
public:
	ADigitalProjectile();

	void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay();

public:
	void Attract(class UTP_WeaponComponent* WeaponComp) override;

	void Spawn(class UTP_WeaponComponent* WeaponComp) override;

	void OnEquipped() override;

	//获取枪口发射命中位置
	bool GeSpawnDirection(FVector& OutHitDirection);

	//获取视口发射命中位置
	bool TraceUnderCrosshair(FVector& OutHitLocation);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	void PerMove();

protected:
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent* CollisionSphere = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Projectile,  meta = (DisplayName = "吸取速度"))
	float AttractSpeed = 10.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Projectile, meta = (DisplayName = "距离阈值"))
	float DistanceThreshold = 50.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Projectile, meta = (DisplayName = "发射力度"))
	float ForceValue = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "小球配置", RowType = "/Script/SightTour.DigitalProjectileConfig"))
	FDataTableRowHandle BallConfig;

private:
	UPROPERTY()
	UTP_WeaponComponent* OwnerWeapon = nullptr;

	bool bOpenPhysicSimulate = false;

	bool bMoving = false;

	friend class UTP_WeaponComponent;
};
