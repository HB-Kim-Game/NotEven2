// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ListViewer/UIViewerItemBase.h"
#include "OrderItem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOrderFailed, class URecipeData*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOrderSuccess, class URecipeData*)
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
	class URecipeData* GetRecipeData() const;

	virtual void Selected() override;
	virtual void Deselected() override;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Success;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Failed;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Appear;
	
	UPROPERTY()
	TArray<class UIngredientUI*> Ingredients;

	FOnOrderFailed OnOrderFailed;
	FOnOrderSuccess OnOrderSuccess;

	FWidgetAnimationDynamicEvent OnAnimationFinished;
	
	bool bIsChecked = false;
	
protected:

	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY()
	class URecipeData* RecipeData;

	float CurrentCookingTime;

	bool bIsReceiveOrder = false;

	UFUNCTION()
	void AnimationFinished();
};
