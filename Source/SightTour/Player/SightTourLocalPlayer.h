// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonLocalPlayer.h"

#include "SightTourLocalPlayer.generated.h"

struct FGenericTeamId;

class APlayerController;
class UInputMappingContext;
class USightTourSettingsLocal;
class USightTourSettingsShared;
class UObject;
class UWorld;
struct FFrame;
struct FSwapAudioOutputResult;

/**
 * USightTourLocalPlayer
 */
UCLASS()
class SIGHTTOUR_API USightTourLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()

public:

	USightTourLocalPlayer();

	//~UObject interface
	virtual void PostInitProperties() override;
	//~End of UObject interface

	/** Gets the local settings for this player, this is read from config files at process startup and is always valid */
	UFUNCTION()
	USightTourSettingsLocal* GetLocalSettings() const;

	/** Gets the shared setting for this player, this is read using the save game system so may not be correct until after user login */
	UFUNCTION()
	USightTourSettingsShared* GetSharedSettings() const;

	/** Starts an async request to load the shared settings, this will call OnSharedSettingsLoaded after loading or creating new ones */
	void LoadSharedSettingsFromDisk(bool bForceLoad = false);

protected:
	void OnSharedSettingsLoaded(USightTourSettingsShared* LoadedOrCreatedSettings);

private:
	UPROPERTY(Transient)
	mutable TObjectPtr<USightTourSettingsShared> SharedSettings;

	FUniqueNetIdRepl NetIdForSharedSettings;

	UPROPERTY(Transient)
	mutable TObjectPtr<const UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> LastBoundPC;
};
