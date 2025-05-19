// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovableObject.h"
#include "IngredientEnums.h"
#include "IngredientStruct.h"
#include "FoodIngredient.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API AFoodIngredient : public AMovableObject
{
	GENERATED_BODY()

public:

	AFoodIngredient();

	virtual void BeginPlay() override;

	// ID, 이름, 상태에 따른 에셋 Path
	UPROPERTY()
	FIngredientData Data;

	// 조리 상태
	UPROPERTY()
	EIngredientState CurrentState;

	// 상태에 따른 올라갈 수 있는 장소 모음
	UPROPERTY()
	FIngredientPlaceData PlaceData;

	// 현재 상태에서 요리된 정도
	UPROPERTY()
	float CookingProgress = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComp;
};
