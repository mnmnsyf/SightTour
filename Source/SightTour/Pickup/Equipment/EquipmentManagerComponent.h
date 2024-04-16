// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../FillBall/FillBall.h"
#include "EquipmentManagerComponent.generated.h"

class UObject;
class UTP_WeaponComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FPickupBallDelegate, bool, bPickup, int32, SlotIndex, FString, BallContent, FName, BallType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeSlotDelegate, int32, LastIndex, int32, NextIndex);

/**
 * 装备管理组件，管理玩家装备的枪械和子弹数量
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIGHTTOUR_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipmentManagerComponent();

	virtual void BeginPlay() override;

public:
	static UEquipmentManagerComponent* Get(AActor* Owner);

	FORCEINLINE UTP_WeaponComponent* GetCurrentEquippedWeapon() const { return EquippedWeapon; }


	FORCEINLINE void EquipWeapon(UTP_WeaponComponent* InEquippedWeapon) { EquippedWeapon = InEquippedWeapon; };

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void PickupFillBall(AFillBall* Ball);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	FInstancedStruct DiscardFillBall();

	void ChangeSlot(int32 NewSlot);

	FORCEINLINE int32 GetCurrentSlot() const { return CurrentSlot; }

	FName GetCurrentBallType() const;

public:
	FPickupBallDelegate PickupBallDelegate;

	FChangeSlotDelegate ChangeSlotDelegate;

private:
	//现在手持的枪械
	UPROPERTY()
	UTP_WeaponComponent* EquippedWeapon = nullptr;

	UPROPERTY()
	TMap<int32, FInstancedStruct> BallMap;

	//当前选择的Slot
	int32 CurrentSlot = 0;
};