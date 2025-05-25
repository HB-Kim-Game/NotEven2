// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PriceUI.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPriceUI::ShowCurrentScore(int32 AddScore)
{
	CurrentComboCount = AddScore > 0 ? FMath::Clamp(CurrentComboCount + 1, 0, 4) : 0;
	CurrentScore = FMath::Max(CurrentScore + AddScore, 0);

	FString tipText = FString::FromInt(CurrentComboCount);
	FString scoreText = FString::FromInt(CurrentComboCount);
	
	TipText->SetText(FText::FromString(tipText));
	PriceText->SetText(FText::FromString(scoreText));

	TipProgress->SetPercent(static_cast<float>(CurrentComboCount / MaxComboCount));
}
