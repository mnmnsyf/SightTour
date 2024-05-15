// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "SightTourHUD.generated.h"


UCLASS(Config = Game)
class SIGHTTOUR_API ASightTourHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ASightTourHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	TArray<int32> CalculateSurroundSide(int32 Num);

	UFUNCTION(BlueprintCallable)
	void DrawSurroundSide(int32 Num);

	void Test();

	void Test2(int32 Width, int32 Height, const TArray<FColor>& Colors);
};
