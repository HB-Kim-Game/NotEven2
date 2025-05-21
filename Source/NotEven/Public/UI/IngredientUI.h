// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngredientUI.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API UIngredientUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* IconPanel;

	void ShowIngredient(struct FRecipeIngredientData data);
};
