// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SightTourSettingsShared.generated.h"


UCLASS()
class SIGHTTOUR_API USightTourSettingsShared : public ULocalPlayerSaveGame
{
	GENERATED_BODY()
public:
	DECLARE_EVENT_OneParam(USightTourSettingsShared, FOnSettingChangedEvent, USightTourSettingsShared* Settings);
	FOnSettingChangedEvent OnSettingChanged;

public:

	USightTourSettingsShared();

	//~ULocalPlayerSaveGame interface
	int32 GetLatestDataVersion() const override;
	//~End of ULocalPlayerSaveGame interface

	bool IsDirty() const { return bIsDirty; }
	void ClearDirtyFlag() { bIsDirty = false; }

	/** Creates a temporary settings object, this will be replaced by one loaded from the user's save game */
	static USightTourSettingsShared* CreateTemporarySettings(const ULocalPlayer* LocalPlayer);

	/** Synchronously loads a settings object, this is not valid to call before login */
	static USightTourSettingsShared* LoadOrCreateSettings(const ULocalPlayer* LocalPlayer);

	DECLARE_DELEGATE_OneParam(FOnSettingsLoadedEvent, USightTourSettingsShared* Settings);

	/** Starts an async load of the settings object, calls Delegate on completion */
	static bool AsyncLoadOrCreateSettings(const ULocalPlayer* LocalPlayer, FOnSettingsLoadedEvent Delegate);

	/** Saves the settings to disk */
	void SaveSettings();

	/** Applies the current settings to the player */
	void ApplySettings();

public:
	////////////////////////////////////////////////////////
	// Fusion Vision Options

	UFUNCTION()
	float GetFusionVisionStrength() const;
	UFUNCTION()
	void SetFusionVisionStrength(float InFusionVisionStrength);

	UFUNCTION()
	bool GetEnableFusionVision() const;
	UFUNCTION()
	void EnableFusionVision(bool bEnable);

private:
	UPROPERTY()
	float FusionVisionStrength = 10.f;
	UPROPERTY()
	bool bEnableFusionVision = true;



	////////////////////////////////////////////////////////
	/// Dirty and Change Reporting
private:
	template<typename T>
	bool ChangeValueAndDirty(T& CurrentValue, const T& NewValue)
	{
		if (CurrentValue != NewValue)
		{
			CurrentValue = NewValue;
			bIsDirty = true;
			OnSettingChanged.Broadcast(this);

			return true;
		}

		return false;
	}

	bool bIsDirty = false;
};
