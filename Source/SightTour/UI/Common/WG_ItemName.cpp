// Fill out your copyright notice in the Description page of Project Settings.

#include "WG_ItemName.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

void UWG_ItemName::SetText(FString ProblemText)
{
	check(T_ItemInfo);

	if (ProblemText.IsEmpty())
	{
		T_ItemInfo->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		T_ItemInfo->SetVisibility(ESlateVisibility::Visible);
		T_ItemInfo->SetText(FText::FromString(ProblemText));
	}
}
