// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SightTourCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UEquipmentManagerComponent;

UCLASS(config=Game)
class ASightTourCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASightTourCharacter();

protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle, class UTP_WeaponComponent* InEquippedWeapon);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

public:
	//扣血
	void ReduceHealth(const float ReduceValue);

	//恢复默认血量
	FORCEINLINE void ResetDefaultHealth() { CurrentHealth = DefaultHealth; };

	void UpdateHealthBar();

public:
	/** 获取组件 */
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	FTransform GetCameraTransform();
	
	UEquipmentManagerComponent* GetEquipmentManagerComponent() const { return EquipmentManagerComponent; }

public:
	APlayerController* GetPlayerController();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	//冲刺
	void Dash(const FInputActionValue& Value);

public:
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	UPROPERTY(BlueprintReadOnly)
	class UTP_WeaponComponent* EquippedWeapon = nullptr;

protected:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	UEquipmentManagerComponent* EquipmentManagerComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Player | Input", meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Player | Input", meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player | Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* DashAction;

	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Player | Input", meta = (AllowPrivateAccess = "true"))
	float DashCoolTime = 3.0f;

	float TickDashTime = 0.0f;

private:
	/** 当前生命值，蓝图不可修改 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player | Health", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth = 100.f;

	/** 最大生命值，蓝图可修改 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | Health", meta = (AllowPrivateAccess = "true"))
	float DefaultHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | Health", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UWG_PlayerHealth> HealthBarClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | Health", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWG_PlayerHealth> HealthBar;
};

