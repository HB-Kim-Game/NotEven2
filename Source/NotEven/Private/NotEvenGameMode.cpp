// Fill out your copyright notice in the Description page of Project Settings.


#include "NotEvenGameMode.h"
#include "NotEvenPlayer.h"
#include "Camera/CameraActor.h"
#include "GameFramework/GameStateBase.h"
#include "GameManager/NotEvenGameInstance.h"
#include "Kismet/GameplayStatics.h"

ANotEvenGameMode::ANotEvenGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANotEvenGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANotEvenGameMode::RestartPlayer(AController* NewPlayer)
{
	auto player = NewPlayer->GetPawn();

	if (player != nullptr)
	{
		NewPlayer->UnPossess();
		player->Destroy();
	}
	
	Super::RestartPlayer(NewPlayer);
	
	// 월드에서 BP_Camera 를 찾는다.
	// BP_Camera 를 viewTarget 로 지정한다.
	TArray<AActor*> outPawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), outPawns);
	for (auto actor : outPawns)
	{
		if (actor->GetActorNameOrLabel().Contains(TEXT("BP_Camera")))
		{
			auto pc = Cast<APlayerController>(NewPlayer);
			pc->PlayerCameraManager->SetViewTarget(actor);
			
			break;
		}
	}
}

void ANotEvenGameMode::PlayerDied(ANotEvenPlayer* Player)
{
	// 캐릭터의 플레이어 컨트롤러에 대한 래퍼런스 구하기
	// AController* CharactorController = Player->GetController();
	// RestartPlayer(CharactorController);
}

void ANotEvenGameMode::NotifyClientLoaded()
{
	if (isPlaying) return;

	ClientReady++;
	
	if (ClientReady >= Cast<UNotEvenGameInstance>(GetGameInstance())->ConnectedPlayers)
	{
		StartGame();
	}
}

void ANotEvenGameMode::StartGame()
{
	isPlaying = true;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
		{
			APlayerController* pc = it->Get();
			UE_LOG(LogTemp,Warning,TEXT("Player Controller : %d"), it.GetIndex());
		
			if (auto p = Cast<ANotEvenPlayer>(pc->GetPawn()))
			{
				p->NetRPC_GameStart();
			}
		}
	}), 2.f, false);
	
	
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
