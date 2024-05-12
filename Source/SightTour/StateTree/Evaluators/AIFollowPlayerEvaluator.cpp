// Fill out your copyright notice in the Description page of Project Settings.


#include "AIFollowPlayerEvaluator.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "StateTree/StateTreeBlackBoardCom.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void FAIFollowPlayerEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	// 判空
	UObject* OwnerObject = Context.GetOwner();
	if (!IsValid(OwnerObject))
		return;
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerObject);
	if (!IsValid(OwnerCharacter))
		return;

	InstanceData.bFollowTarget = false;

	UStateTreeBlackBoardCom* STBB = OwnerCharacter->FindComponentByClass<UStateTreeBlackBoardCom>();
	if (!IsValid(STBB))
		return;
	ACharacter* FollowTarget = UGameplayStatics::GetPlayerCharacter(Context.GetWorld(), 0);
	if (!IsValid(FollowTarget))
		return;

	FVector TargetLocation = FollowTarget->GetActorLocation();
	FVector OwnerLocation = OwnerCharacter->GetActorLocation();

	float CurrentDistance = FVector::Dist2D(TargetLocation, OwnerLocation);

	if (CurrentDistance <= InstanceData.MaxRange && CurrentDistance >= InstanceData.MinRange)
	{
		InstanceData.bFollowTarget = true;

		InstanceData.TargetLocation = TargetLocation;
	}

	//Debug
	if (InstanceData.bShowDebug)
	{
		UKismetSystemLibrary::DrawDebugCircle(OwnerObject, OwnerLocation, InstanceData.MaxRange, 12, FLinearColor::Red, DeltaTime, 5.0f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f));
		UKismetSystemLibrary::DrawDebugCircle(OwnerObject, OwnerLocation, InstanceData.MinRange, 12, FLinearColor::Red, DeltaTime, 5.0f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f));
	}
}