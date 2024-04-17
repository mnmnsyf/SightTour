// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAttackTargetEvaluator.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "GameFramework/Character.h"
#include "../StateTreeBlackBoardCom.h"
#include "Kismet/GameplayStatics.h"
#include "../Enemy/EnemyCharacter.h"

void FAIAttackTargetEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	// 判空
	UObject* OwnerObject = Context.GetOwner();
	if (!IsValid(OwnerObject))
		return;
	AEnemyCharacter* OwnerCharacter = Cast<AEnemyCharacter>(OwnerObject);
	if (!IsValid(OwnerCharacter))
		return;
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(Context.GetWorld(), 0);
	if (!IsValid(Player))
		return;

	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	FVector PlayerLcoation = Player->GetActorLocation();

	InstanceData.bAttack = FVector::Dist2D(OwnerLocation, PlayerLcoation) <= InstanceData.AttackRange;
	if (InstanceData.bAttack)
	{
		InstanceData.Anim = OwnerCharacter->AttackAnim;
		InstanceData.AttackSectionName = OwnerCharacter->GetRandomAttackSectionName();
	}
}