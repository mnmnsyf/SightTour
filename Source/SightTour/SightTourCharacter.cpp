// Copyright Epic Games, Inc. All Rights Reserved.

#include "SightTourCharacter.h"
#include "SightTourProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TP_WeaponComponent.h"
#include "Pickup/Equipment/EquipmentManagerComponent.h"
#include "UI/SightTourHUD.h"

//////////////////////////////////////////////////////////////////////////
// ASightTourCharacter

ASightTourCharacter::ASightTourCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	//装备管理组件
	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("EquipmentManager"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
}

void ASightTourCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	APlayerController* PlayerController = GetPlayerController();
	check(PlayerController);

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}


	ASightTourHUD* HUD = Cast<ASightTourHUD>(PlayerController->GetHUD());
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	check(HUD && LocalPlayer);
	HUD->NotifyPlayerAdded(LocalPlayer);
}

void ASightTourCharacter::Tick(float DeltaTime)
{
	if (TickDashTime > 0.f)
	{
		TickDashTime -= DeltaTime;
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void ASightTourCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASightTourCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASightTourCharacter::Look);

		//冲刺
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ASightTourCharacter::Dash);
	}
}


APlayerController* ASightTourCharacter::GetPlayerController()
{
	return Cast<APlayerController>(Controller);
}

void ASightTourCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ASightTourCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASightTourCharacter::Dash(const FInputActionValue& Value)
{
	//Calculate dash direction and select appropriate animation

	//If we have a non-zero dash, commit the ability
	if (TickDashTime <= 0.f)
	{
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}
		TickDashTime = DashCoolTime;
	}
}

void ASightTourCharacter::SetHasRifle(bool bNewHasRifle, UTP_WeaponComponent* InEquippedWeapon)
{
	bHasRifle = bNewHasRifle;
	EquippedWeapon = bHasRifle ? InEquippedWeapon : nullptr;
	EquipmentManagerComponent->EquipWeapon(InEquippedWeapon);
}

bool ASightTourCharacter::GetHasRifle()
{
	return bHasRifle;
}

FTransform ASightTourCharacter::GetCameraTransform()
{
	if (FirstPersonCameraComponent)
	{
		return FirstPersonCameraComponent->GetComponentTransform();
	}
	return FTransform();
}