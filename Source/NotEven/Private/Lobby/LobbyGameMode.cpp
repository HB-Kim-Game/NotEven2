// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyGameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

AActor* ALobbyGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// 서버면 서버 스폰 자리 / 클라이언트면 클라이언트 스폰 자리
	FName DesiredTag;

	if (Player->IsLocalController() && HasAuthority())
	{
		DesiredTag = FName("ServerSpawn");
	}
	else
	{
		DesiredTag = FName("ClientSpawn");
	}

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* Start = *It;
		if (Start->PlayerStartTag == DesiredTag)
		{
			return Start;
		}
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}
