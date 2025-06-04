// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficLights.generated.h"

UCLASS()
class NOTEVEN_API ATrafficLights : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrafficLights();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPointLightComponent* RedLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPointLightComponent* GreenLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPointLightComponent* YellowLight;

	void SetGreen();
	void SetRed();
	void SetYellow();
};
