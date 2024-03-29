// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings/SightTourGameSettingpanel.h"
#include "../Settings/SightTourGameSettingRegistry.h"
#include "Engine/LocalPlayer.h"
#include "GameSettingCollection.h"
#include "GameSettingFilterState.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "../Settings/Widgets/SightTourSettingsListEntrySetting_KeyboardInput.h"

void USightTourGameSettingpanel::RefreshItemList()
{
	VisibleSettings.Reset();
	Registry->GetSettingsForFilter(FilterState, MutableView(VisibleSettings));

	for (int32 SettingIdx = 0; SettingIdx < VisibleSettings.Num(); ++SettingIdx)
	{
		if (UGameSetting* Setting = VisibleSettings[SettingIdx])
		{
			Setting->RefreshEditableState(false);

			//按排序顺序依次加入HB_Box
			USightTourSettingsListEntrySetting_KeyboardInput* OneCard = FindOrCreateOneKeyboardInput(SettingIdx);
			OneCard->SetSetting(Setting);
		}
	}
}

class USightTourSettingsListEntrySetting_KeyboardInput* USightTourGameSettingpanel::FindOrCreateOneKeyboardInput(int32 Index)
{
	USightTourSettingsListEntrySetting_KeyboardInput* OneKeyboardCard = nullptr;
	//OneBaiFuCardObjectPool是对象池， BaiFuMap目前每次重刷时会清空重新加入，如果有插入逻辑需要插入后重新维护BaiFuMap
	if (BaiFuMap.Num() < OneKeyBoardCardObjectPool.Num())
	{
		OneKeyboardCard = OneKeyBoardCardObjectPool[BaiFuMap.Num()];
		OneKeyboardCard->Initialize();
		BaiFuMap.Add(Index, OneKeyboardCard);
	}
	else
	{
		OneKeyboardCard = LoadUserWidget();
		if (OneKeyboardCard != nullptr)
		{
			OneKeyboardCard->Initialize();
			OneKeyBoardCardObjectPool.Add(OneKeyboardCard);
			BaiFuMap.Add(Index, OneKeyboardCard);
		}
	}

	return OneKeyboardCard;
}

void USightTourGameSettingpanel::CancelChanges()
{
	ChangeTracker.RestoreToInitial();
	ClearDirtyState();
}

void USightTourGameSettingpanel::ApplyChanges()
{
	if (ChangeTracker.HaveSettingsBeenChanged())
	{
		ChangeTracker.ApplyChanges();
		ClearDirtyState();
		Registry->SaveChanges();
	}
}

void USightTourGameSettingpanel::ClearDirtyState()
{
	ChangeTracker.ClearDirtyState();

	OnSettingsDirtyStateChanged(false);
}

void USightTourGameSettingpanel::HandleSettingChanged(UGameSetting* Setting, EGameSettingChangeReason Reason)
{
	OnSettingsDirtyStateChanged(true);
}	

UGameSettingCollection* USightTourGameSettingpanel::GetSettingCollection(FName SettingDevName, bool& HasAnySettings)
{
	HasAnySettings = false;

	if (UGameSettingCollection* Collection = GetOrCreateRegistry()->FindSettingByDevNameChecked<UGameSettingCollection>(SettingDevName))
	{
		TArray<UGameSetting*> InOutSettings;

		FGameSettingFilterState NewFilterState;
		Collection->GetSettingsForFilter(NewFilterState, InOutSettings);

		HasAnySettings = InOutSettings.Num() > 0;

		return Collection;
	}

	return nullptr;
}

void USightTourGameSettingpanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ChangeTracker.WatchRegistry(Registry);

	OnSettingsDirtyStateChanged(HaveSettingsBeenChanged());

	/*bool HasAnySettings = false;
	GetSettingCollection("MouseAndKeyboardCollection", HasAnySettings);*/
	NavigateToSetting("MouseAndKeyboardCollection");
}

UGameSettingRegistry* USightTourGameSettingpanel::CreateRegistry()
{
	USightTourGameSettingRegistry* NewRegistry = NewObject<USightTourGameSettingRegistry>();

	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		NewRegistry->Initialize(LocalPlayer);
	}

	return NewRegistry;
}

UGameSettingRegistry* USightTourGameSettingpanel::GetOrCreateRegistry()
{
	if (Registry == nullptr)
	{
		return this->CreateRegistry();
		//NewRegistry->OnSettingChangedEvent.AddUObject(this, &ThisClass::HandleSettingChanged);
	}

	return Registry;
}

void USightTourGameSettingpanel::NavigateToSetting(FName SettingDevName)
{
	NavigateToSettings({ SettingDevName });
}

void USightTourGameSettingpanel::NavigateToSettings(const TArray<FName>& SettingDevNames)
{
	FGameSettingFilterState NewFilterState;

	for (const FName SettingDevName : SettingDevNames)
	{
		if (UGameSetting* Setting = GetOrCreateRegistry()->FindSettingByDevNameChecked<UGameSetting>(SettingDevName))
		{
			NewFilterState.AddSettingToRootList(Setting);
		}
	}

	FilterState = NewFilterState;
	//Settings_Panel->SetFilterState(FilterState);

	RefreshItemList();
}

USightTourSettingsListEntrySetting_KeyboardInput* USightTourGameSettingpanel::LoadUserWidget()
{
	check(!BaiFuCardUI.IsNull());
	TSubclassOf<USightTourSettingsListEntrySetting_KeyboardInput> WidgetClass = BaiFuCardUI.LoadSynchronous();

	if (ensure(WidgetClass && !WidgetClass->HasAnyClassFlags(CLASS_Abstract)))
	{
		USightTourSettingsListEntrySetting_KeyboardInput* NewObject = CreateWidget<USightTourSettingsListEntrySetting_KeyboardInput>(
			GetOwningLocalPlayer()->GetPlayerController(GetWorld()), WidgetClass);
		
		return NewObject;
	}

	return nullptr;
}
