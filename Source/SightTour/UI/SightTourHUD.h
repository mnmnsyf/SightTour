// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SightTourHUD.generated.h"

class UPrimaryGameLayout;
class ULocalPlayer;
/**
 * 
 */
UCLASS(Config = Game)
class SIGHTTOUR_API ASightTourHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ASightTourHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void BeginPlay() override;

public:
	void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);

protected:
	void AddLayoutToViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

	void CreateLayoutWidget(ULocalPlayer* LocalPlayer);
	TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass();

private:
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPrimaryGameLayout> PrimaryLayoutClass;
};
