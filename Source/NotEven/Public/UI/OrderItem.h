// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ListViewer/UIViewerItemBase.h"
#include "OrderItem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOrderFailed, class URecipeData*)
/**
 * 
 */
UCLASS()
class NOTEVEN_API UOrderItem : public UUIViewerItemBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* IngredientPanel;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* RemainProgress;

	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UIngredientUI> IngredientUIClass;

	virtual void FetchData(UObject* Data) override;

	virtual void Selected() override;
	virtual void Deselected() override;

	UPROPERTY()
	TArray<class UIngredientUI*> Ingredients;

	FOnOrderFailed OnOrderFailed;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
	class URecipeData* RecipeData;

	float CurrentCookingTime;

	bool bIsReceiveOrder = false;
};
