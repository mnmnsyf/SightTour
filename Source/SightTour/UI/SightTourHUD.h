// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"

#include "SightTourHUD.generated.h"

class UPrimaryGameLayout;
class ULocalPlayer;
/**
 * 
 */
USTRUCT()
struct FLyraHUDLayoutRequest
{
	GENERATED_BODY()

	// The layout widget to spawn
	UPROPERTY(EditAnywhere, Category = UI/*, meta = (AssetBundles = "Client")*/)
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	// The layer to insert the widget in
	UPROPERTY(EditAnywhere, Category = UI, meta = (Categories = "UI.Layer"))
	FGameplayTag LayerID;
};

UCLASS(Config = Game)
class SIGHTTOUR_API ASightTourHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ASightTourHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void BeginPlay() override;

public:
	void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);

	//TODO 应该把接口挪到UIManager
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	void PushStreamedContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, UPARAM(meta = (AllowAbstract = false)) TSoftClassPtr<UCommonActivatableWidget> WidgetClass);

	void AddLayoutToViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

protected:
	void CreateLayoutWidget(ULocalPlayer* LocalPlayer);
	TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass();

private:
	//添加HUDLayout栈
	void PushWidgetToLayerStack();

private:
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPrimaryGameLayout> PrimaryLayoutClass;

	// Layout to add to the HUD
	UPROPERTY(EditAnywhere, Category = UI, meta = (TitleProperty = "{LayerID} -> {LayoutClass}"))
	TArray<FLyraHUDLayoutRequest> HUDLayout;

private:
	UPROPERTY()
	UPrimaryGameLayout* PrimaryLayout;
};
