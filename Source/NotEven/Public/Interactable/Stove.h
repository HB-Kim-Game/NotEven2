// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/ImmovableObject.h"
#include "Stove.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API AStove : public AImmovableObject
{
	GENERATED_BODY()

public:
	AStove();

	virtual void Interact(class ANotEvenPlayer* player) override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Interact(class ANotEvenPlayer* player);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual  void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_SpawnObject();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SpawnObject(class APot* pot);
	
protected:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Pot)
	class APot* Pot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APot> PotClass;

	UPROPERTY(EditAnywhere)
	float AddAmount = 10.f;

	UPROPERTY(EditAnywhere)
	class UPointLightComponent* FireComp;

	UFUNCTION(Server, Reliable)
	void AddProgress();

	UFUNCTION()
	void OnRep_Pot();
};
