// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BigLotteryManager.generated.h"

class ABigLottery;

UCLASS()
class SIGHTTOUR_API ABigLotteryManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABigLotteryManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual bool ShouldTickIfViewportsOnly() const;
#endif

protected:
	void OnBigLotteryTriggered(bool bTriggered);

	void TriggerLottery();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_TriggerLottery();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ABigLottery>> BigLotteries;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TriggeredDelayTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "机关通过后开启的下一个关卡机关，必须实现ILevelInterface接口"))
	TObjectPtr<AActor> NextLevelActor = nullptr;

private:
	FTimerHandle TriggerTimer;

	bool bTriggerLottery = false;

#if WITH_EDITOR
	bool bTickLine = true;
#endif
};
