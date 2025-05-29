// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RenderTargetUI.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API URenderTargetUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UImage* RenderImage;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* RenderMaterial;

	UPROPERTY(EditAnywhere)
	class UAnimationAsset* Dance;
	
	UPROPERTY()
	class UMaterialInstanceDynamic* DynamicMat;
	
	void SetTarget(class AActor* actor);

protected:

	virtual void NativeConstruct() override;
};
