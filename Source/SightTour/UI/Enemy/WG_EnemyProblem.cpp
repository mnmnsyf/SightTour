// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/WG_EnemyProblem.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

void UWG_EnemyDialogue::UpdateProblemContext(TArray<FString> Context)
{
	if (HB_ProblemBox)
	{
		HB_ProblemBox->ClearChildren();
		for (FString EachStr : Context)
		{
			if (EachStr.IsEmpty())
			{
				UImage* AnswerBlankImage = Cast<UImage>(AnswerBlankImageClass.Get());
				HB_ProblemBox->AddChild(AnswerBlankImage);
				//HB_ProblemBox->AddChild();
			}
			else
			{
				
			}
		}
	}
}
