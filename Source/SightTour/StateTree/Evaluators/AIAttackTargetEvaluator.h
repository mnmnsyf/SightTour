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
#include "AIAttackTargetEvaluator.generated.h"

USTRUCT()
struct SIGHTTOUR_API FAIAttackTargetEvaluatorInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter, meta = (DisplayName = "攻击距离"))
	float AttackRange = 100.f;

	UPROPERTY(EditAnywhere, Category = Output, meta = (DisplayName = "是否开始攻击"))
	bool bAttack = false;

	UPROPERTY(EditAnywhere, Category = Output, DisplayName = "攻击动画")
	UAnimMontage* Anim = nullptr;

	UPROPERTY(EditAnywhere, Category = Output, DisplayName = "AttackSectionName")
	FName AttackSectionName = NAME_None;
};

USTRUCT(DisplayName = "AI Attack Target")
struct SIGHTTOUR_API FAIAttackTargetEvaluator : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FAIAttackTargetEvaluatorInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

};
