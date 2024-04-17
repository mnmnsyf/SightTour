// Fill out your copyright notice in the Description page of Project Settings.


#include "AIFollowTargetEvaluator.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "GameFramework/Character.h"
#include "../StateTreeBlackBoardCom.h"
#include "Kismet/GameplayStatics.h"

void FAIFollowTargetEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
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

	FInstancedPropertyBag& RuntimeParam = STBB->GetRuntimeParams();
	auto FollowTargetParam = RuntimeParam.GetValueStruct<FAIFollowStruct>(STBBC::STBB_PARAM_Follow);
	if (!FollowTargetParam.IsValid() || !FollowTargetParam.HasValue())
		return;

	FAIFollowStruct AIFollowStruct = *FollowTargetParam.GetValue();
	FVector TargetLoc = FollowTarget->GetActorLocation();
	FVector AgentLoc = OwnerCharacter->GetActorLocation();
	FVector FollowLoc = AIFollowStruct.FollowPointWorldTransform.GetLocation();

	float CheckDist = AIFollowStruct.FollowPointRelativeTransform.GetLocation().Size() * AIFollowStruct.ReCalcFollowPointDistRate;

	//计算下个目标点
	if (!AIFollowStruct.bReCalcFollowPoint)
	{
		//检测AI、FollowPoint和Target之间的水平距离，判断是否应该重新计算
		float HorizontalAgentTargetDist = (TargetLoc - AgentLoc).Size2D();
		float HorizontalFollowTargetDist = (TargetLoc - FollowLoc).Size2D();

		if (HorizontalAgentTargetDist >= CheckDist && HorizontalFollowTargetDist >= CheckDist)
		{
			AIFollowStruct.bReCalcFollowPoint = true;
		}
	}

	if (!AIFollowStruct.bReCalcFollowPoint)
	{
		//InstanceData.bFollowTarget = false;
		return;
	}
	else
	{
		InstanceData.bFollowTarget = true;

		//重新计算目标位置（不接地）
		FTransform ActorRootTransform = FollowTarget->GetActorTransform();

		FVector WorldRelativePos = AIFollowStruct.FollowPointRelativeTransform.GetLocation();
		FVector WorldRelativeRot = AIFollowStruct.FollowPointRelativeTransform.GetRotation().GetForwardVector();

		FVector WorldAbsolutePos = ActorRootTransform.TransformPosition(WorldRelativePos);
		FVector WorldAbsoluteRot = ActorRootTransform.TransformVector(WorldRelativeRot);

		FTransform CalcWorldTrans;
		CalcWorldTrans.SetLocation(WorldAbsolutePos);
		CalcWorldTrans.SetRotation(WorldAbsoluteRot.ToOrientationQuat());

		AIFollowStruct.FollowPointWorldTransform = CalcWorldTrans;
		AIFollowStruct.bReCalcFollowPoint = false;

		InstanceData.AIFollowStruct = AIFollowStruct;
		RuntimeParam.SetValueStruct<FAIFollowStruct>(STBBC::STBB_PARAM_Follow, AIFollowStruct);
	}

	//Debug
	if (InstanceData.bShowDebug)
	{
		float FollowRadius = AIFollowStruct.FollowPointRelativeTransform.GetLocation().Size() * AIFollowStruct.ReCalcFollowPointDistRate;
		UKismetSystemLibrary::DrawDebugCircle(OwnerObject, TargetLoc, FollowRadius * AIFollowStruct.ReCalcFollowPointDistRate, 12, FLinearColor::Red, DeltaTime, 5.0f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f));
	}
}