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

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(Server, Reliable)
	virtual void InitializeIngredient(FIngredientData data, FIngredientPlaceData place);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_InitializeIngredient(FIngredientData data, FIngredientPlaceData place);

	FIngredientData GetIngredientData() const;

	EIngredientState GetIngredientState() const;

	FIngredientPlaceData GetIngredientPlaceData() const;

	void AddCookingProgress(float addProgress);

	float GetCurrentCookingProgress() const;

	float GetMaxCookingProgress() const;

	void SetState(EIngredientState newState);

	virtual void Interact(class ANotEvenPlayer* player) override;

	virtual void SetGrab(bool bGrab) override;

	void SetMaxCookingProgress(float progress);

	UFUNCTION()
	void Rep_CurrentCookingProgress();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetCurrentState(EIngredientState next);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void RemoveWidget(AActor* DestroyedActor);

protected:
	// ID, 이름, 상태에 따른 에셋 Path
	UPROPERTY()
	FIngredientData Data;

	// 조리 상태
	UPROPERTY()
	EIngredientState CurrentState;

	// 상태에 따른 올라갈 수 있는 장소 모음
	UPROPERTY()
	FIngredientPlaceData PlaceData;

	UPROPERTY()
	TMap<EIngredientState, UStaticMesh*> IngredientMeshes;

	// 현재 상태에서 요리된 
	UPROPERTY(ReplicatedUsing = Rep_CurrentCookingProgress)
	float CurrentCookingProgress = 0.f;
	float MaxCookingProgress = 0.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UIngredientActorIcon> IconClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCookingProgress> ProgressClass;
	
	UPROPERTY()
	class UIngredientActorIcon* IconWidget;

	UPROPERTY()
	class UCookingProgress* ProgressWidget;
	
	UFUNCTION(NetMulticast, Reliable)
	void NetRPC_Interact(class ANotEvenPlayer* player);
};