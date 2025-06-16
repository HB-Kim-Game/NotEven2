// Fill out your copyright notice in the Description page of Project Settings.


#include "KitchenTable.h"
#include "MovableObject.h"
#include "NotEvenPlayer.h"
#include "Plate.h"
#include "Pot.h"
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

	bIsInteractable = true;
}

void AKitchenTable::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);

	AMovableObject* temp = nullptr;
	
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
			if (auto onPotObj = Cast<APot>(moveObject))
			{
				onPotObj->Interact(player);
				return;
			}
		}
		
		temp = player->OwnedObj;
		player->DetachGrabObj(false);
	}

	NetMulticast_Interact(player, temp);
}

void AKitchenTable::Server_SpawnObject_Implementation()
{
	if (!bIsSpawnObject) return;
	auto spawnObj = GetWorld()->SpawnActor<AMovableObject>(SpawnObjectClass);
	if (auto plate = Cast<APlate>(spawnObj))
	{
		plate->SetState(EPlatestate::Clean);
	}
	SpawnObject(spawnObj);
}

void AKitchenTable::SpawnObject_Implementation(class AMovableObject* spawnObject)
{
	moveObject = spawnObject;
	moveObject->BoxComp->SetSimulatePhysics(false);
	moveObject->BoxComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision); 
	moveObject->AttachToComponent(attachBox,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AKitchenTable::BeginPlay()
{
	Super::BeginPlay();

}

void AKitchenTable::NetMulticast_Interact_Implementation(class ANotEvenPlayer* player, class AMovableObject* ownedObj)
{
	// 만약에 플레이어가 isGrab 상태이면
	if (ownedObj)
	{
		moveObject = ownedObj;
		moveObject->BoxComp->SetSimulatePhysics(false);
		moveObject->BoxComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		moveObject->AttachToComponent(attachBox,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	else
	{
		if (moveObject == nullptr) return;
		moveObject->BoxComp->SetSimulatePhysics(false);
		moveObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		player->AttachGrabObj(moveObject);
		moveObject= nullptr;
	}
}

void AKitchenTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
