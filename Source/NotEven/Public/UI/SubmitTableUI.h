// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubmitTableUI.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API USubmitTableUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SubmitText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Appear;
};
