// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BigLotteryDefines.h"
#include "BigLottery.generated.h"

DECLARE_DELEGATE_OneParam(FBigLotteryTriggerDelegate, bool);

UCLASS()
class SIGHTTOUR_API ABigLottery : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABigLottery();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE bool GetIsTriggerd() const { return bTriggered; }

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool BFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsTrigged() { return bTriggered; };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "小球配置", RowType = "/Script/SightTour.BigLotteryConfig"))
	FDataTableRowHandle BallConfig;

public:
	FBigLotteryTriggerDelegate BigLotteryDelegate;

private:
	bool bTriggered = false;

};
