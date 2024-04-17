/**********************************************      
        @File                AIMoveToTask.h
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
#include "AIMoveToTask.generated.h"

#define InvalidVector FVector::ZeroVector

USTRUCT()
struct SIGHTTOUR_API FAIMoveToTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter, DisplayName = "目标位置")
	FVector TargetLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = Parameter, DisplayName = "容忍距离")
	float AcceptanceRadius = 100.f;
};

USTRUCT(DisplayName = " AI MoveTo Player")
struct SIGHTTOUR_API FAIMoveToTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FAIMoveToTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
