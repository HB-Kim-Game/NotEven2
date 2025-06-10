// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Pot.h"

#include "SubmitFood.h"
#include "Net/UnrealNetwork.h"

APot::APot()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/pot_A.pot_A'"));

	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}

	MeshComp->SetRelativeLocation(FVector(0, 0, -10.f));

	bReplicates = true;
}

void APot::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->SetSimulatePhysics(false);
}

void APot::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
}

void APot::NetMulticast_Interact_Implementation(class ANotEvenPlayer* player)
{
	// Plate 처럼
}

void APot::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APot, SubmitFood);
}

void APot::AddProgress(float progress)
{
	SubmitFood->AddProgress(progress);
}

bool APot::HasSubmitFood()
{
	return SubmitFood != nullptr;
}

void APot::SetGrab(bool bGrab)
{
	Super::SetGrab(bGrab);

	// 잡았을 경우
	if (bGrab)
	{
		// 물리 끄기
		BoxComp->SetSimulatePhysics(false);
		// NoCollision으로 설정
		BoxComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision); 
	}
	// 놓았을 경우
	else
	{
		//물리 적용
		BoxComp->SetSimulatePhysics(true);
		BoxComp->SetMassScale(NAME_None,1.f);
		//떨어질 때 회전값 잠금
		BoxComp->BodyInstance.bLockRotation = true;

		//떨어질 때 X,Y축의 이동을 잠그고 Z축으로만 이동하게 설정
		BoxComp->BodyInstance.bLockXTranslation = true; // X축
		BoxComp->BodyInstance.bLockYTranslation = true; // Y축
		
		BoxComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics); // Collision Enabled(QueryAndPhysics)으로 설정
	}
}
