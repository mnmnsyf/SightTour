// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "KeyPressDefines.generated.h"

USTRUCT(BlueprintType)
struct FKeyPressConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "按键"))
	TSoftObjectPtr<class UInputAction> IA;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "图片"))
	TObjectPtr<UTexture2D> Image = nullptr;
};