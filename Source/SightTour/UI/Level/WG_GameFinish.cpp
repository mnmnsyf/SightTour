// Fill out your copyright notice in the Description page of Project Settings.

#include "WG_GameFinish.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UWG_GameFinish::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWG_GameFinish::SetContent(FText NewKeyName, UTexture2D* NewImage)
{
	if (T_KeyName)
	{
		T_KeyName->SetText(NewKeyName);
	}
	if (I_Image) 
	{
		I_Image->SetBrushFromTexture(NewImage);
	}

}