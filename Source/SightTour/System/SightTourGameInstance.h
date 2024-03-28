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

	int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;

};