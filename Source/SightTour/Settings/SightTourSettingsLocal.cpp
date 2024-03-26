// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/SightTourSettingsLocal.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
USightTourSettingsLocal::USightTourSettingsLocal()
{
	;
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS

USightTourSettingsLocal* USightTourSettingsLocal::Get()
{
	return GEngine ? CastChecked<USightTourSettingsLocal>(GEngine->GetGameUserSettings()) : nullptr;
}
