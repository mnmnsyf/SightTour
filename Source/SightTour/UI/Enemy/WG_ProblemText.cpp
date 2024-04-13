// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/WG_ProblemText.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UWG_ProblemText::SetText(FString ProblemText)
{
	check(T_ProblemText);

	if (ProblemText.IsEmpty())
	{
		if (!I_TextBlank)
		{
			T_ProblemText->SetVisibility(ESlateVisibility::Visible);
			T_ProblemText->SetText(FText::FromString(FString("___")));
		}
		else
		{
			T_ProblemText->SetVisibility(ESlateVisibility::Collapsed);
			I_TextBlank->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (I_TextBlank)
		{
			I_TextBlank->SetVisibility(ESlateVisibility::Collapsed);
		}
		T_ProblemText->SetVisibility(ESlateVisibility::Visible);
		T_ProblemText->SetText(FText::FromString(ProblemText));
	}
}
