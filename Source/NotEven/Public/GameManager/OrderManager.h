// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/WidgetAnimationEvents.h"
#include "GameFramework/Actor.h"
#include "OrderManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnGameStart);
DECLARE_MULTICAST_DELEGATE(FOnGameEnd);

UCLASS()
class NOTEVEN_API AOrderManager : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AOrderManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

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

	bool CheckOrder(const TArray<struct FRecipeIngredientData>& ingredients);

	TArray<class URecipeData*> RemoveOrder(class URecipeData* data, bool isSuccess);

	float GetMaxTime() const;
	
	FOnGameStart OnGameStart;
	FOnGameEnd OnGameEnd;

	FWidgetAnimationDynamicEvent OnFinishedTimeOverAnim;
	FWidgetAnimationDynamicEvent OnFinishedStartAnim;
	
	void InitWidget();

protected:
	int32 CurrentSuccessOrder = 0;
	int32 CurrentFailedOrder = 0;
	int32 CurrentScore = 0;
	int32 SuccessScore = 0;
	int32 TipScore = 0;
	int32 FailureScore = 0;
	int32 CurrentComboCount = 0;

	float MaxTime = 180.f;
	
	UPROPERTY(Replicated)
	float CurrentTime = MaxTime;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetCurrentTime(float DeltaTime);
	UFUNCTION(NetMulticast, Reliable)
	void NetRPC_ShowCurrentTime(float curTime);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FTimerHandle TimerHandle;

	UPROPERTY(Replicated)
	bool IsPlaying = false;

	UFUNCTION()
	void GameEnd();

	UFUNCTION()
	void GameStart();
};
