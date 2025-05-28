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

	UResultUI(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StageText;
	
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
	class UTextBlock* SuccessOrderCountText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SuccessScoreText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TipScoreText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FailureOrderCountText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FailureScoreText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResultScoreText;

	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Appear;

	void ShowResult(class UResultData* data);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedRestartButton();
	UFUNCTION()
	void OnClickedQuitButton();
	
protected:
	UPROPERTY()
	class UDataTable* ScoreTable;
};
