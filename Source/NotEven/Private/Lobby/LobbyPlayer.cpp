// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayer.h"
#include "NiagaraComponent.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.h"
#include "GameManager/NotEvenGameInstance.h"

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

	LobbyUI = CreateWidget<ULobbyUI>(GetWorld(), LobbyUIClass);
	LobbyUI->AddToViewport();

	if (auto gi = Cast<UNotEvenGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (!gi->IsInRoom())
		{
			gi->CreateMySession(2);
			LobbyUI->SearchStart();
			gi->SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ALobbyPlayer::CompleteCreateSession);
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

void ALobbyPlayer::CompleteCreateSession(FName SessionName, bool bWasSuccessful)
{
	LobbyUI->SearchComplete();
}

