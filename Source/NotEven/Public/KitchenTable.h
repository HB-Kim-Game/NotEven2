// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/ImmovableObject.h"
#include "KitchenTable.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API AKitchenTable : public AImmovableObject
{
	GENERATED_BODY()

public:

	AKitchenTable();

	virtual void Interact(class ANotEvenPlayer* player) override;
	
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Interact(class ANotEvenPlayer* player);

	UFUNCTION(Server, Reliable)
	void Server_SpawnObject();
	
	UFUNCTION(NetMulticast, Reliable)
	void SpawnObject(class AMovableObject* spawnObject);
	
protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* attachBox;

	UPROPERTY()
	class AMovableObject* moveObject;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMovableObject> SpawnObjectClass;
	
	UPROPERTY(EditAnywhere)
	bool bIsSpawnObject = false;
};
