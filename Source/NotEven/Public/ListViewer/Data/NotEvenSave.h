// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "NotEvenSave.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API UNotEvenSave : public USaveGame
{
	GENERATED_BODY()

	public:
	UPROPERTY()
	FString StageID;

	UPROPERTY()
	int32 HighScore;
};
