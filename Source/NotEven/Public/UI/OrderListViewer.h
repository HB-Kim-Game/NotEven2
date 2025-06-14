// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ListViewer/UIListViewerBase.h"
#include "OrderListViewer.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API UOrderListViewer : public UUIListViewerBase
{
	GENERATED_BODY()

public:
	virtual int32 GetCursor() const override;

	virtual void SetCursor(int32 value) override;

	virtual UObject* GetSelectedItem() const override;

	virtual void MoveCursor(int32 Gap, bool bIsRefresh = false) override;

	virtual void RefreshOnDataFetched() override;

	void SetOrderManager(class AOrderManager* orderManager);

	bool CheckOrderSuccess(const TArray<struct FRecipeIngredientData>& data);

	int32 GetSpawnItemsCount() const;
	
	FWidgetAnimationDynamicEvent OnAnimFinished;

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRefresh();
	
	virtual void InitializeItem() override;

	int32 CurrentOrderCount = 0;

	UPROPERTY()
	class AOrderManager* OrderManager;

	UPROPERTY(EditAnywhere)
	USoundBase* SuccessCoinSound;
	UPROPERTY(EditAnywhere)
	USoundBase* SuccessSound;
	UPROPERTY(EditAnywhere)
	USoundBase* FailSound;
	UPROPERTY(EditAnywhere)
	USoundBase* NoOrderSound;
};
