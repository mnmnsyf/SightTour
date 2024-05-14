// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "WG_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API UWG_MainMenu : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HB_InventoryBox;

protected:
	void NativeConstruct() override;

	
};
