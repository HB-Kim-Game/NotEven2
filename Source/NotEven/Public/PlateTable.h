// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/ImmovableObject.h"
#include "PlateTable.generated.h"

/**
 * 
 */

UCLASS()
class NOTEVEN_API APlateTable : public AImmovableObject
{
	GENERATED_BODY()

public:
	APlateTable();

	virtual void Interact(class ANotEvenPlayer* player) override;

	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere)
	class UBoxComponent* attachPoint;
	
	UPROPERTY()
	class AMovableObject* moveObject;

	UPROPERTY(editAnywhere)
	TSubclassOf<class APlate> plateFactory;
	
	UFUNCTION()
	void RespawnPlate();

	UFUNCTION()
	void MakePlate();

	
};
