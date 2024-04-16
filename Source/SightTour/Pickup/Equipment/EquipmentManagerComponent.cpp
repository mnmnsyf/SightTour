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
	TArray<int32> SlotIndexs = { 0, 1, 2, 3, 4 };

	FFillBallBase* TempFillBall = Ball->FillBallConfig.GetMutablePtr<FFillBallBase>();
	if (TempFillBall)
	{
		for (auto&& [BallIndex, EachBall] : BallMap)
		{
			SlotIndexs.Remove(BallIndex);
			FFillBallBase* EachBallConfig = EachBall.GetMutablePtr<FFillBallBase>();
			if (EachBallConfig)
			{
				if (EachBallConfig->ItemTypeName == TempFillBall->ItemTypeName)
				{
					EachBallConfig->ChangeValue(TempFillBall->GetActualValue());
					Ball->Destroy();

					PickupBallDelegate.Broadcast(true, BallIndex, EachBallConfig->GetActualValue(), EachBallConfig->ItemTypeName);
					return;
				}
			}
		}

		// 没有相同类型的情况
		if (SlotIndexs.Num() > 0)
		{
			BallMap.Emplace(SlotIndexs[0], Ball->FillBallConfig);
			PickupBallDelegate.Broadcast(true, SlotIndexs[0], TempFillBall->GetActualValue(), TempFillBall->ItemTypeName);
		}
		Ball->Destroy();
	}
}

FInstancedStruct UEquipmentManagerComponent::DiscardFillBall()
{
	FInstancedStruct ReturnBallConfig;

	for (auto It = BallMap.CreateIterator(); It; ++It)
	{
		FFillBallBase* EachBallConfig = It->Value.GetMutablePtr<FFillBallBase>();
		if (EachBallConfig && EachBallConfig->ItemTypeName == GetCurrentBallType())
		{
			ReturnBallConfig = It->Value;
			PickupBallDelegate.Broadcast(false, It->Key, EachBallConfig->GetActualValue(), EachBallConfig->ItemTypeName);
			It.RemoveCurrent();
			break;
		}
	}

	return ReturnBallConfig;
}

void UEquipmentManagerComponent::ChangeSlot(int32 NewSlot)
{
	ChangeSlotDelegate.Broadcast(CurrentSlot, NewSlot);
	CurrentSlot = NewSlot;
}

FName UEquipmentManagerComponent::GetCurrentBallType() const
{
	FName ReturnName;
	if (const FInstancedStruct* BallStruct = BallMap.Find(CurrentSlot))
	{
		if (const FFillBallBase* CurrentBallConfig = BallStruct->GetPtr<FFillBallBase>())
		{
			ReturnName = CurrentBallConfig->ItemTypeName;
		}
	}
	return ReturnName;
}
