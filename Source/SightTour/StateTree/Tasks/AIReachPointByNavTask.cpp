// Fill out your copyright notice in the Description page of Project Settings.


#include "AIReachPointByNavTask.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavMesh/RecastNavMesh.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Tools/NavCommonTools.h"

EStateTreeRunStatus FAIReachPointByNavTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	Super::EnterState(Context, Transition);
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	UObject* OwnerObject = Context.GetOwner();
	if (!IsValid(OwnerObject))
		return EStateTreeRunStatus::Failed;
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerObject);
	if (!IsValid(OwnerCharacter))
		return EStateTreeRunStatus::Failed;

	//找出两点路径
	InstanceData.NavPath = FindNavPathToTargetLocation(OwnerCharacter, OwnerCharacter->GetActorLocation(), InstanceData.TargetLocation);
	if (InstanceData.NavPath.IsEmpty())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("任务：NPC导航到达目标点，找不到导航路捏")));
		return EStateTreeRunStatus::Failed;
	}
	
	InstanceData.PathIndex = 0;
	InstanceData.HistoryTargetLocation = InstanceData.TargetLocation;
	InstanceData.CurTargetLocation = GetNextPointByNavPoints(OwnerCharacter->GetActorLocation(), InstanceData.NavPath);

	return EStateTreeRunStatus::Running;
}

void FAIReachPointByNavTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	Super::ExitState(Context, Transition);
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	UObject* OwnerObject = Context.GetOwner();
	if (!IsValid(OwnerObject))
		return;
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerObject);
	if (!IsValid(OwnerCharacter))
		return;

	// STBB清除结构体
	/*if (InstanceData.bClearMovePointStruct == true)
	{
		UStateTreeBlackBoardCom* STBB = OwnerCharacter->FindComponentByClass<UStateTreeBlackBoardCom>();
		if (STBB)
		{
			FInstancedPropertyBag& RuntimeParam = STBB->GetRuntimeParams();
			auto AnimRuntimeValue = RuntimeParam.GetValueStruct(STBBC::STBB_PARAM_MoveToPoint);
			if (AnimRuntimeValue.IsValid() && AnimRuntimeValue.HasValue())
			{
				RuntimeParam.RemovePropertyByName(STBBC::STBB_PARAM_MoveToPoint);
			}
		}
	}*/
}

void FAIReachPointByNavTask::StateCompleted(FStateTreeExecutionContext& Context, const EStateTreeRunStatus CompletionStatus, const FStateTreeActiveStates& CompletedActiveStates) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	Super::StateCompleted(Context, CompletionStatus, CompletedActiveStates);
}

EStateTreeRunStatus FAIReachPointByNavTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	Super::Tick(Context, DeltaTime);
	UObject* OwnerObject = Context.GetOwner();
	if (!IsValid(OwnerObject))
		return EStateTreeRunStatus::Failed;
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerObject);
	if (!OwnerCharacter)
		return EStateTreeRunStatus::Failed;

	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (InstanceData.MoveTime > 0)
		InstanceData.RemainedTime -= DeltaTime;
	// 超过移动时间，成功
	if (InstanceData.RemainedTime < 0 && InstanceData.MoveTime > 0)
	{
		return EStateTreeRunStatus::Succeeded;
	}

	//判断是否需要更新导航路径
	FVector ActorToTarget = InstanceData.TargetLocation - InstanceData.HistoryTargetLocation;
	ActorToTarget.Z = 0;
	const FVector::FReal DistToTargetSq = ActorToTarget.SizeSquared();
	if (DistToTargetSq > 300.f)
	{
		InstanceData.NavPath = FindNavPathToTargetLocation(OwnerCharacter, OwnerCharacter->GetActorLocation(), InstanceData.TargetLocation);
		if (InstanceData.NavPath.IsEmpty())
		{
			return EStateTreeRunStatus::Failed;
		}

		InstanceData.HistoryTargetLocation = InstanceData.TargetLocation;
		InstanceData.bInLaneEndPoint = false;
	}



	FVector AILoc = OwnerCharacter->GetActorLocation();

	FVector Direct2D = (InstanceData.CurTargetLocation - AILoc);
	Direct2D.Z = 0;

	FVector Direct2DNormalize = Direct2D.GetSafeNormal();

	if (InstanceData.bShowDebug)
	{
		for (int32 Index = 0; Index < InstanceData.NavPath.Num(); ++Index)
		{
			UKismetSystemLibrary::DrawDebugPoint(OwnerObject, InstanceData.NavPath[Index], 10.f, FLinearColor::Blue, 5.0f);
		}
		UKismetSystemLibrary::DrawDebugBox(OwnerObject, InstanceData.CurTargetLocation, FVector(40.0f, 40.0f, 100.0f), FLinearColor::Blue, FRotator::ZeroRotator, 5.f, 1.0f);
	}

	//到达终点返回成功
	if (InstanceData.bInLaneEndPoint == true && Direct2D.Size() <= InstanceData.ReachToEndPointleranceDist)
	{
		OwnerCharacter->SetActorLocation(InstanceData.TargetLocation);
		return EStateTreeRunStatus::Succeeded;
	}

	// 水平距离大于到达点的容错误差，处理移动
	if (Direct2D.Size() > InstanceData.ReachPointToleranceDist)
	{
		if (UCharacterMovementComponent* CharacterMovement = OwnerCharacter->GetCharacterMovement())
		{
			CharacterMovement->AddInputVector(Direct2DNormalize);
		}
	}

	// 本次迭代的目标点为终点 且水平距离大于终点的容忍误差，继续处理移动
	else if (InstanceData.bInLaneEndPoint == true && Direct2D.Size() > InstanceData.ReachToEndPointleranceDist)
	{
		if (UCharacterMovementComponent* CharacterMovement = OwnerCharacter->GetCharacterMovement())
		{
			CharacterMovement->AddInputVector(Direct2DNormalize);
		}
	}

	// 水平距离小于到达点迭代的距离，处理迭代
	if (Direct2D.Size() <= InstanceData.ReachRefreshTarLocRate * InstanceData.ReachPointToleranceDist)
	{
		//用当前目标导航点的位置计算下一个目标导航点
		InstanceData.CurTargetLocation = GetNextPointByNavPoints(InstanceData.CurTargetLocation, InstanceData.NavPath);

		//目标导航点无效
		if (InstanceData.CurTargetLocation.Equals(InvalidVector))
			return EStateTreeRunStatus::Failed;

		//下一个目标点为终点
		if (FVector::Dist2D(InstanceData.CurTargetLocation, InstanceData.TargetLocation) <= InstanceData.ReachPointToleranceDist)
			InstanceData.bInLaneEndPoint = true;

	}
	
	return EStateTreeRunStatus::Running;
}

