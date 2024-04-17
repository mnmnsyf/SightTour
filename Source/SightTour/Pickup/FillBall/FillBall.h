// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/PickupItemBase.h"
#include "Interface/AttractInterface.h"
#include "Engine/DataTable.h"
#include "PickupItemDefines.h"
#include "../Interface/ItemInfoInterface.h"
#include "FillBall.generated.h"

class UTP_WeaponComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class USphereComponent;

/**
 * 数字小球
 */
UCLASS()
class SIGHTTOUR_API AFillBall : public APickupItemBase, public IAttractInterface, public IItemInfoInterface
{
	GENERATED_BODY()
	
public:
	AFillBall();

	void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay();

public:
	FString GetItemName() override;

	void Attract(class UTP_WeaponComponent* WeaponComp) override;

	void Spawn(class UTP_WeaponComponent* WeaponComp) override;

	void OnEquipped() override;


	//获取枪口发射命中位置
	bool GeSpawnDirection(FVector& OutHitDirection);

	//获取视口发射命中位置
	bool TraceUnderCrosshair(FVector& OutHitLocation);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void BallSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

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
	float ForceValue = 25000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "小球配置", RowType = "/Script/SightTour.PickableConfig"))
	FDataTableRowHandle BallConfigRowHandle;

public:
	//数字小球配置
	FInstancedStruct FillBallConfig;

	//是否开启伤害
	bool bEnableDamage = false;

private:
	UPROPERTY()
	TObjectPtr<UTP_WeaponComponent> OwnerWeapon = nullptr;

	//初始是否开启模拟物理
	bool bOpenPhysicSimulate = false;

	//是否被吸取中
	bool bAttacting = false;

	friend class UTP_WeaponComponent;
};
