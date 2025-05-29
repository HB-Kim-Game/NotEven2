// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/ImmovableObject.h"
#include "FoodSubmitTable.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API AFoodSubmitTable : public AImmovableObject
{
	GENERATED_BODY()

public:
	AFoodSubmitTable();

	virtual void BeginPlay() override;
	
	virtual void Interact(class ANotEvenPlayer* player) override;

	void SubmitFood(const TArray<struct FRecipeIngredientData>& ingredients);

	UPROPERTY()
	class UStaticMeshComponent* MeshComp;
	UPROPERTY()
	class UBoxComponent* BoxComp;
	
protected:
	UPROPERTY()
	class UOrderManager* OrderManager;
};
