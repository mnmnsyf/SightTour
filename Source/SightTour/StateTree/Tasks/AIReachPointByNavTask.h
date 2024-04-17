/**********************************************      
        @File                AIReachPointByNavTask.h
        @Brief                导航到达目标点,不依赖AI
        @Author                SheYongfei
        @Version        V1.0
        @Date                2023/12/20
        @History changes
                V1.0        NoChange
**********************************************/

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "AIReachPointByNavTask.generated.h"

#define InvalidVector FVector::ZeroVector

USTRUCT()
struct SIGHTTOUR_API FAIReachPointByNavTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter, DisplayName = "到达点位置")
	FVector TargetLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = Parameter, DisplayName = "到达终点容忍误差")
	float ReachToEndPointleranceDist = 10.0f;

	UPROPERTY(EditAnywhere, Category = Parameter, DisplayName = "到达点容忍误差")
	float ReachPointToleranceDist = 30.0f;

	UPROPERTY(EditAnywhere, Category = Parameter, DisplayName = "到达点迭代位置倍率")
	float ReachRefreshTarLocRate = 1.5f;

	UPROPERTY(EditAnywhere, Category = Parameter, DisplayName = "每次迭代距离")
	float MoveDistance = 200.0f;

	UPROPERTY(EditAnywhere, Category = Parameter, DisplayName = "移动时间, 为负，无限时间")
	float MoveTime = -1.f;

	UPROPERTY(EditAnywhere, Category = Parameter, DisplayName = "Debug显示")
	bool bShowDebug = false;

	UPROPERTY(EditAnywhere, Category = Parameter, DisplayName = "是否清除STBB移动到目标点结构体")
	bool bClearMovePointStruct = false;

	//依赖导航计算得出的路径
	UPROPERTY() 
	TArray<FVector> NavPath;

	//当前遍历的路径下标
	UPROPERTY()
	int32 PathIndex = 0;

	// 是否遍历到路径最后一个点
	bool bInLaneEndPoint = false;

	// 缓存到达点位置，如果到达点位置发生改变，则更新导航路径
	UPROPERTY()
	FVector HistoryTargetLocation = FVector(0.0f, 0.0f, 0.0f);

	// 若移动时间为正，此为记录的剩余的时间
	UPROPERTY()
	float RemainedTime = 0.0f;

	//本次跟随的目标导航点
	UPROPERTY()
	FVector CurTargetLocation = FVector(0.0f, 0.0f, 0.0f);;
};

USTRUCT(DisplayName = " Reach Point By Nav")
struct SIGHTTOUR_API FAIReachPointByNavTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FAIReachPointByNavTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void StateCompleted(FStateTreeExecutionContext& Context, const EStateTreeRunStatus CompletionStatus, const FStateTreeActiveStates& CompletedActiveStates) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

private:
	TArray<FVector> FindNavPathToTargetLocation(UObject* WorldObj, FVector StartPoint, FVector EndPoint) const;
	FVector GetNextPointByNavPoints(const FVector& EntityLocation, const TArray<FVector>& NavPoints) const;
};
