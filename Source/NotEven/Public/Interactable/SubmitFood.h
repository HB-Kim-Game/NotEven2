// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovableObject.h"
#include "IngredientEnums.h"
#include "IngredientStruct.h"
#include "SubmitFood.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API ASubmitFood : public AMovableObject
{
	GENERATED_BODY()

public:

	ASubmitFood();
	
	void AddIngredient(FIngredientData data, EIngredientState state, float CookingProgress, FIngredientPlaceData placeData);

	TArray<FRecipeIngredientData> GetIngredients() const;

	virtual void Interact(class ANotEvenPlayer* player) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void AddProgress(float progress);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	float GetCurrentCookingProgress() const;
	float GetMaxCookingProgress() const;

	void SetState(EIngredientState next);
	
protected:
	float MaxCookingProgress = 0.f;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_CurrentCookingProgress)
	float CurrentCookingProgress = 0.f;

	UFUNCTION()
	void OnRep_CurrentCookingProgress();
	
	UPROPERTY()
	TArray<FSubmitFoodIngredientData> Ingredients;

	UPROPERTY()
	class UDataTable* MeshTable;

	void FindMesh();

	UPROPERTY()
	class UWidgetComponent* IconWidgetComp;
	UPROPERTY()
	TSubclassOf<class USubmitFoodUI> IconClass;
	UPROPERTY()
	class USubmitFoodUI* IconWidget;
	
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* ProgressWidgetComp;

	UPROPERTY()
	class UCookingProgress* ProgressWidget;

	UPROPERTY()
	class APlayerCameraManager* PlayerCameraManager;
	
};
