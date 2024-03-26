// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "SightTourSettingsLocal.generated.h"

/**
 * USightTourSettingsLocal
 */
UCLASS()
class SIGHTTOUR_API USightTourSettingsLocal : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	USightTourSettingsLocal();

	static USightTourSettingsLocal* Get();
	
};
