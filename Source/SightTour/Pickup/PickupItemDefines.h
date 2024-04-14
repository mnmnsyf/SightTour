// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/DataTable.h"
#include "InstancedStruct.h"
#include "PickupItemDefines.generated.h"

class APickupItemBase;

USTRUCT(BlueprintType)
struct FPickableConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, DisplayName = "Item", Meta = (ExcludeBaseStruct, BaseStruct = "/Script/SightTour.PickableItem"))
	FInstancedStruct PickableItem;
};

USTRUCT(meta = (Hidden))
struct FPickableItem
{
	GENERATED_BODY()

	virtual ~FPickableItem() {}

	virtual void Init() {}
	
	bool operator==(const FPickableItem& A) const
	{
		return this->ItemTypeName == A.ItemTypeName;
	}

	FName ItemTypeName;
};

USTRUCT(meta = (Hidden))
struct FFillBallBase : public FPickableItem
{
	GENERATED_BODY()

	virtual void ChangeValue(FString NewValue) {}

	void UseBall(bool bUse) { bUsed = bUse; }

	virtual FString GetActualValue() { return FString(); }

	virtual bool IsUsed() { return bUsed; }

private:
	bool bUsed = false;
};

USTRUCT(BlueprintType)
struct FAddOrSubBall : public FFillBallBase
{
	GENERATED_BODY()

	virtual void Init() override;
	
	virtual void ChangeValue(FString NewValue) override;

	virtual FString GetActualValue() override;

private:
	UPROPERTY(EditAnywhere)
	int32 Value = 0;
};

USTRUCT(BlueprintType)
struct FMultiOrDivBall : public FFillBallBase
{
	GENERATED_BODY()

	virtual void Init() override;

	virtual void ChangeValue(FString NewValue) override;

	virtual FString GetActualValue() override;

private:
	UPROPERTY(EditAnywhere)
	int32 Value = 0;

	UPROPERTY(EditAnywhere)
	bool bIsMulti = true;
};

USTRUCT(BlueprintType)
struct FTextBall : public FFillBallBase
{
	GENERATED_BODY()

	virtual void Init() override;

	virtual void ChangeValue(FString NewValue) override;

	virtual FString GetActualValue() override;

private:
	UPROPERTY(EditAnywhere)
	FString Value;
};