// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItemBase.generated.h"

class UTP_PickUpComponent;

UCLASS()
class SIGHTTOUR_API APickupItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnEquipped() {};
	virtual void OnUnequipped() {};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
};
