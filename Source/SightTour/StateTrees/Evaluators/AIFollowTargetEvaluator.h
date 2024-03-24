/**********************************************      
        @File                AIFollowTargetEvaluator.h
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
#include "AIFollowTargetEvaluator.generated.h"

USTRUCT()
struct SIGHTTOUR_API FAIFollowTargetEvaluatorInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Output, meta = (DisplayName = "AI是否需要跟随目标"))
	bool bFollowTarget = false;

	UPROPERTY(EditAnywhere, Category = Output, meta = (DisplayName = "跟随所需数据"))
	FAIFollowStruct AIFollowStruct;

	UPROPERTY(EditAnywhere, Category = Parameter, DisplayName = "显示Debug")
	bool bShowDebug = false;
};

USTRUCT(DisplayName = "AI Follow Target")
struct SIGHTTOUR_API FAIFollowTargetEvaluator : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FAIFollowTargetEvaluatorInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

};
