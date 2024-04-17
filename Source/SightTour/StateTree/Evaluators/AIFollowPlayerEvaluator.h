/**********************************************      
        @File                AIFollowPlayerEvaluator.h
        @Brief                AI是否处于跟随状态估值器
        @Author                SheYongfei
        @Version        V1.0
        @Date                2024/03/24
        @History changes
                V1.0        NoChange
**********************************************/

#pragma once

#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "../StateTreeAIDefine.h"
#include "AIFollowPlayerEvaluator.generated.h"

USTRUCT()
struct SIGHTTOUR_API FAIFollowPlayerEvaluatorInstanceData
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, Category = Parameter, meta = (DisplayName = "最大跟随范围"))
	float MaxRange = 600.f;

	UPROPERTY(EditAnywhere, Category = Parameter, meta = (DisplayName = "最小跟随范围"))
	float MinRange = 100.f;

	UPROPERTY(EditAnywhere, Category = Parameter, meta = (DisplayName = "显示Debug"))
	bool bShowDebug = false;

	UPROPERTY(EditAnywhere, Category = Output, meta = (DisplayName = "跟随目标位置"))
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = Output, meta = (DisplayName = "AI是否需要跟随玩家"))
	bool bFollowTarget = false;
};

USTRUCT(DisplayName = "AI Follow Player")
struct SIGHTTOUR_API FAIFollowPlayerEvaluator : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FAIFollowPlayerEvaluatorInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

};
