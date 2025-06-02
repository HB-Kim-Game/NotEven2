// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImmovableObject.h"
#include "IngredientStruct.h"
#include "GameFramework/Actor.h"
#include "IngredientBox.generated.h"

UCLASS()
class NOTEVEN_API AIngredientBox : public AImmovableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIngredientBox();

	virtual void Interact(class ANotEvenPlayer* player) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FString IngredientID;

	UPROPERTY(EditAnywhere, blueprintReadWrite)
	class UDataTable* IngredientTable;

	UPROPERTY(EditAnywhere, blueprintReadWrite)
	class UDataTable* PlaceTable;

	UPROPERTY(VisibleAnywhere)
	FIngredientData Data;

	UPROPERTY(VisibleAnywhere)
	FIngredientPlaceData PlaceData;

	UPROPERTY(editAnywhere)
	class UStaticMeshComponent* IconMesh;

	UPROPERTY(editAnywhere)
	class UMaterialInterface* IconMaterial;

	UPROPERTY()
	class UMaterialInstanceDynamic* IconDynamic;
};
