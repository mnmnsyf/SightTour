
#pragma once

#include "CoreMinimal.h"
#include "PropertyBag.h"
#include "StateTreeBlackBoardCom.generated.h"

struct FInstancedPropertyBag;

UCLASS(Blueprintable)
class SIGHTTOUR_API UStateTreeBlackBoardConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	FInstancedPropertyBag GetConfigParams() const { return ConfigParams; }
	FInstancedPropertyBag GetRuntimeParams() const { return RuntimeParams; }

	// 配置项参数
	UPROPERTY(EditAnywhere, Category = "状态树黑板默认配置")
	FInstancedPropertyBag ConfigParams;

	// 运行时参数
	UPROPERTY(EditAnywhere, Category = "状态树黑板运行配置")
	FInstancedPropertyBag RuntimeParams;
};


UCLASS(ClassGroup = "DiyComponent", hidecategories = (Object, LOD, Lighting, TextureStreaming), editinlinenew,
	meta = (DisplayName = "StateTreeBlackBoardCom", BlueprintSpawnableComponent))
class SIGHTTOUR_API UStateTreeBlackBoardCom : public UActorComponent
{

private:
	GENERATED_BODY()

public:
	UStateTreeBlackBoardCom(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	//由配置设置运行时数据项
	void SetRuntimeValueByConfig();

	//重设运行时数据项
	void ResetRuntimeValue();

	//获取黑板配置资产
	void GetBlackBoardConfigData();

public:
	//获取配置Param
	FInstancedPropertyBag& GetConfigParams();
	//获取运行时Param(可修改)
	FInstancedPropertyBag& GetRuntimeParams();

public:
	//配置项参数和运行时参数的资产
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "StateTreeBlackBoardConfig", DisplayName = "状态树黑板配置资产"))
	FSoftObjectPath STBBConfig;

	//配置项参数
	UPROPERTY(SaveGame, EditAnywhere, Category = "", meta = (FixedLayout))
	FInstancedPropertyBag ConfigParams;

	// 运行时参数
	UPROPERTY(SaveGame, EditAnywhere, Category = "", meta = (FixedLayout))
	FInstancedPropertyBag RuntimeParams;

};



