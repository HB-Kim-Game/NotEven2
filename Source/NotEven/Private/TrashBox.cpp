// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashBox.h"

#include "FoodIngredient.h"
#include "NotEvenPlayer.h"
#include "Plate.h"
#include "Pot.h"
#include "SubmitFood.h"
#include "Components/BoxComponent.h"

ATrashBox::ATrashBox()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/crate.crate'"));
	if (tempMesh.Succeeded())
	{
		MeshComp-> SetStaticMesh(tempMesh.Object); 
	}
	
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetRelativeLocation(FVector(0, 0, 60.f));

	OverlapBox->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	OverlapBox->SetupAttachment(BoxComp);

	bIsInteractable = true;
	bReplicates = true;
}

void ATrashBox::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
	
	NetMulticast_Interact(player);
}

void ATrashBox::NetMulticast_Interact_Implementation(class ANotEvenPlayer* player)
{
	// 만약에 플레이어가 isGrab 상태이면
	if (player && player -> isGrab)
	{
		AMovableObject* moveObject = player->OwnedObj;
		// moveObject을 Grad 하고 있으면
		if (moveObject)
		{
			//음식이면
			if (AFoodIngredient* food = Cast<AFoodIngredient>(moveObject))
			{
				// 음식을 파괴하고싶다
				player -> OwnedObj = nullptr;
				food-> Destroy(); 
				player->isGrab = false; // 들고 있는 상태 초기화
			}

			// 접시이면
			if (APlate* plate = Cast<APlate>(moveObject))
			{
				//접시의 제출음식을 파괴
				auto food = plate->submitFood;
				plate->submitFood = nullptr;
				
				if (food)
				{
					food->Destroy();
				}
			}

			if (APot* pot = Cast<APot>(moveObject))
			{
				auto food = pot->SubmitFood;
				pot->SubmitFood = nullptr;
				if (HasAuthority())
				{
					pot->ServerRPC_SetBoiled(false);
					pot->ServerRPC_SetBurned(false);	
				}
				
				if (food)
				{
					food->Destroy();
				}
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

	DestroyOverlapObjects();
}

void ATrashBox::DestroyOverlapObjects()
{
	TArray<AActor*> OverlappingActors;
	OverlapBox->GetOverlappingActors(OverlappingActors, AMovableObject::StaticClass());

	for (auto actor : OverlappingActors)
	{
		if (auto cast = Cast<AMovableObject>(actor))
		{
			if (cast->BoxComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision) continue;
			actor->Destroy();
		}
	}
}

