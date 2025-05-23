// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IngredientEnums.h"
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

	UPROPERTY(EDITAnywhere, BlueprintReadWrite)
	float MaxCookingProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EIngredientState> PossibleStates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIngredientMeshPath> MeshPaths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconAssetPath;
};

USTRUCT(Atomic, BlueprintType)
struct FRecipeIngredientData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IngredientID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EIngredientState> RequireStates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconImagePath;
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
	GENERATED_USTRUCT_BODY()

	// ...
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IngredientID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIngredientPlaceRule> PlacementRules;
};

USTRUCT(Atomic, BlueprintType)
struct FRecipe : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RecipeID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCookingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRecipeIngredientData> RequiredIngredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MeshAssetPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconImagePath;
};