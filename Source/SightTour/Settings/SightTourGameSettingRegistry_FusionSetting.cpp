// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonInputBaseTypes.h"
#include "EnhancedInputSubsystems.h"
#include "CustomSettings/SightTourSettingKeyboardInput.h"
#include "DataSource/GameSettingDataSource.h"
#include "EditCondition/WhenCondition.h"
#include "GameSettingCollection.h"
#include "GameSettingValueDiscreteDynamic.h"
#include "GameSettingValueScalarDynamic.h"
#include "SightTourGameSettingRegistry.h"
#include "SightTourSettingsLocal.h"
#include "CommonLocalPlayer.h"
#include "PlayerMappableInputConfig.h"
#include "SightTourSettingsShared.h"

class ULocalPlayer;

#define LOCTEXT_NAMESPACE "SightTour"

UGameSettingCollection* USightTourGameSettingRegistry::InitializeFusionVisionSettings(ULocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* Screen = NewObject<UGameSettingCollection>();
	Screen->SetDevName(TEXT("FusionVisionCollection"));
	Screen->SetDisplayName(LOCTEXT("FusionVisionCollection_Name", "Fusion Vision"));
	Screen->Initialize(InLocalPlayer);

	// Bindings for Fusion Vision - Automatically Generated
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingCollection* Sensitivity = NewObject<UGameSettingCollection>();
		Sensitivity->SetDevName(TEXT("FusionSensitivityCollection"));
		Sensitivity->SetDisplayName(LOCTEXT("FusionSensitivityCollection_Name", "Sensitivity"));
		Screen->AddSetting(Sensitivity);

		//----------------------------------------------------------------------------------
		/*{
			UGameSettingValueScalarDynamic* Setting = NewObject<UGameSettingValueScalarDynamic>();
			Setting->SetDevName(TEXT("FusionSensitivity"));
			Setting->SetDisplayName(LOCTEXT("FusionSensitivity_Name", "Fusion Sensitivity"));
			Setting->SetDescriptionRichText(LOCTEXT("FusionSensitivity_Description", "Sets the sensitivity of the fusion. The smaller the value, the less obvious the object is visualized."));

			Setting->SetDynamicGetter(GET_SHARED_SETTINGS_FUNCTION_PATH(GetFusionVisionStrength));
			Setting->SetDynamicSetter(GET_SHARED_SETTINGS_FUNCTION_PATH(SetFusionVisionStrength));
			Setting->SetDefaultValue(GetDefault<USightTourSettingsShared>()->GetFusionVisionStrength());
			Setting->SetDisplayFormat(UGameSettingValueScalarDynamic::RawTwoDecimals);
			Setting->SetSourceRangeAndStep(TRange<double>(0, 10), 0.01);
			Setting->SetMinimumLimit(0.01);

			Sensitivity->AddSetting(Setting);
		}*/
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Bool* Setting = NewObject<UGameSettingValueDiscreteDynamic_Bool>();
			Setting->SetDevName(TEXT("EnableFusionVision"));
			Setting->SetDisplayName(LOCTEXT("EnableFusionVision_Name", "Enable Fusion Vision"));
			Setting->SetDescriptionRichText(LOCTEXT("EnableFusionVision_Description", "Enable the fusion of vision."));

			Setting->SetDynamicGetter(GET_SHARED_SETTINGS_FUNCTION_PATH(GetEnableFusionVision));
			Setting->SetDynamicSetter(GET_SHARED_SETTINGS_FUNCTION_PATH(EnableFusionVision));
			Setting->SetDefaultValue(GetDefault<USightTourSettingsShared>()->GetEnableFusionVision());

			Sensitivity->AddSetting(Setting);
		}
	}

	return Screen;
}

#undef LOCTEXT_NAMESPACE