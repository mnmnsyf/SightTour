// Fill out your copyright notice in the Description page of Project Settings.

#include "WG_GameFinish.h"
#include "UI/Subsystem/SightTourUIManager.h"
#include "CommonUIExtensions.h"
#include "Input/CommonUIInputTypes.h"
#include "NativeGameplayTags.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_Menu, "UI.Layer.Menu");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_SPACEBAR, "UI.Action.Spacebar");

UWG_GameFinish::UWG_GameFinish(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UWG_GameFinish::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_ACTION_SPACEBAR), false, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleSpacebarAction)));
}

void UWG_GameFinish::HandleSpacebarAction()
{
	if (ensure(!SpacebarClass.IsNull()))
	{
		ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
		USightTourUIManager::Get()->PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_LAYER_Menu, SpacebarClass);
	}
}