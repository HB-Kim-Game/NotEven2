// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultUI.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API UResultUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
	class UTextBlock* StageText;
	
	UPROPERTY()
	TArray<class UTextBlock*> StarTexts;

	UPROPERTY()
	class UTextBlock* SuccessOrderCountText;
	UPROPERTY()
	class UTextBlock* FailureOrderCountText;
	UPROPERTY()
	class UTextBlock* ResultScoreText;
	
};
