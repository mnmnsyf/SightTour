// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMoveToTask.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavMesh/RecastNavMesh.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Tools/NavCommonTools.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/AIModule/Classes/AIController.h"

EStateTreeRunStatus FAIMoveToTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	return EStateTreeRunStatus::Running;
}

void FAIMoveToTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	Super::ExitState(Context, Transition);
}

EStateTreeRunStatus FAIMoveToTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	Super::Tick(Context, DeltaTime);
	UObject* OwnerObject = Context.GetOwner();
	if (!IsValid(OwnerObject))
		return EStateTreeRunStatus::Failed;
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerObject);
	if (!OwnerCharacter)
		return EStateTreeRunStatus::Failed;
	AController* OwnerController = OwnerCharacter->GetController();
	if (!OwnerController)
		return EStateTreeRunStatus::Failed;
	AAIController* AIController = Cast<AAIController>(OwnerController);
	if (!AIController)
		return EStateTreeRunStatus::Failed;

	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	AIController->MoveToLocation(InstanceData.TargetLocation, InstanceData.AcceptanceRadius);

	return EStateTreeRunStatus::Running;
}