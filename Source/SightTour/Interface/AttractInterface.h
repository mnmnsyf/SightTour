// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIGHTTOUR_API IAttractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Attract(class UTP_WeaponComponent* WeaponComp) = 0;

	virtual void Spawn(class UTP_WeaponComponent* WeaponComp) = 0;

	virtual void OnEquipped() {};

	virtual void UnEquipped() {};
};
