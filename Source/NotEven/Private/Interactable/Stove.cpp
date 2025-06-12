// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Stove.h"

#include "CookingIcon.h"
#include "NotEvenPlayer.h"
#include "Pot.h"
#include "Animation/UMGSequencePlayer.h"
#include "Net/UnrealNetwork.h"

AStove::AStove()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/stove_single.stove_single'"));

	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}
	
	bReplicates = true;

	bIsInteractable = true;
}

void AStove::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);

	if (player->isGrab)
	{
		if (Pot)
		{
			Pot->Interact(player);
			return;
		}

		// 냄비가 없을경우, 손에 들고있는 것이 냄비일때
		if (auto pot = Cast<APot>(player->OwnedObj))
		{
			Pot = pot;
			player->DetachGrabObj(false);
			Pot->BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Pot->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachPoint"));
			return;
		}
	}
	
	NetMulticast_Interact(player);
}

void AStove::NetMulticast_Interact_Implementation(class ANotEvenPlayer* player)
{
	if (Pot == nullptr) return;
	
	Pot->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	auto detachPot = Pot;
	player->AttachGrabObj(detachPot);
	Pot = nullptr;
	
}

void AStove::Server_SpawnObject_Implementation()
{
	NetMulticast_SpawnObject();
}

void AStove::NetMulticast_SpawnObject_Implementation()
{
	auto spawnPot = GetWorld()->SpawnActor<APot>(PotClass);
	Pot = spawnPot;
	Pot->BoxComp->SetSimulatePhysics(false);
	Pot->BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Pot->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachPoint"));
}

void AStove::BeginPlay()
{
	Super::BeginPlay();
}

void AStove::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Pot && Pot->HasSubmitFood() && !Pot->bISBurned && HasAuthority()) AddProgress();
}

void AStove::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStove, Pot);
}

void AStove::AddProgress_Implementation()
{
	Pot->AddProgress(AddAmount * GetWorld()->GetDeltaSeconds());
}
