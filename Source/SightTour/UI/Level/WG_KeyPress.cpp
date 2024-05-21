// Fill out your copyright notice in the Description page of Project Settings.

#include "WG_KeyPress.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstance.h"

void UWG_KeyPress::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWG_KeyPress::SetContent(FText NewKeyName, UMaterialInstance* NewImage)
{
	if (T_KeyName)
	{
		T_KeyName->SetText(NewKeyName);
	}
	if (I_Image) 
	{
		I_Image->SetBrushFromMaterial(NewImage);
	}

}