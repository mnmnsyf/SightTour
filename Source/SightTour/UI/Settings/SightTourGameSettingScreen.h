// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameSettingRegistry.h"
#include "GameSettingRegistryChangeTracker.h"

#include "SightTourGameSettingScreen.generated.h"

class UGameSetting;
class UGameSettingCollection;
class UObject;
class UWidget;
class SettingScreenTopOne;
struct FFrame;

enum class EGameSettingChangeReason : uint8;

/**
 * 游戏设置的TopUI
 */
UCLASS()
class SIGHTTOUR_API USightTourGameSettingScreen : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ShowMaxItemNum = 5;

	UPROPERTY(Meta = (BindWidget))
	class UCanvasPanel* Panel_Content;

public:
	TArray<class SettingScreenTopOne*> TopArr;

protected:
	int32 NowSelect = -1;
	int32 NumOfTop;
};
