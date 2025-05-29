// Fill out your copyright notice in the Description page of Project Settings.


#include "Plate.h"

#include "DDSFile.h"
#include "NotEvenPlayer.h"

APlate::APlate()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/plate.plate'"));
	if (tempMesh.Succeeded())
	{
		MeshComp -> SetStaticMesh(tempMesh.Object);
	}
	MeshComp->SetRelativeLocation(FVector(0, 0, -12.5));
}

void APlate::BeginPlay()
{
	Super::BeginPlay();
}

void APlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void APlate::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);

	if (player && player->isGrab && player-> OwnedObj)
	{
		if (AFoodIngredient* food = Cast<AFoodIngredient>(player->OwnedObj))
		{
			player->DetachGrabObj();
			OnPlate(food);
		}
	}
	else
	{
		SetGrab(true);
		// 플레이어의 SkeletalMesh 내에 있는 Socket에 붙이기
		AttachToComponent(player->GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,TEXT("GrabPoint"));
	}

}

void APlate::SetGrab(bool bGrab)
{
	Super::SetGrab(bGrab);

	// 잡았을 경우
	if (bGrab)
	{
		// 물리 끄기
		BoxComp->SetSimulatePhysics(false);
		// NoCollision으로 설정
		BoxComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision); 
	}
	// 놓았을 경우
	else
	{
		//물리 적용
		BoxComp->SetSimulatePhysics(true);
		BoxComp->SetMassScale(NAME_None,1.f);
		//떨어질 때 회전값 잠금
		BoxComp->BodyInstance.bLockRotation = true;

		//떨어질 때 X,Y축의 이동을 잠그고 Z축으로만 이동하게 설정
		BoxComp->BodyInstance.bLockXTranslation = true; // X축
		BoxComp->BodyInstance.bLockYTranslation = true; // Y축
		
		BoxComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics); // Collision Enabled(QueryAndPhysics)으로 설정
	}
}

void APlate::OnPlate(AFoodIngredient* foodObj)
{
	if (!foodObj)
	{
		foodObj->AttachToComponent(MeshComp,FAttachmentTransformRules::SnapToTargetIncludingScale,TEXT("AttachPoint"));

		foodObj->SetGrab(false);
	}
	
}
