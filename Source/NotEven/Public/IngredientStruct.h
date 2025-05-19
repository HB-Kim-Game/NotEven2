// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Public/IngredientEnums.h"
#include "IngredientStruct.generated.h"

/**
 * 
 */
USTRUCT(Atomic, BlueprintType)
struct FIngredientMeshPath : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EIngredientState State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StaticMeshPath;
};

USTRUCT(Atomic, BlueprintType)
struct FIngredientData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IngredientID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIngredientMeshPath> Ingredients;
};

USTRUCT(Atomic, BlueprintType)
struct FIngredientPlaceRule : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EIngredientState State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EPlacementRequirement> Requirement;
	
};

USTRUCT(Atomic, BlueprintType)
struct FIngredientPlaceData : public FTableRowBase
{
	GENERATED_BODY()

	// ...
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IngredientID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIngredientPlaceRule> PlacementRules;
};