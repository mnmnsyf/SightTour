// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SightTourGameInstance.h"
#include "UI/SightTourHUD.h"
#include "UI/Subsystem/SightTourUIManager.h"
#include "Kismet/GameplayStatics.h"

USightTourGameInstance* USightTourGameInstance::Instance = nullptr;

USightTourGameInstance::USightTourGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USightTourGameInstance::Init()
{
	Instance = this;
	Super::Init();

	SightTourUIManager = USightTourUIManager::Get();
}

APlayerController* USightTourGameInstance::GetCurPlayerController()
{
	if (Instance == nullptr)
		return nullptr;
	UWorld* UEWorld = Instance->GetWorld();
	if (UEWorld == nullptr)
		return nullptr;
	return UEWorld->GetFirstPlayerController();
}

APawn* USightTourGameInstance::GetCurPawn()
{
	APlayerController* CurPlayerController = GetCurPlayerController();
	if (CurPlayerController == nullptr)
		return nullptr;
	return CurPlayerController->GetPawn();
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