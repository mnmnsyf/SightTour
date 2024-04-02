// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SightTourUIManager.generated.h"

class UUserWidget;

/**
 * UI管理系统
 */
UCLASS()
class SIGHTTOUR_API USightTourUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USightTourUIManager();

	//static USightTourUIManager* Get();

private:
	bool bInitialized;

};
