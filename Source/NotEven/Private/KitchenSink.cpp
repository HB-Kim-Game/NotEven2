// Fill out your copyright notice in the Description page of Project Settings.


#include "KitchenSink.h"

#include "NotEvenPlayer.h"
#include "Plate.h"

AKitchenSink::AKitchenSink()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/kitchentable_sink_large.kitchentable_sink_large' "));
	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}

	BoxComp->SetRelativeScale3D(FVector(1.35,0,0));
}

void AKitchenSink::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
	
	NetMulticast_Interact(player);
}

void AKitchenSink::NetMulticast_Interact_Implementation(class ANotEvenPlayer* player)
{
	if (player->isGrab == true)
	{
		auto plate = Cast<APlate>(player->OwnedObj);
		
		if (plate)
		{
			PlateObj = plate;
			PlateObj->AttachToComponent(MeshComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("WashingPoint"));
			isInSink = true;
			player->DetachGrabObj();
			PlateObj->BoxComp->SetSimulatePhysics(false);
		}
		else
		{
			if (PlateObj==nullptr)
				return;
			isInSink = false;
			PlateObj->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			player->AttachGrabObj(PlateObj);
		}
	}
}

void AKitchenSink::Washing(class ANotEvenPlayer* player)
{
	
}

void AKitchenSink::NetMulticast_Washing_Implementation(class ANotEvenPlayer* player)
{
	
}
