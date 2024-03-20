// Fill out your copyright notice in the Description page of Project Settings.


#include "BigLotteryManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BigLottery.h"

// Sets default values
ABigLotteryManager::ABigLotteryManager()
{
#if WITH_EDITOR
	PrimaryActorTick.bCanEverTick = true;
#elif
	PrimaryActorTick.bCanEverTick = false;
#endif
}

// Called when the game starts or when spawned
void ABigLotteryManager::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	bTickLine = false;
#endif

	for (ABigLottery* EachLottery : BigLotteries)
	{
		EachLottery->BigLotteryDelegate.BindUObject(this, &ABigLotteryManager::OnBigLotteryTriggered);
	}
}

void ABigLotteryManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

#if WITH_EDITOR
	bTickLine = true;
#endif
}

// Called every frame
void ABigLotteryManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITOR
	if (bTickLine)
	{
		for (const ABigLottery* BigLottery : BigLotteries)
		{
			if (BigLottery)
			{
				UKismetSystemLibrary::DrawDebugArrow(GetWorld(), GetActorLocation(), BigLottery->GetActorLocation(), 10.f, FColor::Cyan, -1, 2.f);
			}
		}
	}
#endif
}

#if WITH_EDITOR
bool ABigLotteryManager::ShouldTickIfViewportsOnly() const
{
	return true;
}
#endif

void ABigLotteryManager::OnBigLotteryTriggered(bool bTriggered)
{
	if (bTriggered)
	{
		bool bAllTriggered = true;
		for (ABigLottery* EachLotttery : BigLotteries)
		{
			if (!EachLotttery->GetIsTriggerd())
			{
				bAllTriggered = false;
				break;
			}
		}

		if (bAllTriggered)
		{
			GetWorldTimerManager().SetTimer(TriggerTimer, this, &ABigLotteryManager::TriggerLottery, TriggeredDelayTime, false);
			return;
		}
	}

	GetWorldTimerManager().ClearTimer(TriggerTimer);
}

void ABigLotteryManager::TriggerLottery()
{
	bTriggerLottery = true;
}
