// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DataSource/GameSettingDataSourceDynamic.h" // IWYU pragma: keep
#include "GameSettingRegistry.h"
#include "Settings/SightTourSettingsLocal.h" // IWYU pragma: keep

#include "SightTourGameSettingRegistry.generated.h"

class ULocalPlayer;
class UObject;

//--------------------------------------
// ULyraGameSettingRegistry
//--------------------------------------

class UGameSettingCollection;
class ULocalPlayer;

DECLARE_LOG_CATEGORY_EXTERN(LogSightTourGameSettingRegistry, Log, Log);

#define GET_SHARED_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
		GET_FUNCTION_NAME_STRING_CHECKED(USightTourLocalPlayer, GetSharedSettings),				\
		GET_FUNCTION_NAME_STRING_CHECKED(USightTourSettingsShared, FunctionOrPropertyName)		\
	}))

#define GET_LOCAL_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
		GET_FUNCTION_NAME_STRING_CHECKED(USightTourLocalPlayer, GetLocalSettings),				\
		GET_FUNCTION_NAME_STRING_CHECKED(USightTourSettingsLocal, FunctionOrPropertyName)		\
	}))

/**
 * 
 */
UCLASS()
class USightTourGameSettingRegistry : public UGameSettingRegistry
{
	GENERATED_BODY()

public:
	USightTourGameSettingRegistry();

	static USightTourGameSettingRegistry* Get(ULocalPlayer* InLocalPlayer);
	
	virtual void SaveChanges() override;

protected:
	virtual void OnInitialize(ULocalPlayer* InLocalPlayer) override;
	virtual bool IsFinishedInitializing() const override;

	UGameSettingCollection* InitializeMouseAndKeyboardSettings(ULocalPlayer* InLocalPlayer);

	UGameSettingCollection* InitializeFusionVisionSettings(ULocalPlayer* InLocalPlayer);

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> MouseAndKeyboardSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> FusionVisionSettings;
};
