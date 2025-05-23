// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CookingProgress.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API UCookingProgress : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* CookingProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Progress;
};
