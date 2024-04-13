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

	{
		HB_ProblemTextBox->ClearChildren();
		for (FString EachStr : Context)
		{
			UWG_ProblemText* ProblemText = LoadProblemText();
			if (ProblemText)
			{
				ProblemText->SetText(EachStr);
				ProblemText->SetVisibility(ESlateVisibility::Visible);
				HB_ProblemTextBox->AddChildToHorizontalBox(ProblemText);
			}
		}
	}
	
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

UWG_ProblemText* UWG_EnemyProblem::LoadProblemText()
{
	check(!ProblemTextClass.IsNull());
	TSubclassOf<UWG_ProblemText> WidgetClass = ProblemTextClass.LoadSynchronous();

	if (ensure(WidgetClass && !WidgetClass->HasAnyClassFlags(CLASS_Abstract)))
	{
		UWG_ProblemText* NewObject = CreateWidget<UWG_ProblemText>(GetOwningLocalPlayer()->GetPlayerController(GetWorld()), WidgetClass);

		return NewObject;
	}

	return nullptr;
}
