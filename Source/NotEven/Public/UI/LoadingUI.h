// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingUI.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API ULoadingUI : public UUserWidget
{
	GENERATED_BODY()

public:

	ULoadingUI(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StarOneText;
	UPROPERTY(meta = (BindWidget))
	class UImage* StarOneImage;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StarTwoText;
	UPROPERTY(meta = (BindWidget))
	class UImage* StarTwoImage;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StarThreeText;
	UPROPERTY(meta = (BindWidget))
	class UImage* StarThreeImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StageText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BestScoreText;

	virtual void NativeConstruct() override;

	void ShowStageInfo();

protected:
	UPROPERTY()
	class UDataTable* ScoreTable;
};
