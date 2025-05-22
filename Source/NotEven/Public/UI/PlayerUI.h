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

protected:
	virtual void NativeConstruct() override;
};
