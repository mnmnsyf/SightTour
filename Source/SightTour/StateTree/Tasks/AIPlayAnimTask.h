/**********************************************      
        @File                AIPlayAnimTask.h
        @Brief                AI播放动画
        @Author                SheYongfei
        @Version        V1.0
        @Date                2024/4/17
        @History changes
                V1.0        NoChange
**********************************************/

#pragma once

#include "StateTreeTaskBase.h"
#include "AIPlayAnimTask.generated.h"

class UAnimMontage;


#define InvalidVector FVector::ZeroVector

UCLASS()
class SIGHTTOUR_API UAIPlayAnimTaskInstanceData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Parameter, DisplayName = "蒙太奇动画")
	UAnimMontage* MontageAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = Parameter, DisplayName = "动画Section")
	FName AnimSectionName = NAME_None;

	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bWaitForNotifyEventToEnd = false;

	/** When bWaitForNotifyEventToEnd is set to true this is the name of the notify that we will look for to signal the end of the task */
	UPROPERTY(EditAnywhere, Category = Parameter, meta = (EditCondition = "bWaitForNotifyEventToEnd"))
	FName NotifyEventNameToEnd = NAME_None;

	/** How many times the animation should be run before completing the task. */
	UPROPERTY(EditAnywhere, Category = Parameter, meta = (EditCondition = "!bLoopForever", ClampMin = "1"))
	int32 LoopsToRun = 1;

	/** If true the animation will loop forever until a transition stops it. */
	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bLoopForever = false;

	TOptional<float> TimeBeforeStartingNewLoop;

	//播放次数
	int32 CompletedLoops = 0;

	/** It can happen that the montage is interrupted, in which case we want to fail the task. */
	bool bMontageInterrupted = false;

	/** Handling of specific case: when we forcibly start a new montage from a notify event, an interruption will happen from the previous event, so we need to make sure to ignore the first occurrence. */
	bool bExpectInterruptionFromNotifyEventLoop = false;


	EStateTreeRunStatus OnEnterState(const FStateTreeExecutionContext& Context);

	EStateTreeRunStatus OnTick(const FStateTreeExecutionContext& Context, float DeltaTime);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* const EndedMontage, const bool bInterrupted);

	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

};

USTRUCT(DisplayName = " Play Anim")
struct SIGHTTOUR_API FAIPlayAnimTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using UInstanceDataType = UAIPlayAnimTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return UInstanceDataType::StaticClass(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
