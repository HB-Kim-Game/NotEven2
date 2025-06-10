// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IngredientStruct.h"
#include "GameFramework/Actor.h"
#include "FoodFactory.generated.h"

UCLASS()
class NOTEVEN_API AFoodFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoodFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	class AConveyorBelt* StartBelt;

	UPROPERTY(EditAnywhere)
	TArray<FString> IngredientIDs;

	UPROPERTY(EditAnywhere, blueprintReadWrite)
	class UDataTable* IngredientTable;

	UPROPERTY(EditAnywhere, blueprintReadWrite)
	class UDataTable* PlaceTable;

	UPROPERTY(VisibleAnywhere)
	FIngredientData Data;

	UPROPERTY(VisibleAnywhere)
	FIngredientPlaceData PlaceData;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	FTimerHandle TimerHandle;
};
