// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayTimeUI.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayTimeUI::SetMaxTime(float maxTime)
{
	MaxTime = maxTime;
	CurrentTime = MaxTime;
}

void UPlayTimeUI::SetIsPlaying(bool playing)
{
	bIsPlaying = playing;
}

void UPlayTimeUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsPlaying) return;
	
	ShowRemainTime(InDeltaTime);
}

void UPlayTimeUI::ShowRemainTime(float deltaTime)
{
	CurrentTime -= deltaTime;
	
	if (CurrentTime <= 0.f) return;

	FString text = FString::Printf(TEXT("%02d:%02d"), static_cast<int>(CurrentTime) / 60, static_cast<int>(CurrentTime) % 60);
	TimeText->SetText(FText::FromString(text));

	float percent = CurrentTime / MaxTime;
	RemainProgress->SetPercent(percent);
	FLinearColor color = FMath::Lerp(FLinearColor::Red, FLinearColor::Green, percent);
	RemainProgress->SetFillColorAndOpacity(color);
}
