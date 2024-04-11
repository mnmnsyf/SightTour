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
	CurrentHealth = MaxHealth;
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
