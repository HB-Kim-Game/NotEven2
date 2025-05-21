// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NotEvenPlayer.h"
#include "GameFramework/GameModeBase.h"
#include "NotEvenGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, ANotEvenPlayer*, Player);
/**
 * 
 */
UCLASS()
class NOTEVEN_API ANotEvenGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	virtual void BeginPlay() override;
	
	const FOnPlayerDiedSignature& GetOnPlayerDied() const {return OnPlayerDied;}

	virtual void RestartPlayer(AController* NewPlayer) override;

	UFUNCTION()
	virtual void PlayerDie(ANotEvenPlayer* Player);

	UPROPERTY()
	FOnPlayerDiedSignature OnPlayerDied;
	

	
};
