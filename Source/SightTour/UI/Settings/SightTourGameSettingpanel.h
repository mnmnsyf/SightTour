// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameSettingRegistry.h"
#include "GameSettingRegistryChangeTracker.h"
#include "SightTourGameSettingpanel.generated.h"

struct FCompiledToken;

class UGameSetting;
class UObject;
struct FGameSettingFilterState;
class UGameSettingListEntryBase;
class USightTourSettingsListEntrySetting_KeyboardInput;
/**
 * 
 */
UCLASS(Abstract, meta = (Category = "Settings", DisableNativeTick))
class SIGHTTOUR_API USightTourGameSettingpanel : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeOnInitialized() override;

protected:
	UFUNCTION(BlueprintCallable)
	UGameSettingCollection* GetSettingCollection(FName SettingDevName, bool& HasAnySettings);

	UFUNCTION(BlueprintNativeEvent)
	void OnSettingsDirtyStateChanged(bool bSettingsDirty);
	virtual void OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty) { }

	UFUNCTION(BlueprintCallable)
	virtual void CancelChanges();

	UFUNCTION(BlueprintCallable)
	virtual void ApplyChanges();

	UFUNCTION(BlueprintCallable)
	bool HaveSettingsBeenChanged() const { return ChangeTracker.HaveSettingsBeenChanged(); }

	void ClearDirtyState();

	void HandleSettingChanged(UGameSetting* Setting, EGameSettingChangeReason Reason);

	void RefreshItemList();

	//按对象池查找或创建一个百夫卡片UI
	//Index:从1开始，表示兵牌UI在兵牌上对应的排列序号
	USightTourSettingsListEntrySetting_KeyboardInput* FindOrCreateOneKeyboardInput(int32 Index);
	

private:
	UGameSettingRegistry* CreateRegistry();
	UGameSettingRegistry* GetOrCreateRegistry();

	UFUNCTION(BlueprintCallable)
	void NavigateToSetting(FName SettingDevName);

	UFUNCTION(BlueprintCallable)
	void NavigateToSettings(const TArray<FName>& SettingDevNames);

	USightTourSettingsListEntrySetting_KeyboardInput* LoadUserWidget();

protected:
	FGameSettingRegistryChangeTracker ChangeTracker;

private:	// Bound Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	class UVerticalBox* VB_Items;

	//按键绑定UI
	UPROPERTY(EditDefaultsOnly, Category = "KBMCard", meta = (AllowAbstract))
	TSoftClassPtr<USightTourSettingsListEntrySetting_KeyboardInput> BaiFuCardUI;

private:
	UPROPERTY(Transient)
	mutable TObjectPtr<UGameSettingRegistry> Registry;

	UPROPERTY(Transient)
	FGameSettingFilterState FilterState;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UGameSetting>> VisibleSettings;

	//当前显示的兵牌 兵牌上的序号 - UI 数据
	UPROPERTY()
	TMap<int32, USightTourSettingsListEntrySetting_KeyboardInput*> BaiFuMap;

	//对象池，重刷时避免重复创建子项
	UPROPERTY()
	TArray<USightTourSettingsListEntrySetting_KeyboardInput*> OneKeyBoardCardObjectPool;
};