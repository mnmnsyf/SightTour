// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SightTourGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API USightTourGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USightTourGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void Init() override;

	static USightTourGameInstance* Get() { return Instance; }
	static APlayerController* GetCurPlayerController(); 
	static APawn* GetCurPawn();
	// 增加一个蓝图可调用的接口
	UFUNCTION(BlueprintPure, Category = "STGameInstance")
	USightTourUIManager* GetMOUIManager() const { return SightTourUIManager; }

	//~ Begin UGameInstance interface
	int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
	//~ End UGameInstance interface


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "PrimaryLayoutClass", Category = "UI"))
	TSoftClassPtr<class UPrimaryGameLayout> PrimaryLayoutClass;

	// Layout to add to the HUD
	UPROPERTY(EditAnywhere, Category = UI, meta = (TitleProperty = "{LayerID} -> {LayoutClass}", Category = "UI"))
	TArray<struct FSightTourHUDLayoutRequest> HUDLayout;

private:
	UPROPERTY()//使用UPROPERTY防止在通过OpenLevel切换关卡时被释放
	class USightTourUIManager* SightTourUIManager = nullptr;



private:
	static USightTourGameInstance* Instance;
};
