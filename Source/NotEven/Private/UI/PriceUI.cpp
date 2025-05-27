// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PriceUI.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameManager/OrderManager.h"

void UPriceUI::ShowCurrentScore()
{
	if (!OrderManager) return;
	
	FString tipText = FString::FromInt(OrderManager->GetCurrentComboCount());
	FString scoreText = FString::FromInt(OrderManager->GetCurrentScore());
	
	TipText->SetText(FText::FromString(tipText));
	PriceText->SetText(FText::FromString(scoreText));

	TipProgress->SetPercent(static_cast<float>(OrderManager->GetCurrentComboCount()) / MaxComboCount);
}

void UPriceUI::SetOrderManager(class UOrderManager* orderManager)
{
	OrderManager = orderManager;
}
