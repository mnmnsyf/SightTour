// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//开始游戏后将敌人当前生命值和最大生命值相匹配
	ResetDefaultHealth();
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharacter::SetCharacterMoveByForce(const FVector& ForceDir)
{
	AddMovementInput(ForceDir);
}

void AEnemyCharacter::ReduceHealth(const float ReduceValue)
{
	CurrentHealth -= ReduceValue;

	if (CurrentHealth <= 0)
	{
		//TODO:播放死亡动画&音效
		SetLifeSpan(5.0f);
	}

	FString DebugMsg = FString::Printf(TEXT("Enemy current health %.0f"), CurrentHealth);
	GEngine->AddOnScreenDebugMessage(-1, 1500.0f, FColor::Red, DebugMsg);
}
