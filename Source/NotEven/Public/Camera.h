// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera.generated.h"

UCLASS()
class NOTEVEN_API ACamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SceneComp;
	
	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;
	
	float CurrentDistance;

	FVector CameraLocation;
	float MoveRange = 100.f;
	
	float MinDistance = 1500.f;
	float MaxDistance = 2000.f;
	
	float MaxFov = 38.f;
	float MinFov = 35.f;
	
};
