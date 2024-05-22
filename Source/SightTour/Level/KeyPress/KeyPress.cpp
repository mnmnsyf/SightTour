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
#include "Player/SightTourCharacter.h"

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

	UpdateNextKey(DeltaTime);
}

void AKeyPress::SetUIHidden(bool bHide)
{
	bHiddenUI = bHide;
	if (KeyPressUI)
	{
		ESlateVisibility Vis = bHiddenUI ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
		KeyPressUI->SetVisibility(Vis);
	}
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
		for (auto& EachIA : InputActions)
		{
			if (EachIA.IsNull())
			{
				continue;
			}

			UInputAction* IA = EachIA.LoadSynchronous();
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

bool AKeyPress::UpdateNextKey(float DeltaTime)
{
	if (!bStartPlay || bHiddenUI)
	{
		return false;
	}

	RemainTime -= DeltaTime;
	if (RemainTime > 0)
	{
		return false;
	}
	RemainTime = UpdateTime;

	APlayerController* PC = USightTourGameInstance::Get()->GetCurPlayerController();
	if (!PC)
	{
		return false;
	}

	int32 KeyIndex = FMath::RandRange(0, InputActions.Num() - 1);
	int32 ImageIndex = FMath::RandRange(0, Images.Num() - 1);

	if (InputActions[KeyIndex].IsNull() || !IsValid(Images[ImageIndex]))
	{
		ensure(false);
		return false;
	}

	UInputAction* IA = InputActions[KeyIndex].LoadSynchronous();
	check(IA);
	//update UI
	GetKeyPressUI()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	GetKeyPressUI()->SetContent(IA->ActionDescription, Images[ImageIndex].Get());
	
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

TObjectPtr<UWG_KeyPress> AKeyPress::GetKeyPressUI()
{
	APlayerController* PC = USightTourGameInstance::Get()->GetCurPlayerController();
	if (!PC)
	{
		return nullptr;
	}

	if (!KeyPressUI && ensure(KeyPressClass))
	{
		KeyPressUI = CreateWidget<UWG_KeyPress>(PC, KeyPressClass);
		if (KeyPressUI)
			KeyPressUI->AddToPlayerScreen(1000);
	}
	
	return KeyPressUI;
}

