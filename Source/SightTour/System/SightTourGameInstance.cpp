// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SightTourGameInstance.h"
#include "../UI/SightTourHUD.h"
#include "Kismet/GameplayStatics.h"

USightTourGameInstance::USightTourGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

int32 USightTourGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	return Super::AddLocalPlayer(NewPlayer, UserId);
}
