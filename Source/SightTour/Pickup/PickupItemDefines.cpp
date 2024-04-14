// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItemDefines.h"

void FAddOrSubBall::Init()
{
	ItemTypeName = (Value > 0) ? FName("AddBall") : FName("SubBall");
}

void FAddOrSubBall::ChangeValue(FString NewValue)
{
	int32 NewValueNumber = FCString::Atoi(*NewValue);

	Value += NewValueNumber;
}

FString FAddOrSubBall::GetActualValue()
{
	return (Value > 0) ? FString::Printf(TEXT("+%d"), Value) : FString::Printf(TEXT("%d"), Value);
}

void FMultiOrDivBall::Init()
{
	ItemTypeName = bIsMulti ? FName("MultiBall") : FName("DivBall");
}

void FMultiOrDivBall::ChangeValue(FString NewValue)
{
	Value = FCString::Atoi(*NewValue);
}

FString FMultiOrDivBall::GetActualValue()
{
	return bIsMulti ? FString::Printf(TEXT("*%d"), Value) : FString::Printf(TEXT("/%d"), Value);
}

void FTextBall::Init()
{
	ItemTypeName = FName("TextBall");
}

void FTextBall::ChangeValue(FString NewValue)
{
	Value = NewValue;
}

FString FTextBall::GetActualValue()
{
	return Value;
}
