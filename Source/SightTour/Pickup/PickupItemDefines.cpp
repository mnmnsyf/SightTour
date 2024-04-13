// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItemDefines.h"

FAddOrSubBall::FAddOrSubBall()
{
	ItemTypeName = (Value > 0) ? FName("AddBall") : FName("SubBall");
}

void FAddOrSubBall::ChangeValue(FString NewValue)
{
	int32 NewValueNumber = FCString::Atoi(*NewValue);

	Value += NewValueNumber;
}

void FAddOrSubBall::Reset()
{
	Value = 0;
}

FString FAddOrSubBall::GetActualValue()
{
	return (Value > 0) ? FString::Printf(TEXT("+%d"), Value) : FString::Printf(TEXT("-%d"), Value);
}

FMultiOrDivBall::FMultiOrDivBall()
{
	ItemTypeName = bIsMulti ? FName("MultiBall") : FName("DivBall");
}

void FMultiOrDivBall::ChangeValue(FString NewValue)
{
	Value = FCString::Atoi(*NewValue);
}

void FMultiOrDivBall::Reset()
{
	Value = 0;
}

FString FMultiOrDivBall::GetActualValue()
{
	return bIsMulti ? FString::Printf(TEXT("*%d"), Value) : FString::Printf(TEXT("/%d"), Value);
}

FTextBall::FTextBall()
{
	ItemTypeName = FName("TextBall");
}

void FTextBall::ChangeValue(FString NewValue)
{
	Value = NewValue;
}

void FTextBall::Reset()
{
	Value = FString("");
}

FString FTextBall::GetActualValue()
{
	return Value;
}
