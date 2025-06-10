// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayTimeUI.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameManager/OrderManager.h"

void UPlayTimeUI::SetMaxTime(AOrderManager* orderManager)
{
	OrderManager = orderManager;
	MaxTime = orderManager->GetMaxTime();
	ShowRemainTime(MaxTime);
}

void UPlayTimeUI::ShowRemainTime(float currentTime)
{
	if (currentTime <= 0.f)
	{
		PlayAnimation(TimeOver);
		return;	
	}

	if (currentTime <= 30.f)
	{
		if (!IsPlayingAnimation())
		PlayAnimation(Countdown, 0, 0);
	}

	FString text = FString::Printf(TEXT("%02d:%02d"), static_cast<int>(currentTime) / 60, static_cast<int>(currentTime) % 60);
	TimeText->SetText(FText::FromString(text));

	float percent = currentTime / MaxTime;
	RemainProgress->SetPercent(percent);
	FLinearColor color = FMath::Lerp(FLinearColor::Red, FLinearColor::Green, percent);
	RemainProgress->SetFillColorAndOpacity(color);
}

