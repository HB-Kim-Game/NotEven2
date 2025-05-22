// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RecipeData.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API URecipeData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RecipeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCookingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentCookingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<struct FRecipeIngredientData> RequiredIngredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MeshAssetPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconImagePath;
};
