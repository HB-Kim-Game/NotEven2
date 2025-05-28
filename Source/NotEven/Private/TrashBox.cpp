// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashBox.h"

#include "FoodIngredient.h"
#include "NotEvenPlayer.h"
#include "Components/BoxComponent.h"

ATrashBox::ATrashBox()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetBoxExtent(FVector(50, 50, 50));
	BoxComp-> SetCollisionProfileName(TEXT("UnGrabObj"));
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp -> SetupAttachment(BoxComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded())
	{
		MeshComp-> SetStaticMesh(tempMesh.Object); 
	}
}

void ATrashBox::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);

	// 만약에 플레이어가 isGrab 상태이면
	if (player && player -> isGrab)
	{
		AMovableObject* moveObject = player->OwnedObj;
		// moveObject을 Grad 하고 있으면
		if (moveObject)
		{
			//moveObject가 음식이면
			if (AFoodIngredient* food = Cast<AFoodIngredient>(moveObject))
			{
				// 음식을 파괴하고싶다
				player -> OwnedObj = nullptr;
				food-> Destroy(); 
				player->isGrab = false; // 들고 있는 상태 초기화
			}
		}
	}
	
	
}

void ATrashBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrashBox::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
}

