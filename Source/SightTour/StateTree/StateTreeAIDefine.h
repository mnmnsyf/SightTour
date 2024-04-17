
#pragma once

#include "CoreMinimal.h"
#include "StateTreeAIDefine.generated.h"

namespace STBBC
{
	static const FName STBB_PARAM_Follow = TEXT("Follow");						//AI跟随

	static const FName STBB_PARAM_Attack = TEXT("Attack");						
};

//AI Follow 所需数据结构
USTRUCT(Blueprintable)
struct FAIFollowStruct
{
	GENERATED_USTRUCT_BODY()

	FAIFollowStruct()
	{
		Reset();
	}

	void Reset()
	{
		FollowPointRelativeTransform = FTransform::Identity;
		FollowPointWorldTransform = FTransform::Identity;

		ReCalcFollowPointDistRate = 1.2f;
		bReCalcFollowPoint = true;
	}

	//跟随位置 相对AI 的Transform
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, Category = "AIFollow")
	FTransform FollowPointRelativeTransform;

	//跟随位置 世界坐标 Transform
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, Category = "AIFollow")
	FTransform FollowPointWorldTransform;

	//重新计算跟随点的倍率
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, Category = "AIFollow", meta = (EnumCondition = 1, ClampMin = "1.0", ClampMax = "100.0"))
	float ReCalcFollowPointDistRate;

	//是否计算跟随点(Dirty位)
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, Category = "AIFollow")
	bool bReCalcFollowPoint;
};