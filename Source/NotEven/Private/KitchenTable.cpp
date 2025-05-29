// Fill out your copyright notice in the Description page of Project Settings.


#include "KitchenTable.h"

#include "FoodIngredient.h"
#include "ISourceControlProvider.h"
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
	
}

void AKitchenTable::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
	// 만약에 플레이어가 isGrab 상태이면
	if (player -> isGrab == true)
	{
		if (moveObject!=nullptr)
		{
			if(auto onplateObj = Cast<APlate>(moveObject))
			{
				//onplate 에 음식이 들어간다
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
		
		UE_LOG(LogTemp,Log,TEXT("ANotEvenPlayer::???"));
		moveObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		player->AttachGrabObj(moveObject);
	}
}

void AKitchenTable::BeginPlay()
{
	Super::BeginPlay();
}

void AKitchenTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
