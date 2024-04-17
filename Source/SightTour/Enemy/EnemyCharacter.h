// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UAnimMontage;
class UAnimationAsset;
class UAnimSequence;

UCLASS()
class SIGHTTOUR_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//向移动添加力的方向 forceDir传单位方向
	void SetCharacterMoveByForce(const FVector& ForceDir);

	//扣血
	void ReduceHealth(const float ReduceValue);

	//恢复默认血量
	FORCEINLINE void ResetDefaultHealth() { CurrentHealth = DefaultHealth; };

	//播放动画蒙太奇
	UAnimMontage* PlayAnimAsMontage(class UAnimSequence* Anim, const float PlayRate = 1.0f);

	FName GetRandomAttackSectionName();

private:
	/** 敌人当前生命值，蓝图不可修改 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat | Health", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth = 100.f;

	/** 敌人最大生命值，蓝图可修改 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Health", meta = (AllowPrivateAccess = "true"))
	float DefaultHealth = 100.f;

	/** 被子弹击中时产生的粒子效果 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Impact", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ImpactParticles;

	/** 被子弹击中时播放的声音 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* ImpactSound;

public:
	/** 包含不同攻击方式的动画蒙太奇 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Attack", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackAnim;
};
