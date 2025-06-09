// Fill out your copyright notice in the Description page of Project Settings.


#include "KitchenTable.h"
#include "MovableObject.h"
#include "NotEvenPlayer.h"
#include "Plate.h"
#include "Components/BoxComponent.h"

AKitchenTable::AKitchenTable()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh
	(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/kitchencounter_straight_B.kitchencounter_straight_B'"));

	if (tempMesh.Succeeded())
	{
		MeshComp ->SetStaticMesh(tempMesh.Object);
	}

	attachBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttachBox"));
	attachBox-> SetupAttachment(BoxComp);
	attachBox->SetBoxExtent(FVector(75, 75, 32));
	attachBox ->SetRelativeLocation(FVector(0, 0, 75));
	attachBox ->SetRelativeScale3D(FVector(1,1,0.25));

	bReplicates = true;
}

void AKitchenTable::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);

	NetMulticast_Interact(player);
}

void AKitchenTable::Server_SpawnObject_Implementation()
{
	SpawnObject();
}

void AKitchenTable::SpawnObject_Implementation()
{
	if (bIsSpawnObject)
	{
		auto spawnObj = GetWorld()->SpawnActor<AMovableObject>(SpawnObjectClass);
		moveObject = spawnObj;
		moveObject->BoxComp->SetSimulatePhysics(false);
		moveObject->BoxComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision); 
		moveObject->AttachToComponent(attachBox,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void AKitchenTable::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Server_SpawnObject();
	}
}

void AKitchenTable::NetMulticast_Interact_Implementation(class ANotEvenPlayer* player)
{
	// 만약에 플레이어가 isGrab 상태이면
	if (player -> isGrab == true)
	{
		if (moveObject!=nullptr)
		{
			if(auto onplateObj = Cast<APlate>(moveObject))
			{
				//onplate 에 음식이 들어간다
				onplateObj->Interact(player);
				return;
			}
			else
			{
				return;
			}
		}
		// moveObject을 Grad 하고 있으면
		moveObject = player->OwnedObj;
		moveObject->AttachToComponent(attachBox,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		player->DetachGrabObj();
		moveObject->BoxComp->SetSimulatePhysics(false);
	}
	else
	{
		if (moveObject == nullptr)
			return;
		
		moveObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		auto attachObj = moveObject;
		player->AttachGrabObj(attachObj);
		moveObject= nullptr;
	}
}

void AKitchenTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
