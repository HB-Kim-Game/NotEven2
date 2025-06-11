// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoodIngredient.h"
#include "IngredientStruct.h"
#include "Interactable/MovableObject.h"
#include "Plate.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API APlate : public AMovableObject
{
	GENERATED_BODY()

public:
	
	APlate();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(class ANotEvenPlayer* player) override;
	
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Interact(class ANotEvenPlayer* player);

	virtual void SetGrab(bool bGrab) override;

	void OnPlate(AFoodIngredient* foodObj);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere)
	class ASubmitFood* submitFood;

	UPROPERTY(EditAnywhere)
	TMap<EPlatestate, UStaticMesh*> StateMeshMap;

	UPROPERTY()
	EPlatestate Platestate;
	
	UFUNCTION()
	void SetState(EPlatestate nextState);

	UPROPERTY()
	class UCookingProgress* WashingProgress;

	UFUNCTION()
	void Rep_CurrentWashingProgress();

	void AddWashingProgress(float addProgress);

	float GetCurrentWashingProgress() const;

	float GetMaxWashingProgress() const;

	void SetMaxWashingProgress(float progress);

	UFUNCTION(netMulticast, Reliable)
	void NetMulticast_SetCurrentState(EPlatestate next);

	UPROPERTY(ReplicatedUsing = Rep_CurrentWashingProgress)
	float CurrentWashingProgress = 0.f;
	float MaxWashingProgress = 100.f;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* WashWidgetComp;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* attachPoint;

	UPROPERTY(Replicated)
	EPlatestate CurrentState;
	
};
