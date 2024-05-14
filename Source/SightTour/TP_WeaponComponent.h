// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Texture2D.h"
#include "Pickup/PickupItemDefines.h"
#include "TP_WeaponComponent.generated.h"

class ASightTourCharacter;
class AProjectile;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIGHTTOUR_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

public:
	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachWeapon(ASightTourCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Attract();

	/** Muzzel */
	FName GetMuzzelScoketName() const { return AttractSocket; }

	FVector GetMuzzelLocation() const { return GetSocketLocation(AttractSocket); }

	FRotator GetMuzzelRotation() const { return GetSocketRotation(AttractSocket); }

	/** Gameplay */
	ASightTourCharacter* GetOwnerCharacter();

	APlayerController* GetPlayerController();

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Gameplay")
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Gameplay")
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Gameplay")
	FName AttractSocket = FName("Muzzle");

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttractAction;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AFillBall> BallClass = nullptr;


public:
	/** 武器十字准星纹理 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairMiddle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Crosshairs", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsDown;

public:
	float ShootRange = 1000.f;

protected:
	/** The Character holding this weapon*/
	UPROPERTY()
	TObjectPtr<ASightTourCharacter> OwnerCharacter = nullptr;

	//枪口是否有吸附小球
	bool bAttractProjectile = false;

	UPROPERTY()
	TObjectPtr<AProjectile> AttactProjectile = nullptr;
};
