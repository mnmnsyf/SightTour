// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SightTourLevelDefines.h"
#include "Engine/DataTable.h"
#include "SightTourGameInstance.generated.h"

//DECLARE_DELEGATE_OneParam(FOnGameOverDelegate, bool);

UCLASS()
class SIGHTTOUR_API USightTourGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USightTourGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void Init() override;

public:
	//----------------------- 查询相关接口 ------------------------------------
	static USightTourGameInstance* Get() { return Instance; }
	static APlayerController* GetCurPlayerController(); 
	static APawn* GetCurPawn();

	UFUNCTION(BlueprintPure, Category = "STGameInstance")
	USightTourUIManager* GetMOUIManager() const { return SightTourUIManager; }

	int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;

	//---------------------------游戏流程相关  ----------------------------------
public:
	void FinishCurrentLevel();

	void LoseCurrentLevel();

	UFUNCTION(BlueprintCallable, Category = "SightTour")
	bool UpdateNextLevelName();

	UFUNCTION(BlueprintCallable)
	void TransitToNextLevel();

	UFUNCTION(BlueprintCallable)
	void RestartCurrentLevel();


private:
	UFUNCTION(BlueprintCallable, Category = "SightTour")
	void InnerReloadCurrentLevel();

	//六送到下个关卡
	UFUNCTION(BlueprintCallable, Category = "SightTour")
	void InnerLoadNextLevel();

	UFUNCTION(BlueprintCallable, Category = "SightTour")
	void InnerInitLevelData();

	UFUNCTION(BlueprintCallable, Category = "SightTour")
	void InnerUnloadLastLevel();

public:
	//--------------------------- 游戏流程相关数据 ------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Category = "SightTour | Level"))
	TSoftClassPtr<class UCommonActivatableWidget> GameFinishClass;

	//流关卡数据
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Category = "SightTour | Level", RowType = "/Script/SightTour.SightTourLevelConfig"))
	UDataTable* GameLevelData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CurrentLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LastLevelName;

	TMap<FName, FSightTourLevelConfig> GameLevelConfigs;

	// 当前问题敌人在表格中索引
	int32 ProblemEnemyIndex = 0;

public:
	//  ------------------------- UI相关数据 -----------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Category = "SightTour | UI"))
	TSoftClassPtr<class UPrimaryGameLayout> PrimaryLayoutClass;

	// Layout to add to the HUD
	UPROPERTY(EditAnywhere, meta = (TitleProperty = "{LayerID} -> {LayoutClass}", Category = "SightTour | UI"))
	TArray<struct FSightTourHUDLayoutRequest> HUDLayout;

private:
	UPROPERTY()//使用UPROPERTY防止在通过OpenLevel切换关卡时被释放
	class USightTourUIManager* SightTourUIManager = nullptr;



private:
	static USightTourGameInstance* Instance;
};
