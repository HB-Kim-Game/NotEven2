// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImmovableObject.h"
#include "GameFramework/Actor.h"
#include "ConveyorBelt.generated.h"

UCLASS()
class NOTEVEN_API AConveyorBelt : public AImmovableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConveyorBelt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(class ANotEvenPlayer* player) override;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* OverlapBox;

	UPROPERTY()
	TArray<class AMovableObject*> OwnedObj;

protected:

	UPROPERTY(EditAnywhere)
	float Speed = 75.f;

	void MoveObject(float DeltaTime);

	UPROPERTY(EditAnywhere)
	class UArrowComponent* ArrowComp;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
