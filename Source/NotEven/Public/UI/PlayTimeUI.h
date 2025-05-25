// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayTimeUI.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnGameStart);
DECLARE_MULTICAST_DELEGATE(FOnGameEnd);
/**
 * 
 */
UCLASS()
class NOTEVEN_API UPlayTimeUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeText;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* RemainProgress;

	FOnGameStart OnGameStart;
	FOnGameEnd OnGameEnd;
	
	void SetMaxTime(float maxTime);

	void SetIsPlaying(bool playing);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	float CurrentTime = 0.f;
	float MaxTime = 0.f;

	bool bIsPlaying = false;

	void ShowRemainTime(float deltaTime);
};
