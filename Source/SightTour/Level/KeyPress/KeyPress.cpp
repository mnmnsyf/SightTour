// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyPress.h"
#include "KeyPressDefines.h"
#include "UI/Level/WG_KeyPress.h"
#include "System/SightTourGameInstance.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/Image.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../Player/SightTourCharacter.h"

AKeyPress::AKeyPress()
{
 	PrimaryActorTick.bCanEverTick = true;
}

void AKeyPress::BeginPlay()
{
	Super::BeginPlay();
	
	InitConfig();
}

void AKeyPress::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bStartPlay)
	{
		return;
	}

	RemainTime -= DeltaTime;
	if (RemainTime > 0)
	{
		return;
	}
	RemainTime = UpdateTime;

	UpdateNextKey();
}

bool AKeyPress::InitConfig()
{
	APlayerController* PC = USightTourGameInstance::Get()->GetCurPlayerController();
	if (!PC)
	{
		return false;
	}

	//IA Binding
	if (UEnhancedInputComponent* EIComponent = Cast<UEnhancedInputComponent>(PC->InputComponent))
	{
		for (auto& EachConfig : KeyConfig)
		{
			if (EachConfig.IA.IsNull() || IsValid(EachConfig.Image))
			{
				continue;
			}

			UInputAction* IA = EachConfig.IA.LoadSynchronous();
			check(IA);

			auto& Binding = EIComponent->BindAction(IA, ETriggerEvent::Triggered, this, &AKeyPress::CheckAnswerKey);
		}
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 2);

		return true;
	}

	return false;
}

bool AKeyPress::UpdateNextKey()
{
	APlayerController* PC = USightTourGameInstance::Get()->GetCurPlayerController();
	if (!PC)
	{
		return false;
	}

	if (!KeyPressUI && ensure(KeyPressClass))
	{
		KeyPressUI = CreateWidget<UWG_KeyPress>(PC, KeyPressClass);
		if(KeyPressUI)
			KeyPressUI->AddToPlayerScreen(1000);
	}
	if (!KeyPressUI)
	{
		return false;
	}

	int32 RandomIndex = FMath::RandRange(0, KeyConfig.Num() - 1);
	FKeyPressConfig& NewContent = KeyConfig[RandomIndex];
	if (NewContent.IA.IsNull() || IsValid(NewContent.Image))
	{
		return false;
	}

	UInputAction* IA = NewContent.IA.LoadSynchronous();
	check(IA);

	//update UI
	KeyPressUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	KeyPressUI->SetContent(IA->ActionDescription, NewContent.Image);
	
	//Set Timer
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AKeyPress::WrongCallback, CheckTime, false);

	return true;
}

void AKeyPress::CheckAnswerKey(const FInputActionInstance& InputActionInstance)
{
	//Close UI
	KeyPressUI->SetVisibility(ESlateVisibility::Collapsed);

	//remove TimerHandle
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AKeyPress::WrongCallback()
{
	//回答错误玩家扣血
	if (APawn* Player = USightTourGameInstance::Get()->GetCurPawn())
	{
		if (ASightTourCharacter* STCharacter = Cast<ASightTourCharacter>(Player))
		{
			STCharacter->ReduceHealth(ReduceHP);
		}
	}

	//Close UI
	if (KeyPressUI->GetVisibility() != ESlateVisibility::Collapsed)
	{
		KeyPressUI->SetVisibility(ESlateVisibility::Collapsed);
	}

	//remove TimerHandle
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