TArray<FVector> FAIReachPointByNavTask::FindNavPathToTargetLocation(UObject* WorldObj, FVector StartPoint, FVector EndPoint) const
{
	/*
	暂时：垂直向下打一条射线，以获取实地面位置，射线半径暂时按身高来算
	*/
	StartPoint = FNavCommonTools::ConvertStandUpGroundPos(WorldObj, StartPoint, 200.f, 200.f);
	EndPoint = FNavCommonTools::ConvertStandUpGroundPos(WorldObj, EndPoint, 200.f, 200.f);

	TArray<FVector> PathPoints = FNavCommonTools::FindPathToLocation(WorldObj, StartPoint, EndPoint);

	if (PathPoints.IsEmpty())
		return PathPoints;

	TArray<FVector> Points = PathPoints;
	PathPoints.Empty();
	PathPoints.Emplace(Points[0]);
	FVector FromPos = Points[0];

	UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(WorldObj);
	ANavigationData* NavMesh = NavigationSystem->GetDefaultNavDataInstance();
	FSharedConstNavQueryFilter Filter = UNavigationQueryFilter::GetQueryFilter(*NavMesh, WorldObj, nullptr);

	for (int i = 2; i < Points.Num(); ++i)
	{
		FVector CurPos = Points[i];
		FVector HitLocation;
		if (NavMesh->Raycast(FromPos, CurPos, HitLocation, Filter))
		{
			PathPoints.Emplace(Points[i - 1]);
			FromPos = Points[i - 1] + (CurPos - Points[i - 1]).GetSafeNormal() * 0.01f;
		}
	}

	PathPoints.Emplace(Points[Points.Num() - 1]);
	PathPoints.Emplace(EndPoint);
	return PathPoints;
}

FVector FAIReachPointByNavTask::GetNextPointByNavPoints(const FVector& EntityLocation, const TArray<FVector>& NavPoints) const
{
	/*
	 * 沿着导航点走的逻辑
	 * 获取下一个目标点的规则：
	 * 把导航路径看成由很多有向线段组成的，把实体单位当前的位置投射到导航路径上，看在哪个有向线段上，取该有向线段的终点作为下一个目标点
	 *
	 */
	FVector AnsPoint = InvalidVector;
	if (NavPoints.Num() < 2)
	{
		return AnsPoint;
	}
	for (int i = 0; i < NavPoints.Num() - 1; i++)
	{
		FVector LocalStartPoint = NavPoints[i];
		FVector LocalEndpoint = NavPoints[i + 1];
		FVector LocalLine = LocalEndpoint - LocalStartPoint;
		FVector CheckVec = EntityLocation - LocalStartPoint;
		// 排除Z轴的影响
		LocalLine.Z = 0.f;
		CheckVec.Z = 0.f;
		float LineLength = LocalLine.Length();
		float ProjectDis = FVector::DotProduct(CheckVec, LocalLine.GetSafeNormal());
		if (ProjectDis < -0.1f)
		{
			AnsPoint = LocalStartPoint;
			break;
		}
		else if (ProjectDis < LineLength)
		{
			AnsPoint = LocalEndpoint;
			break;
		}
	}
	return AnsPoint;
}

