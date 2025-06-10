// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/ImmovableObject.h"
#include "FoodSubmitTable.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API AFoodSubmitTable : public AImmovableObject
{
	GENERATED_BODY()

public:
	AFoodSubmitTable();

	virtual void BeginPlay() override;
	
	virtual void Interact(class ANotEvenPlayer* player) override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Interact(class ANotEvenPlayer* player);
	void SubmitFood(const TArray<struct FRecipeIngredientData>& ingredients, class APlate* plate);

	FTimerHandle PlateSpawnTimer;
	
protected:
	UPROPERTY()
	class AOrderManager* OrderManager;

	UPROPERTY()
	class APlateTable* plateTable;
	
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* TextWidgetComp;
	UPROPERTY()
	TSubclassOf<class USubmitTableUI> TextClass;
	UPROPERTY()
	class USubmitTableUI* TextWidget;
};
