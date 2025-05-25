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
	class UProgressBar* TipProgress;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TipText;

	void ShowCurrentScore(int32 AddScore);

protected:
	int32 CurrentComboCount = 0;
	int32 MaxComboCount = 4;

	int32 CurrentScore = 0;
	
};
