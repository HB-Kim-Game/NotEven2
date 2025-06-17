// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayer.h"
#include "NiagaraComponent.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.h"
#include "GameFramework/GameStateBase.h"
#include "GameManager/NotEvenGameInstance.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ALobbyPlayer::ALobbyPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	RootComp->SetupAttachment(RootComponent);
	
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComp);

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ParticleComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void ALobbyPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		LobbyUI = CreateWidget<ULobbyUI>(GetWorld(), LobbyUIClass);
		LobbyUI->AddToViewport();

		LobbyUI->SetButtonEnabled(HasAuthority());
		
		if (auto gi = Cast<UNotEvenGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (!gi->bIsJoinSession)
			{
				LobbyUI->SearchStart();
				CreateSessionDelegate = gi->SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ALobbyPlayer::CompleteCreateSession);
				gi->CreateMySession(2);
			}
		}
	}
}

// Called every frame
void ALobbyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALobbyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALobbyPlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayer, PlayerIndex);
}

void ALobbyPlayer::OnRep_PlayerIndex()
{
	MeshComp->SetMaterial(0,PlayerIndex > 0 ? RedColor : BlueColor);
}

void ALobbyPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (HasAuthority())
	{
		PlayerIndex = GetWorld()->GetGameState()->PlayerArray.IndexOfByKey(GetPlayerState());
		OnRep_PlayerIndex();
		UE_LOG(LogTemp,Display,TEXT("Player Index is %d"),PlayerIndex);
	}
}

void ALobbyPlayer::PostNetInit()
{
	Super::PostNetInit();

	OnRep_PlayerIndex();
}

void ALobbyPlayer::CompleteCreateSession(FName SessionName, bool bWasSuccessful)
{
	LobbyUI->SearchComplete();
	if (auto gi = Cast<UNotEvenGameInstance>(GetWorld()->GetGameInstance()))
	{
		gi->SessionInterface->OnCreateSessionCompleteDelegates.Remove(CreateSessionDelegate);
	}
}

