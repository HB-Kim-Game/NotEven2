// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/MovableObject.h"
#include "Pot.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API APot : public AMovableObject
{
	GENERATED_BODY()

	public:
	
	APot();

	virtual void BeginPlay() override;
	
	virtual void Interact(class ANotEvenPlayer* player) override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Interact(class ANotEvenPlayer* player);

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void AddProgress(float progress);

	bool HasSubmitFood();
	
protected:

	UPROPERTY(VisibleAnywhere, Replicated)
	class ASubmitFood* SubmitFood;
};
