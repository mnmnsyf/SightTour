// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SightTourLevelManager.generated.h"

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API USightTourLevelManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//-------------------------内部流程接口，外部人员不需关心 start------------------------------------

	USightTourLevelManager() { }

	void Initialize(FSubsystemCollectionBase& Collection) override;

	static USightTourLevelManager* Get() { return Instance; }

	//-------------------------内部流程接口，外部人员不需关心 end------------------------------------

public:
	//-------------------------关卡流程相关接口------------------------------------
	
	// 加载关卡
	bool LoadNewLevel(FName SceneName);


private:
	//-------------------------关卡数据相关------------------------------------
	//旧的场景名字
	FName OldSceneName;
	//新场景名字
	FName NewSceneName;

private:
	static USightTourLevelManager* Instance;
};
