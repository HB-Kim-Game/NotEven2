// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubmitFoodUI.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API USubmitFoodUI : public UUserWidget
{
	GENERATED_BODY()

	public:
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* Parent;

	UPROPERTY()
	TArray<class UImage*> IconImages;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* IconMaterialInstance;

	UPROPERTY()
	class UMaterialInstanceDynamic* IconMatDynamic;

	void ShowIconImage(TArray<struct FRecipeIngredientData> data);

	virtual void NativeConstruct() override;

	void SetMaterial();
};
