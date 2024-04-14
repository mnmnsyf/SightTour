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
	FFillBallBase* TempFillBall = Ball->FillBallConfig.GetMutablePtr<FFillBallBase>();
	if (TempFillBall)
	{
		for (FInstancedStruct EachBall : BallList)
		{
			FFillBallBase* EachBallConfig = EachBall.GetMutablePtr<FFillBallBase>();
			if (EachBallConfig)
			{
				if (EachBallConfig->ItemTypeName == TempFillBall->ItemTypeName)
				{
					EachBallConfig->ChangeValue(TempFillBall->GetActualValue());
					Ball->Destroy();
					return;
				}
			}
		}
		BallList.Emplace(Ball->FillBallConfig);
		Ball->Destroy();
	}
}

FInstancedStruct UEquipmentManagerComponent::DiscardFillBall(FName BallType)
{
	FInstancedStruct ReturnBallConfig;

	for (auto It = BallList.CreateIterator(); It; ++It)
	{
		FFillBallBase* EachBallConfig = It->GetMutablePtr<FFillBallBase>();
		if (EachBallConfig && EachBallConfig->ItemTypeName == BallType)
		{
			ReturnBallConfig = *It;
			It.RemoveCurrent();
			break;
		}
	}

	return ReturnBallConfig;
}