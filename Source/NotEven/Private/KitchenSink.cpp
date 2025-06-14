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
	bReplicates=true;
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

		if (!plate)
			return;
		
		if (plate)
		{
			PlateObj = plate;
			if (plate->Platestate==EPlatestate::Dirty)
			{
				player->DetachGrabObj(false);
				PlateObj->AttachToComponent(MeshComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("WashingPoint"));
				UE_LOG(LogTemp,Warning,TEXT("isInSink"));
				isInSink = true;
			}
		}
	}
	else
	{
		if (PlateObj)
		{
			if (PlateObj->Platestate==EPlatestate::Dirty) return;
			
			isInSink = false;
			PlateObj->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			PlateObj->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GrabPoint"));
			player->AttachGrabObj(PlateObj);
			PlateObj = nullptr;
		}
	}
}


void AKitchenSink::NetMulticast_Washing_Implementation(class ANotEvenPlayer* player)
{
	UE_LOG(LogTemp, Display, TEXT("Washing"));
	// 만약 싱크대 안에 접시가 존재하면
	if (isInSink == true)
	{
		UE_LOG(LogTemp, Display, TEXT("isInSink == true"));
		if (PlateObj->Platestate != EPlatestate::Dirty)
			return;
		
		UAnimInstance* animInst = player->GetMesh()->GetAnimInstance();
		if (animInst && InteractMontage && !animInst->IsAnyMontagePlaying())
		{
			animInst->Montage_Play(InteractMontage);
		}
	}
}

void AKitchenSink::Washing(class ANotEvenPlayer* player)
{
	if (!PlateObj)
	{
		UE_LOG(LogTemp,Warning,TEXT("NotPlate"));
		return;
	}
	
	if (isInSink == true)
	{
		if (PlateObj->Platestate != EPlatestate::Dirty)
			return;

		// 실행 될 때마다 currentCount를 올리고 싶다
		PlateObj->AddWashingProgress(10);
		
		// 카운트가 maxCount랑 같아지면
		if (PlateObj->GetCurrentWashingProgress() >= PlateObj->GetMaxWashingProgress())
		{
			// 설거지를 완료하고 싶다
			// -> 접시(Clean) 상태로 변환하고 싶다
			PlateObj->SetState(EPlatestate::Clean);
		}
		if (PlateObj->Platestate == EPlatestate::Clean)
		{
			PlateObj->AttachToComponent(MeshComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("CompletePoint"));
		}
	}
	NetMulticast_Washing(player);

}
