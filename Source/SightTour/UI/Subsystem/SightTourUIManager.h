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
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	void PushStreamedContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, UPARAM(meta = (AllowAbstract = false)) TSoftClassPtr<UCommonActivatableWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	UCommonActivatableWidget* PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UCommonActivatableWidget> WidgetClass);

	void AddLayoutToViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

protected:
	void CreateLayoutWidget(ULocalPlayer* LocalPlayer);

	TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass();

private:
	bool bInitialized;

private:
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPrimaryGameLayout> PrimaryLayoutClass;

	// Layout to add to the HUD
	UPROPERTY(EditAnywhere, Category = UI, meta = (TitleProperty = "{LayerID} -> {LayoutClass}"))
	TArray<FSightTourHUDLayoutRequest> HUDLayout;

	UPROPERTY()
	UPrimaryGameLayout* PrimaryLayout;
};
