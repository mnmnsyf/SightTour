// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BigLotteryDefines.h"
#include "BigLotteryComponent.generated.h"

/**
 * 大乐透小球组件，添加此组件的Actor可触发大乐透机关
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIGHTTOUR_API UBigLotteryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBigLotteryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FORCEINLINE FName GetBallID() const { return BallConfig.RowName; }
		
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "小球配置", RowType = "/Script/SightTour.BigLotteryConfig"))
	FDataTableRowHandle BallConfig;
};
