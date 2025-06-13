// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/ImmovableObject.h"
#include "CuttingBoard.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API ACuttingBoard : public AImmovableObject
{
	GENERATED_BODY()

public:	
	ACuttingBoard();

	virtual void Interact(class ANotEvenPlayer* player) override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Interact(class ANotEvenPlayer* player);
	
	virtual void Cutting(class ANotEvenPlayer* player);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Cutting(class ANotEvenPlayer* player);
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* attachBox;

	UPROPERTY()
	class AFoodIngredient* moveObject;

	bool isOnCuttingBoard;
	
	float currentCount=0;
	float maxCount=10;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* InteractMontage;

	
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* CuttingEffect;
};
