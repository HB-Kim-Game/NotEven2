// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrderManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NOTEVEN_API UOrderManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOrderManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TSubclassOf<class UPlayerUI> PlayerUIClass;
	
	UPROPERTY()
	class UPlayerUI* PlayerUI;

	UPROPERTY()
	class UDataTable* OrderTable;

	UPROPERTY()
	TArray<class URecipeData*> OrderList;

	UPROPERTY()
	TArray<FName> RecipeNameList;

	int32 GetCurrentScore() const;
	int32 GetCurrentComboCount() const;

	void AddScore(int32 addScore);

	void AddSuccess(int32 price);
	void AddFailure(int32 price);

	void AddOrder();

	void RefreshOrder();

	TArray<class URecipeData*> RemoveOrder(class URecipeData* data, bool isSuccess);

protected:
	int32 CurrentSuccessOrder = 0;
	int32 CurrentFailedOrder = 0;
	int32 CurrentScore = 0;
	int32 SuccessScore = 0;
	int32 TipScore = 0;
	int32 FailureScore = 0;
	int32 CurrentComboCount = 0;
	
	FTimerHandle TimerHandle;
};
