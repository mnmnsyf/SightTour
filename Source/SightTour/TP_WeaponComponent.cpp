// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "SightTourCharacter.h"
#include "SightTourProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SightTour.h"
#include "Interface/AttractInterface.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn

}


void UTP_WeaponComponent::Fire()
{
	if (AttactActor == nullptr || OwnerCharacter == nullptr || OwnerCharacter->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (UWorld* const World = GetWorld())
	{
		APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		const FVector SpawnLocation = AttactActor->GetActorLocation();

		if (IAttractInterface* AttractInterface = Cast<IAttractInterface>(AttactActor))
		{
			AttractInterface->Spawn(this);
			AttactActor = nullptr;
		}
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, OwnerCharacter->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::Attract()
{
	if (bAttract && OwnerCharacter == nullptr || OwnerCharacter->GetController() == nullptr)
	{
		return;
	}
	FTransform CamerTrans = OwnerCharacter->GetCameraTransform();

	FVector TraceStartLoc = CamerTrans.GetLocation();
	FVector TraceEndLoc = TraceStartLoc + CamerTrans.GetUnitAxis(EAxis::X) * ShootRange;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(TRACE_PICKUP);

	TArray<AActor*> ActorsToIgnore;
	FHitResult OutHit;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		TraceStartLoc,
		TraceEndLoc,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true);

	if (OutHit.bBlockingHit)
	{
		if (IAttractInterface* AttractInterface = Cast<IAttractInterface>(OutHit.GetActor()))
		{
			AttractInterface->Attract(this);
			AttactActor = OutHit.GetActor();
			bAttract = true;
		}
	}
}

void UTP_WeaponComponent::AttachWeapon(ASightTourCharacter* TargetCharacter)
{
	OwnerCharacter = TargetCharacter;
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(OwnerCharacter->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// switch bHasRifle so the animation blueprint can switch to another animation set
	OwnerCharacter->SetHasRifle(true, this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);

			// 吸附物品
			EnhancedInputComponent->BindAction(AttractAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Attract);
		}
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}