// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayTimeUI.generated.h"

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
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Countdown;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* TimeOver;
	
	void SetMaxTime(class AOrderManager* orderManager);

	void ShowRemainTime(float currentTime);
	
protected:
	
	float MaxTime = 0.0f;

	UPROPERTY()
	class AOrderManager* OrderManager;
};