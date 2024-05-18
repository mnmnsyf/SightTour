// Fill out your copyright notice in the Description page of Project Settings.


#include "BallPool.h"
#include "Enemy/ProblemEnemy.h"
#include "System/SightTourGameInstance.h"
#include "Pickup/PickupItemDefines.h"
#include "Kismet/KismetMathLibrary.h"
#include "../ProblemGame/Problem/ProblemBase.h"

ABallPool::ABallPool()
{
 	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnArea->SetupAttachment(RootComponent);
}

void ABallPool::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(Enemy))
	{
		Enemy->UpdateProblemDelegate.BindUObject(this, &ABallPool::SpawnBall);
	}
}

void ABallPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABallPool::InitConfig()
{
	APlayerController* PC = USightTourGameInstance::Get()->GetCurPlayerController();
	if (!PC)
	{
		return false;
	}

	

	return false;
}

void ABallPool::SpawnBall(FProblemBase Problem)
{
	//清空地图上现有的小球
	TArray<AActor*> CurrentBall;
	UGameplayStatics::GetAllActorsOfClass(this, AFillBall::StaticClass(), CurrentBall);
	for (auto& EachBall : CurrentBall)
	{
		EachBall->Destroy();
	}
	//生成正确答案的小球
	for (auto& EachAnswer : Problem.CorrectAnswer)
	{
		InnerSpawnBall(EachAnswer);
	}

	//生成错误答案的小球
	for (auto& EachAnswer : Problem.WrongAnswer)
	{
		InnerSpawnBall(EachAnswer);
	}
}

void ABallPool::InnerSpawnBall(FInstancedStruct& BallConfig)
{
	FVector SpawnLoc = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->GetComponentLocation(), SpawnArea->GetScaledBoxExtent());

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AFillBall* NewFillBall = GetWorld()->SpawnActor<AFillBall>(BallClass, SpawnLoc, FRotator(0.f), SpawnParams);

	if (FPickableItem* Config = BallConfig.GetMutablePtr<FPickableItem>())
	{
		NewFillBall->SetProjectileMeshMaterial(Config->Material);
	}
	NewFillBall->SetFillBallConfig(BallConfig);
}
