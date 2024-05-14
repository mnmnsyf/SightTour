// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SighTourPlayerController.h"
#include "EnhancedInputSubsystems.h"

void ASighTourPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const ULocalPlayer* LP = GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* EISubsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(EISubsystem);

	EISubsystem->ClearAllMappings();

	// Actually add the config to the local player,添加本地玩家的按键配置，bNotifyUserSettings设为true	
	FModifyContextOptions Options = {};
	Options.bIgnoreAllPressedKeysUntilRelease = false;
	Options.bNotifyUserSettings = true;
	EISubsystem->AddMappingContext(DefaultMappingContext, 0, Options);
}
