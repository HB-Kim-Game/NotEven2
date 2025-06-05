// Fill out your copyright notice in the Description page of Project Settings.


#include "KitchenSink.h"

AKitchenSink::AKitchenSink()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/kitchentable_sink.kitchentable_sink'"));
	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}
}

void AKitchenSink::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
}
