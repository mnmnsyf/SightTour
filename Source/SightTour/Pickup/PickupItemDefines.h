// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/DataTable.h"
#include "PickupItemDefines.generated.h"

struct FInstancedStruct;
class APickupItemBase;

USTRUCT(BlueprintType)
struct FPickableConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, DisplayName = "问题", Meta = (ExcludeBaseStruct, BaseStruct = "/Script/SightTour.PickableItem"))
	TArray<FInstancedStruct> PickableItem;
};

USTRUCT(meta = (Hidden))
struct FPickableItem
{
	GENERATED_BODY()

	virtual ~FPickableItem() {}
	
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

	virtual void Reset() {}

	virtual FString GetActualValue() { return FString(); }
};

USTRUCT(BlueprintType)
struct FAddOrSubBall : public FFillBallBase
{
	GENERATED_BODY()

	FAddOrSubBall();
	
	virtual void ChangeValue(FString NewValue) override;

	virtual void Reset() override;

	virtual FString GetActualValue() override;

private:
	UPROPERTY(EditAnywhere)
	int32 Value = 0;
};

USTRUCT(BlueprintType)
struct FMultiOrDivBall : public FFillBallBase
{
	GENERATED_BODY()

	FMultiOrDivBall();

	virtual void ChangeValue(FString NewValue) override;

	virtual void Reset() override;

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

	FTextBall();

	virtual void ChangeValue(FString NewValue) override;

	virtual void Reset() override;

	virtual FString GetActualValue() override;

private:
	UPROPERTY(EditAnywhere)
	FString Value;
};