// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoodIngredient.h"
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

	virtual void SetGrab(bool bGrab) override;

	void OnPot(class AFoodIngredient* food);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetBoiled(bool isBoiled);
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetBurned(bool isBurned);

	UPROPERTY(ReplicatedUsing=OnRep_Burned)
	bool bISBurned = false;

	UPROPERTY(ReplicatedUsing=OnRep_Boiled)
	bool bIsBoiled = false;

	UPROPERTY(VisibleAnywhere)
	class ASubmitFood* SubmitFood;

protected:

	UFUNCTION()
	void OnRep_Burned();

	UFUNCTION()
	void OnRep_Boiled();
};
