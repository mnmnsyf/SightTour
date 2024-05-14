// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SighTourPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API ASighTourPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
protected:
	virtual void SetupInputComponent() override;

public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
};
