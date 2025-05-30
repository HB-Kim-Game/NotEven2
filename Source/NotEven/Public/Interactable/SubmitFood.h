// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovableObject.h"
#include "IngredientEnums.h"
#include "IngredientStruct.h"
#include "SubmitFood.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API ASubmitFood : public AMovableObject
{
	GENERATED_BODY()

public:

	ASubmitFood();
	
	void AddIngredient(FIngredientData data, EIngredientState state, float CookingProgress, FIngredientPlaceData placeData);

	TArray<FRecipeIngredientData> GetIngredients() const;

	virtual void Interact(class ANotEvenPlayer* player) override;
	
protected:
	float MaxCookingProgress = 0.f;
	float CurrentCookingProgress = 0.f;
	
	UPROPERTY()
	TArray<FSubmitFoodIngredientData> Ingredients;

	UPROPERTY()
	class UDataTable* MeshTable;

	void FindMesh();
};
