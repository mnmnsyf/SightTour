// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "WG_EnemyProblem.generated.h"

class UWG_ProblemText;

/**
 * 
 */
UCLASS()
class SIGHTTOUR_API UWG_EnemyProblem : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* CP_CanvasPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HB_ProblemBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HB_ProblemTextBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* I_ProblemBackground;

public:
	void UpdateProblemContext(TArray<FString> Context);

	//对象池处理数据Item的条数。创建或者隐藏，少了就创建，多了就隐藏。 bSetVisible：是否自动将合适的Item设置visible，不然外部自己控制
	static void ControlItemCount(TArray<UCommonUserWidget*> CurAllItem, int32 NewNum, TFunction<void()> CreateOneItem, bool bSetVisible = true);

private:
	UWG_ProblemText* LoadOneProblemText();
	void ResetContainerData();

	void NativeConstruct() override;
	void NativeDestruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Problem", AllowAbstract))
	TSoftClassPtr<class UWG_ProblemText> ProblemTextClass;

	UPROPERTY()
	TArray<class UWG_ProblemText*> ProblemTextPool;
};
