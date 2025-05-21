// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VehicleManager.generated.h"

UCLASS()
class NOTEVEN_API AVehicleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVehicleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class AVehicle> VehicleFactory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UArrowComponent* SpawnPoint;

	UFUNCTION()
	void MakeVehicle();

	FTimerHandle MakeVehicleTimer;
};
