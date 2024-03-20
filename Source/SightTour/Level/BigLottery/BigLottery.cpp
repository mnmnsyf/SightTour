// Fill out your copyright notice in the Description page of Project Settings.


#include "BigLottery.h"
#include "Components/BoxComponent.h"
#include "BigLotteryComponent.h"

// Sets default values
ABigLottery::ABigLottery()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	TriggerBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABigLottery::BeginPlay()
{
	Super::BeginPlay();
	
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABigLottery::OnBoxBeginOverlap);
		TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ABigLottery::OnBoxEndOverlap);
	}
	
}

// Called every frame
void ABigLottery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABigLottery::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool BFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (UBigLotteryComponent* BigLotteryComponent = OtherActor->GetComponentByClass<UBigLotteryComponent>())
		{
			if (BallConfig.RowName == BigLotteryComponent->GetBallID())
			{
				bTriggered = true;
				BigLotteryDelegate.ExecuteIfBound(bTriggered);
			}
			
		}
	}
}

void ABigLottery::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (UBigLotteryComponent* BigLotteryComponent = OtherActor->GetComponentByClass<UBigLotteryComponent>())
		{
			if (BallConfig.RowName == BigLotteryComponent->GetBallID())
			{
				bTriggered = false;
				BigLotteryDelegate.ExecuteIfBound(bTriggered);
			}
		}
	}
}

