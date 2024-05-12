// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SightTourGameInstance.h"
#include "../UI/SightTourHUD.h"
#include "Kismet/GameplayStatics.h"
#include "../UI/Subsystem/SightTourUIManager.h"

USightTourGameInstance* USightTourGameInstance::Instance = nullptr;

USightTourGameInstance::USightTourGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USightTourGameInstance::Init()
{
	Instance = this;
	Super::Init();
}

int32 USightTourGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);

	/*if (ReturnVal != INDEX_NONE)
	{
		GetSubsystem<USightTourUIManager>()->NotifyPlayerAdded(NewPlayer);
	}*/
	

	return ReturnVal;
}