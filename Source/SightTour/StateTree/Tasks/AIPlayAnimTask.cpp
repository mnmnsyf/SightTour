// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPlayAnimTask.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Enemy/EnemyCharacter.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

EStateTreeRunStatus FAIPlayAnimTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	return InstanceData->OnEnterState(Context);

	UObject* OwnerObject = Context.GetOwner();
	if (!IsValid(OwnerObject))
		return EStateTreeRunStatus::Failed;
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerObject);
	if (!IsValid(OwnerCharacter))
		return EStateTreeRunStatus::Failed;

	return EStateTreeRunStatus::Running;
}

void FAIPlayAnimTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	Super::ExitState(Context, Transition);
}

EStateTreeRunStatus FAIPlayAnimTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	return InstanceData->OnTick(Context, DeltaTime);
}

EStateTreeRunStatus UAIPlayAnimTaskInstanceData::OnEnterState(const FStateTreeExecutionContext& Context)
{
	UObject* OwnerObject = Context.GetOwner();
	if (!OwnerObject)
		return EStateTreeRunStatus::Failed;

	AEnemyCharacter* OwnerCharacter = Cast<AEnemyCharacter>(OwnerObject);
	if (!OwnerCharacter)
		return EStateTreeRunStatus::Failed;

	USkeletalMeshComponent* SkelMeshComp = OwnerCharacter->GetMesh();
	if (!SkelMeshComp)
		return EStateTreeRunStatus::Failed;

	UAnimInstance* AnimInstance = SkelMeshComp->GetAnimInstance();
	if (!AnimInstance)
		return EStateTreeRunStatus::Failed;

	if (!MontageAnim)
	{
		UE_VLOG_UELOG(Context.GetOwner(), LogStateTree, Warning, TEXT("%hs Failed. Reason: Invalid Anim Asset"), __FUNCTION__);
		return EStateTreeRunStatus::Failed;
	}

	OwnerCharacter->PlayAnimMontage(MontageAnim, 1.f, AnimSectionName);

	AnimInstance->OnMontageBlendingOut.AddUniqueDynamic(this, &UAIPlayAnimTaskInstanceData::OnMontageEnded);
	AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UAIPlayAnimTaskInstanceData::OnNotifyBeginReceived);

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus UAIPlayAnimTaskInstanceData::OnTick(const FStateTreeExecutionContext& Context, float DeltaTime)
{
	if (bMontageInterrupted)
	{
		return EStateTreeRunStatus::Failed;
	}

	if (!bLoopForever && CompletedLoops >= LoopsToRun)
	{
		return EStateTreeRunStatus::Succeeded;
	}

	EStateTreeRunStatus RunStatus = EStateTreeRunStatus::Running;

	//TODO 播下一个动画
	//if (TimeBeforeStartingNewLoop.IsSet())
	//{
	//	*TimeBeforeStartingNewLoop -= DeltaTime;
	//	if (*TimeBeforeStartingNewLoop <= 0.f)
	//	{
	//		//RunStatus = Play(Context);
	//	}
	//}

	return RunStatus;
}

void UAIPlayAnimTaskInstanceData::OnMontageEnded(UAnimMontage* const EndedMontage, const bool bInterrupted)
{
	if (bWaitForNotifyEventToEnd && bInterrupted && bExpectInterruptionFromNotifyEventLoop)
	{
		bExpectInterruptionFromNotifyEventLoop = false;

		return;
	}

	if (bWaitForNotifyEventToEnd && !bInterrupted)
	{
		return;
	}

	UE_LOG(LogStateTree, Verbose, TEXT("%hs EndedMontage: %s bInterrupted: %d"), __FUNCTION__, *GetNameSafe(EndedMontage), bInterrupted);

	if (bInterrupted)
	{
		bMontageInterrupted = true;
	}

	CompletedLoops++;
	TimeBeforeStartingNewLoop = 0.0f;
}

void UAIPlayAnimTaskInstanceData::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	UE_LOG(LogStateTree, Verbose, TEXT("%hs NotifyName: %s Anim: %s"), __FUNCTION__, *NotifyName.ToString(), *GetNameSafe(BranchingPointNotifyPayload.SequenceAsset));

	if (bWaitForNotifyEventToEnd && NotifyEventNameToEnd == NotifyName)
	{
		bExpectInterruptionFromNotifyEventLoop = true;

		CompletedLoops++;
		TimeBeforeStartingNewLoop = 0.0f;
	}
}
