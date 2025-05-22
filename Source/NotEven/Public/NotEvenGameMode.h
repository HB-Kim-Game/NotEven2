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

	// 플레이어의 폰을 스폰 시도
	virtual void RestartPlayer(AController* NewPlayer) override;

	// 플레이어 캐릭터가 죽으면 호출
	UFUNCTION()
	virtual void PlayerDied(ANotEvenPlayer* Player);

	// 델리게이트를 바인딩할 시그니처
	UPROPERTY()
	FOnPlayerDiedSignature OnPlayerDied;


	

	
};
