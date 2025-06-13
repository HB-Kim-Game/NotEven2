// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IngredientEnums.h"
#include "Blueprint/UserWidget.h"
#include "CookingIcon.generated.h"

/**
 * 
 */

USTRUCT(Atomic, BlueprintType)
struct FIconValue
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BackgroundColor;
};

UCLASS()
class NOTEVEN_API UCookingIcon : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UImage* IconImage;
	UPROPERTY(meta=(BindWidget))
	class UImage* BackgroundImage;
	UPROPERTY(meta=(BindWidget))
	class UImage* Border;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* Appear;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* Warning;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* Normal;

	void ShowIcon(EIngredientState state);

	void ShowWarning(float Speed);

	void DisappearIcon();

	UPROPERTY()
	class UUMGSequencePlayer* WarningPlayer;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EIngredientState, FIconValue> Icons;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* WarningIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor WarningColor;
};
