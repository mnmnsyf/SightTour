// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentManagerComponent.h"
#include "TP_WeaponComponent.h"
#include "../PickupItemDefines.h"

// Sets default values for this component's properties
UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UEquipmentManagerComponent::UpdateDigitalProjectileValue(const float UpdateValue)
{
	if (CurrentEquippedWeapon == nullptr)
	{
		const FString ErrorStr = FString::Printf(TEXT("There is an error in OwnerWeapon!"));
		UE_LOG(LogTemp, Error, TEXT("%s:%d %s"), *FString(__FUNCTION__), __LINE__, *ErrorStr);
		return;
	}
	switch (CurrentEquippedWeapon->WeaponType)
	{
		case EWeaponType::AdditionGun:
		{
			DigitalProjectileValue += UpdateValue;
			break;
		}
		case EWeaponType::DivisionGun:
		{
			DigitalProjectileValue -= UpdateValue;
			break;
		}
		case EWeaponType::MultiplicationGun:
		{
			DigitalProjectileValue *= UpdateValue;
			break;
		}
		case EWeaponType::SubtractionGun:
		{
			if(UpdateValue != 0)
				DigitalProjectileValue /= UpdateValue;
			break;
		}
		default:
			break;
	}
}

