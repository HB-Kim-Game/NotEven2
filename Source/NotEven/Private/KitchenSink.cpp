// Fill out your copyright notice in the Description page of Project Settings.


#include "KitchenSink.h"

#include "CookingProgress.h"
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

	bIsInteractable=true;
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
			if (plate->Platestate==EPlatestate::Dirty)
			{
				PlateObj->AttachToComponent(MeshComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("WashingPoint"));
				isInSink = true;
				player->DetachGrabObj(false);
			}

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
	else
	{
		if (PlateObj)
		{
			isInSink = false;
			PlateObj->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			PlateObj->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GrabPoint"));
			player->AttachGrabObj(PlateObj);
			
		}
	}
}

void AKitchenSink::Washing(class ANotEvenPlayer* player)
{
	// 실행 될 때마다 currentCount를 올리고 싶다
	NetMulticast_Washing(player, PlateObj->AddWashingProgress(10));
}
	

void AKitchenSink::NetMulticast_Washing_Implementation(class ANotEvenPlayer* player,float currentWashingProgress)
{
	UE_LOG(LogTemp, Display, TEXT("Washing"));
	// 만약 싱크대 안에 접시가 존재하면
	if (isInSink == true)
	{
		UE_LOG(LogTemp, Display, TEXT("isInSink == true"));
		if (PlateObj->CurrentState != EPlatestate::Dirty)
			return;
		
		UE_LOG(LogTemp, Display, TEXT("WashingProgress added"));
		// 카운트가 maxCount랑 같아지면
		if (currentWashingProgress >= PlateObj->GetMaxWashingProgress())
		{
			// 설거지를 완료하고 싶다
			// -> 기존에 있던 접시(Dirty)의 상태를
			// -> 접시(Clean) 상태를 변환하고 싶다
			PlateObj->SetState(EPlatestate::Clean);
		}
		
		if (PlateObj->CurrentState == EPlatestate::Clean)
		{
			PlateObj->AttachToComponent(MeshComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("CompletePoint"));
		}
	}
}
