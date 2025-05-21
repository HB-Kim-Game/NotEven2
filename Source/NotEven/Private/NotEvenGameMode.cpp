// Fill out your copyright notice in the Description page of Project Settings.


#include "NotEvenGameMode.h"

#include "NotEvenPlayer.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ANotEvenGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &ANotEvenGameMode::PlayerDie);
	}
}

void ANotEvenGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void ANotEvenGameMode::PlayerDie(ANotEvenPlayer* Player)
{
	AController* CharactorController = Player->GetController();
	RestartPlayer(CharactorController);
}

// void ANotEvenGameMode::RequestRespawn(AController* Controller)
// {
// 	if (!Controller)
// 		return;
//
// 	FTimerHandle RespawnTimerHandle;
// 	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle,FTimerDelegate::CreateLambda([this]()
// 	{
// 		AActor* spawnPoint = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
//
// 		//스폰포인트 찾기
// 		if (spawnPoint)
// 		{
// 			FTransform SpawnTransform = spawnPoint->GetActorTransform();
// 			ANotEvenPlayer* newplayer = GetWorld()->SpawnActor<ANotEvenPlayer>(DefaultPawnClass, SpawnTransform);
//
// 			if (newplayer)
// 			{
// 				Controller->Possess(newplayer);
// 				newplayer->ResetPlayer();
// 			}
// 		}
// 	}), 3.f, false); // 3초 후 리스폰
// }
