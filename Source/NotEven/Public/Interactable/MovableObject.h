// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectInteractable.h"
#include "MovableObject.generated.h"

UCLASS()
class NOTEVEN_API AMovableObject : public AActor, public IObjectInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovableObject();

	virtual void Interact(class ANotEvenPlayer* player) override;

	virtual void SetGrab(bool bGrab);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
