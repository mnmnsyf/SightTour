// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/FillBall/FillBall.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "BallPool.generated.h"

class AProblemEnemy;

UCLASS()
class SIGHTTOUR_API ABallPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	bool InitConfig();

	UFUNCTION()
	void SpawnBall(FProblemBase Problem);

	void InnerSpawnBall(FInstancedStruct& BallConfig);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BallPool)
	UBoxComponent* SpawnArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BallPool, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AProblemEnemy> Enemy;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BallPool)
	TSubclassOf<AFillBall> BallClass;
};
