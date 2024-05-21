// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyPressDefines.h"
#include "Engine/DataTable.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "KeyPress.generated.h"

DECLARE_DELEGATE_OneParam(FKeyPressTriggerDelegate, bool);

UCLASS()
class SIGHTTOUR_API AKeyPress : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyPress();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetStartPlay(bool bStart) { bStartPlay = bStart; }

	bool InitConfig();

	bool UpdateNextKey(float DeltaTime);

	void CheckAnswerKey(const FInputActionInstance& InputActionInstance);

	void WrongCallback();

private:
	TObjectPtr<class UWG_KeyPress> GetKeyPressUI();
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = KeyPress, meta = (DisplayName = "UpdateTime"))
	float UpdateTime = 5.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = KeyPress, meta = (DisplayName = "CheckTime"))
	float CheckTime = 2.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = KeyPress, meta = (DisplayName = "ReduceHP"))
	int32 ReduceHP = 3;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = KeyPress, meta = (DisplayName = "KeyPressUI"))
	TArray<TSoftObjectPtr<class UInputAction>> InputActions;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = KeyPress, meta = (DisplayName = "KeyPressUI"))
	TArray<TObjectPtr<UMaterialInstance>> Images;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KeyPress", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* MappingContext;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeyPress", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UWG_KeyPress> KeyPressClass;

	UPROPERTY()
	TObjectPtr<class UWG_KeyPress> KeyPressUI;

	bool bStartPlay = false;

	float RemainTime = 0.0f;

	//---------------------- 当前所需数据 -----------------------------------
	uint32 IABindHandle = -1;
	FTimerHandle TimerHandle;
};
