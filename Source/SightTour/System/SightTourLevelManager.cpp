// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SightTourLevelManager.h"
#include "SightTourGameInstance.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

USightTourLevelManager* USightTourLevelManager::Instance = nullptr;

void USightTourLevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Instance = this;
	Super::Initialize(Collection);
}

bool USightTourLevelManager::LoadNewLevel(FName SceneName)
{
	//TODO:start acting

	if (SceneName == OldSceneName)
	{
		//TODO:end acting
		return false;
	}

	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	check(CameraManager);

	CameraManager->StartCameraFade(0.f, 1.f, 0.f, FLinearColor::Black, true, true);

	//TODO:widget visible


}