// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentManagerComponent.h"
#include "TP_WeaponComponent.h"
#include "../PickupItemDefines.h"

UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

UEquipmentManagerComponent* UEquipmentManagerComponent::Get(AActor* Owner)
{
	return Owner->FindComponentByClass<UEquipmentManagerComponent>();
}

void UEquipmentManagerComponent::PickupFillBall(AFillBall* Ball)
{
	for (AFillBall* EachBall : BallList)
	{
		if (EachBall->FillBallConfig == Ball->FillBallConfig)
		{
			EachBall->FillBallConfig.ChangeValue(Ball->FillBallConfig.GetActualValue());
			Ball->Destroy();
			return;
		}
	}

	BallList.Emplace(Ball);
}

AFillBall* UEquipmentManagerComponent::DiscardFillBall(FName BallType)
{
	for (AFillBall* EachBall : BallList)
	{
		if (EachBall->FillBallConfig.ItemTypeName == BallType)
		{
			return EachBall;
		}
	}
	return nullptr;
}