// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IngredientStruct.h"
#include "Interactable/ImmovableObject.h"
#include "TrashBox.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API ATrashBox : public AImmovableObject
{
	GENERATED_BODY()

public:
	ATrashBox();

	virtual void Interact(class ANotEvenPlayer* player) override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Interact(class ANotEvenPlayer* player);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* OverlapBox;

	void DestroyOverlapObjects();
};
