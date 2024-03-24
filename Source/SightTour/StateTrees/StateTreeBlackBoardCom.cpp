// Fill out your copyright notice in the Description page of Project Settings.

#include "StateTreeBlackBoardCom.h"

 UStateTreeBlackBoardCom::UStateTreeBlackBoardCom(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 }

void UStateTreeBlackBoardCom::BeginPlay()
{
	Super::BeginPlay();

	GetBlackBoardConfigData();

	//SetRuntimeValueByConfig();
}

void UStateTreeBlackBoardCom::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UStateTreeBlackBoardCom::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ResetRuntimeValue();

	Super::EndPlay(EndPlayReason);
}

void UStateTreeBlackBoardCom::SetRuntimeValueByConfig()
{
	RuntimeParams.InitializeFromBagStruct(ConfigParams.GetPropertyBagStruct());
}

void UStateTreeBlackBoardCom::ResetRuntimeValue()
{
	RuntimeParams.Reset();
}

void UStateTreeBlackBoardCom::GetBlackBoardConfigData()
{
	if (STBBConfig != nullptr)
	{
		if (auto* ConfigObj = STBBConfig.TryLoad())
		{
			UStateTreeBlackBoardConfig* Config = Cast<UStateTreeBlackBoardConfig>(ConfigObj);

			ConfigParams.InitializeFromBagStruct(Config->ConfigParams.GetPropertyBagStruct());
			ConfigParams.CopyMatchingValuesByID(Config->ConfigParams);
			RuntimeParams.InitializeFromBagStruct(Config->RuntimeParams.GetPropertyBagStruct());
			RuntimeParams.CopyMatchingValuesByID(Config->RuntimeParams);
		}
	}
}

FInstancedPropertyBag& UStateTreeBlackBoardCom::GetConfigParams()
{
	return ConfigParams; 
}

FInstancedPropertyBag& UStateTreeBlackBoardCom::GetRuntimeParams()
{
	return RuntimeParams;
}
