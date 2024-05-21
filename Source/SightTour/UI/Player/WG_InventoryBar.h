// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "../Pickup/PickupItemDefines.h"
#include "WG_InventoryBar.generated.h"




UCLASS()
class SIGHTTOUR_API UWG_InventoryBar : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HB_InventoryBox;

protected:
	void NativeConstruct() override;

	UFUNCTION()
	void OnBallPickup(bool bPickup, int32 InSlotIndex, FString BallContent, FName BallType);

	UFUNCTION()
	void OnBallSlotChange(int32 LastIndex, int32 NextIndex);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryBar", meta = (AllowPrivateAccess = "true", AllowAbstract))
	TSoftClassPtr<class UWG_BallSlot> BallSlotClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryBar", meta = (AllowPrivateAccess = "true"))
	TMap<FName, class UTexture2D*> BallTypeIconMap;

private:
	UWG_BallSlot* LoadBallSlot();

private:
	UPROPERTY()
	TArray<TObjectPtr<class UWG_BallSlot>> InventorySlot;
};
