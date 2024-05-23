// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/WG_EnemyProblem.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "WG_ProblemText.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

void UWG_EnemyProblem::UpdateProblemContext(TArray<FString> Context)
{
	if (!ensure(HB_ProblemTextBox) || !ensure(HB_ProblemBox) || !ensure(I_ProblemBackground))
	{
		return;
	}

	// 控制对象池
	ControlItemCount(static_cast<TArray<UCommonUserWidget*>>(ProblemTextPool), Context.Num(), [this]() { LoadOneProblemText(); }, true);
	for (int32 i = 0; i < Context.Num(); i++)
	{
		UWG_ProblemText* ProblemText = ProblemTextPool[i];
		if (ProblemText)
		{
			ProblemText->SetText(Context[i]);
			ProblemText->SetVisibility(ESlateVisibility::Visible);
		}
	}
	
	// 调整背景大小
	{
		HB_ProblemTextBox->ForceLayoutPrepass();
		HB_ProblemBox->ForceLayoutPrepass();
		FVector2D BackgroundSize;
		FVector2D ProblemBoxSize;
		UCanvasPanelSlot* ImageSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(I_ProblemBackground);
		if (ImageSlot)
		{
			ProblemBoxSize = HB_ProblemBox->GetDesiredSize();
			BackgroundSize = ImageSlot->GetSize();
			ImageSlot->SetSize(FVector2D(ProblemBoxSize.X, BackgroundSize.Y));
		}
	}
}

void UWG_EnemyProblem::ControlItemCount(TArray<UCommonUserWidget*> CurAllItem, int32 NewNum, TFunction<void()> CreateOneItem, bool bSetVisible /*= true*/)
{
	//数目处理，直接比较数目。
		//现在的 < 新的：创建
	if (CurAllItem.Num() < NewNum)
	{
		int32 CreateNum = NewNum - CurAllItem.Num();
		if (CreateOneItem != nullptr)
		{
			for (int32 i = 0; i < CreateNum; i++)
			{
				CreateOneItem();
			}
		}
	}
	//现在的 > 新的：隐藏（折叠）多余的
	else if (CurAllItem.Num() > NewNum)
	{
		for (int32 i = NewNum; i < CurAllItem.Num(); i++)
		{
			CurAllItem[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	//目前控制的还是在这里全部显示（SelfHitTestInvisible）,不然外部经常忘记对于重新使用的对象池设visible
	if (bSetVisible)
	{
		for (int32 i = 0; i < NewNum && i < CurAllItem.Num(); i++)
		{
			CurAllItem[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

UWG_ProblemText* UWG_EnemyProblem::LoadOneProblemText()
{
	check(!ProblemTextClass.IsNull());
	TSubclassOf<UWG_ProblemText> WidgetClass = ProblemTextClass.LoadSynchronous();

	if (ensure(WidgetClass && !WidgetClass->HasAnyClassFlags(CLASS_Abstract)))
	{
		UWG_ProblemText* NewObject = CreateWidget<UWG_ProblemText>(GetOwningLocalPlayer()->GetPlayerController(GetWorld()), WidgetClass);
		// 缓存对象
		ProblemTextPool.Add(NewObject);
		// 添加到容器
		HB_ProblemTextBox->AddChild(NewObject);
		return NewObject;
	}

	return nullptr;
}

void UWG_EnemyProblem::ResetContainerData()
{
	ProblemTextPool.Empty();
	HB_ProblemTextBox->ClearChildren();
}

void UWG_EnemyProblem::NativeConstruct()
{
	Super::NativeConstruct();

	ResetContainerData();
}

void UWG_EnemyProblem::NativeDestruct()
{
	Super::NativeDestruct();
	
	ResetContainerData();
}