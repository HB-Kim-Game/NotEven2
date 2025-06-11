// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/ImmovableObject.h"
#include "KitchenSink.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API AKitchenSink : public AImmovableObject
{
	GENERATED_BODY()
	
public:
	
	AKitchenSink();

	virtual void Interact(class ANotEvenPlayer* player) override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Interact(class ANotEvenPlayer* player);

	virtual void Washing(class ANotEvenPlayer* player);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Washing(class ANotEvenPlayer* player, float currentWashingProgress);
	
	UPROPERTY(EditAnywhere)
	class APlate* PlateObj;

	bool isInSink;
	
};
