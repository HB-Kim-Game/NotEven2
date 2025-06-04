// Fill out your copyright notice in the Description page of Project Settings.


#include "PlateTable.h"

#include "NotEvenPlayer.h"
#include "Plate.h"
#include "Components/BoxComponent.h"
#include "Framework/Text/Android/AndroidPlatformTextField.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"

APlateTable::APlateTable()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/kitchentable_A.kitchentable_A'"));
	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}

	attachPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("AttachPoint"));
	attachPoint-> SetupAttachment(BoxComp);
	attachPoint->SetBoxExtent(FVector(75, 75, 75));
	attachPoint ->SetRelativeLocation(FVector(0, 0, 75));
	attachPoint ->SetRelativeScale3D(FVector(1,1,0.25));
}

void APlateTable::Interact(class ANotEvenPlayer* player)
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
		moveObject->AttachToComponent(attachPoint,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		player->DetachGrabObj();
		moveObject->BoxComp->SetSimulatePhysics(false);
	}
	else
	{
		if (moveObject == nullptr)
			return;
		moveObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		player->AttachGrabObj(moveObject);
		moveObject= nullptr;
	}
}

void APlateTable::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle,this, &APlateTable::MakePlate,10,true);
	UE_LOG(LogTemp,Warning,TEXT("plate"));
}

void APlateTable::RespawnPlate()
{
	
}

void APlateTable::MakePlate()
{
	FVector loc = attachPoint->GetComponentLocation();
	FRotator rot = attachPoint->GetComponentRotation();
	
	auto* newplate = GetWorld()->SpawnActor<APlate>(plateFactory,loc,rot);
	newplate ->AttachToComponent(attachPoint,FAttachmentTransformRules::KeepWorldTransform);
	moveObject = newplate;
	
}
