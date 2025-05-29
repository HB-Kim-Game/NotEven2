// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PriceUI.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API UPriceUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PriceText;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PriceTopText;
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* TipProgress;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TipText;
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* FeverPanel;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* CoinAnimation;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* Success;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* Failed;
	
	void ShowCurrentScore();

	void SetOrderManager(class UOrderManager* orderManager);

protected:

	UPROPERTY()
	class UOrderManager* OrderManager;
	int32 MaxComboCount = 4;
	
	int32 PrevScore = 0;
};
