// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "WG_InventoryBar.generated.h"

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API UWG_InventoryBar : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* PB_Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HB_InventoryBox;

protected:
	void NativeConstruct() override;

	void OnBallPickup(bool bPickup, int32 InSlotIndex, FString BallContent);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnBallSlotChange(int32 LastIndex, int32 NextIndex);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | HealthBar", meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<class UWG_BallSlot> BallSlotClass;

private:
	UWG_BallSlot* LoadBallSlot();

private:
	UPROPERTY()
	TArray<TObjectPtr<class UWG_BallSlot>> InventorySlot;
};
