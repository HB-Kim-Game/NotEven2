// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UPlayTimeUI* PlayTime;

	UPROPERTY(meta = (BindWidget))
	class UOrderListViewer* OrderListViewer;

	UPROPERTY(meta = (BindWidget))
	class UPriceUI* PriceUI;

	UPROPERTY(meta = (BindWidget))
	class UResultUI* ResultUI;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Start;

	UPROPERTY(meta = (BindWidget))
	class UImage* Mask;
	
	void GameStart();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* MaskMaterial;
	UPROPERTY()
	class UMaterialInstanceDynamic* Dynamic_Mask;

	FTimerHandle StartHandle;

};
