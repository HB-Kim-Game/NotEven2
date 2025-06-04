// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PriceUI.h"

#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameManager/OrderManager.h"

void UPriceUI::ShowCurrentScore()
{
	if (!OrderManager) return;
	
	FString tipText = FString::FromInt(OrderManager->GetCurrentComboCount());
	FString scoreText = FString::FromInt(OrderManager->GetCurrentScore());

	int32 distance = OrderManager->GetCurrentScore() - PrevScore;

	FString distanceText = FString::FromInt(distance);
	if (distance > 0)
	{
		distanceText.InsertAt(0, TEXT("+"));
	}
	
	TipText->SetText(FText::FromString(tipText));
	PriceText->SetText(FText::FromString(scoreText));
	PriceTopText->SetText(FText::FromString(distanceText));
	
	if (OrderManager->GetCurrentComboCount() == MaxComboCount)
	{
		FeverPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		FeverPanel->SetVisibility(ESlateVisibility::Hidden);
	}

	if (distance > 0)
	{
		StopAllAnimations();
		PlayAnimation(CoinAnimation);
		PlayAnimation(Success);
	}
	else if (distance < 0)
	{
		StopAllAnimations();
		PlayAnimation(Failed);
	}

	PrevScore = OrderManager->GetCurrentScore();
	TipProgress->SetPercent(static_cast<float>(OrderManager->GetCurrentComboCount()) / MaxComboCount);
}

void UPriceUI::SetOrderManager(class AOrderManager* orderManager)
{
	OrderManager = orderManager;
}
