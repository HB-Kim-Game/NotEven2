// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngredientActorIcon.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API UIngredientActorIcon : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* IconMaterialInstance;

	UPROPERTY()
	class UMaterialInstanceDynamic* IconMatDynamic;

	virtual void NativeConstruct() override;

	void SetIconImage(class UTexture2D* Icon);
};
