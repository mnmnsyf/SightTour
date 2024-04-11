// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "PrimaryGameLayout.h"

#include "SightTourUIManager.generated.h"

class UUserWidget;
class UCommonActivatableWidget;
class ULocalPlayer;

/**
 * UI超级管理系统
 */
USTRUCT()
struct FSightTourHUDLayoutRequest
{
	GENERATED_BODY()

	// The layout widget to spawn
	UPROPERTY(EditAnywhere, Category = UI/*, meta = (AssetBundles = "Client")*/)
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	// The layer to insert the widget in
	UPROPERTY(EditAnywhere, Category = UI, meta = (Categories = "UI.Layer"))
	FGameplayTag LayerID;
};

UCLASS()
class SIGHTTOUR_API USightTourUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USightTourUIManager();

	static USightTourUIManager* Get();

public:
	void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	void PushStreamedContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, UPARAM(meta = (AllowAbstract = false)) TSoftClassPtr<UCommonActivatableWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	UCommonActivatableWidget* PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UCommonActivatableWidget> WidgetClass);

protected:
	void CreatePrimaryLayout(ULocalPlayer* LocalPlayer);

	TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass();

	void AddPrimaryGameLayoutToViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

	/** 添加HUDLayout栈到PrimaryLayout中 */
	void PushHUDLayerToPrimaryLayoutStack();

private:
	bool bInitialized;

private:
	UPROPERTY()
	UPrimaryGameLayout* PrimaryLayout;
};
