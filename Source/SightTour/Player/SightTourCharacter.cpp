// Copyright Epic Games, Inc. All Rights Reserved.

#include "SightTourCharacter.h"
#include "SightTourProjectile.h"
#include "TP_WeaponComponent.h"
#include "Pickup/Equipment/EquipmentManagerComponent.h"
#include "UI/Subsystem/SightTourUIManager.h"
#include "UI/Player/WG_PlayerHUD.h"
#include "UI/Common/WG_ItemName.h"

#include "UserSettings/EnhancedInputUserSettings.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "NativeGameplayTags.h"

//////////////////////////////////////////////////////////////////////////
// ASightTourCharacter

UE_DEFINE_GAMEPLAY_TAG_STATIC(PlayerHUDLayer, "UI.Layer.Game");
UE_DEFINE_GAMEPLAY_TAG_STATIC(ItemInfoLayer, "UI.Layer.Modal");

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

	// 要等playercontroller加载完才能调用，暂时现在这里调
	APlayerController* PlayerController = GetPlayerController();
	check(PlayerController);

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	check(LocalPlayer);

	USightTourUIManager::Get()->NotifyPlayerAdded(LocalPlayer);

	ResetDefaultHealth();

	UpdateHealthBar();
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
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* EISubsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(EISubsystem);

	EISubsystem->ClearAllMappings();

	// Actually add the config to the local player,添加本地玩家的按键配置，bNotifyUserSettings设为true	
	FModifyContextOptions Options = {};
	Options.bIgnoreAllPressedKeysUntilRelease = false;
	Options.bNotifyUserSettings = true;						
	EISubsystem->AddMappingContext(DefaultMappingContext, 0, Options);

	// Set up action bindings 绑定按键到具体接口
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
	
		EnhancedInputComponent->BindAction(NumberOneAction, ETriggerEvent::Triggered, this, &ASightTourCharacter::NumberTrigger, 1);

		EnhancedInputComponent->BindAction(NumberTwoAction, ETriggerEvent::Triggered, this, &ASightTourCharacter::NumberTrigger, 2);

		EnhancedInputComponent->BindAction(NumberThreeAction, ETriggerEvent::Triggered, this, &ASightTourCharacter::NumberTrigger, 3);

		EnhancedInputComponent->BindAction(NumberFourAction, ETriggerEvent::Triggered, this, &ASightTourCharacter::NumberTrigger, 4);

		EnhancedInputComponent->BindAction(NumberFiveAction, ETriggerEvent::Triggered, this, &ASightTourCharacter::NumberTrigger, 5);
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
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	//旋转镜头
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}

	//更新视线内物品信息
	UpdateItemInfoInSight();
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

void ASightTourCharacter::NumberTrigger(const FInputActionValue& Value, int32 Number)
{
	if (Value.IsNonZero())
	{
		EquipmentManagerComponent->ChangeSlot(Number - 1);
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

void ASightTourCharacter::ReduceHealth(const float ReduceValue)
{
	CurrentHealth -= ReduceValue;

	if (CurrentHealth <= 0)
	{
		//TODO:播放死亡动画&音效
		//SetLifeSpan(5.0f);
	}

	UpdateHealthBar();

	FString DebugMsg = FString::Printf(TEXT("Player current health %.0f"), CurrentHealth);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, DebugMsg);
}

void ASightTourCharacter::UpdateHealthBar()
{
	if (!PlayerHUD && PlayerHUDClass)
	{
		USightTourUIManager* UIManager = USightTourUIManager::Get();
		check(UIManager);
		PlayerHUD = CastChecked<UWG_PlayerHUD>(UIManager->PushContentToLayer_ForPlayer(GetPlayerController()->GetLocalPlayer(), PlayerHUDLayer, PlayerHUDClass));
	}
	if (PlayerHUD)
	{
		PlayerHUD->SetHealth(CurrentHealth / DefaultHealth);
	}
}

void ASightTourCharacter::UpdateItemInfoInSight()
{
	if (!ItemInfoUI && ItemInfoClass)
	{
		USightTourUIManager* UIManager = USightTourUIManager::Get();
		check(UIManager);
		ItemInfoUI = CastChecked<UWG_ItemName>(UIManager->PushContentToLayer_ForPlayer(GetPlayerController()->GetLocalPlayer(), ItemInfoLayer, ItemInfoClass));
	}
	if (ItemInfoUI)
	{
		ItemInfoUI->SetText(GetItemNameInSight());
	}
}

FString ASightTourCharacter::GetItemNameInSight()
{
	FVector CameraDirection = GetCameraTransform().Rotator().Vector() * 500.0f;

	FHitResult OutHitResult;
	const FVector TraceStart = GetCameraTransform().GetLocation();
	const FVector TraceEnd = TraceStart + CameraDirection;

	GetWorld()->LineTraceSingleByChannel(OutHitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

	if (OutHitResult.bBlockingHit)
	{
		AActor* HitActor = OutHitResult.GetActor();
		if (IItemInfoInterface* Item = Cast<IItemInfoInterface>(HitActor))
		{
			return Item->GetItemName();
		}
	}

	return FString();
}

FTransform ASightTourCharacter::GetCameraTransform()
{
	if (FirstPersonCameraComponent)
	{
		return FirstPersonCameraComponent->GetComponentTransform();
	}
	return FTransform();
}