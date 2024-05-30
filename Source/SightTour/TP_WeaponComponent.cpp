
#include "TP_WeaponComponent.h"
#include "Player/SightTourCharacter.h"
#include "SightTourProjectile.h"
#include "Pickup/Projectile/Projectile.h"
#include "Interface/AttractInterface.h"
#include "Pickup/Equipment/EquipmentManagerComponent.h"
#include "SightTour.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/PlayerCameraManager.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn

}


void UTP_WeaponComponent::Fire()
{
	if (OwnerCharacter == nullptr || OwnerCharacter->GetController() == nullptr)
	{
		return;
	}

	bool bCanFire = false;

	// 打出小球
	if (AttactProjectile)
	{
		APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

		if (IAttractInterface* AttractInterface = Cast<IAttractInterface>(AttactProjectile))
		{
			AttractInterface->Spawn(this);
			AttactProjectile = nullptr;
			bAttractProjectile = false;
			bCanFire = true;
		}
	}
	//打出子弹总伤害
	else if (UEquipmentManagerComponent* EquipmentManager = OwnerCharacter->GetEquipmentManagerComponent())
	{
		FInstancedStruct TempFillBallConfig = EquipmentManager->DiscardFillBall();
		if (TempFillBallConfig.IsValid())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AFillBall* NewFillBall = GetWorld()->SpawnActor<AFillBall>(BallClass, FTransform(), SpawnParams);
			NewFillBall->FillBallConfig = TempFillBallConfig;
			if (IAttractInterface* AttractInterface = Cast<IAttractInterface>(NewFillBall))
			{
				AttractInterface->Spawn(this);
				bCanFire = true;
			}
		}
	}

	//播放音效和动画
	if (bCanFire)
	{
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, OwnerCharacter->GetActorLocation());
		}

		if (FireAnimation != nullptr)
		{
			UAnimInstance* AnimInstance = OwnerCharacter->GetMesh1P()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

void UTP_WeaponComponent::Attract()
{
	if (!IsValid(OwnerCharacter))
		return;

	if (bAttractProjectile)
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

			if (AProjectile* Projectile = Cast<AProjectile>(OutHit.GetActor()))
			{
				AttactProjectile = Projectile;
				bAttractProjectile = true;
			}
		}
	}
}

ASightTourCharacter* UTP_WeaponComponent::GetOwnerCharacter()
{
	return OwnerCharacter;
}

APlayerController* UTP_WeaponComponent::GetPlayerController()
{
	if (IsValid(OwnerCharacter))
	{
		return OwnerCharacter->GetPlayerController();
	}
	else
	{
		return nullptr;
	}
}

void UTP_WeaponComponent::AttachWeapon(ASightTourCharacter* TargetCharacter)
{
	if(!IsValid(TargetCharacter))
	return;

	OwnerCharacter = TargetCharacter;

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