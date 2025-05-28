// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoodIngredient.h"
#include "IngredientStruct.h"
#include "Interactable/MovableObject.h"
#include "Plate.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API APlate : public AMovableObject
{
	GENERATED_BODY()

public:
	
	APlate();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(class ANotEvenPlayer* player) override;

	virtual void SetGrab(bool bGrab) override;

	void OnPlate(AFoodIngredient* foodObj);
	
	
};
