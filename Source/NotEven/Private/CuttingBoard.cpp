// Fill out your copyright notice in the Description page of Project Settings.


#include "CuttingBoard.h"

#include "NiagaraFunctionLibrary.h"
#include "NotEvenPlayer.h"
#include "Plate.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ACuttingBoard::ACuttingBoard()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempmesh
	(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/KitchenCounter_cuttingboard.KitchenCounter_cuttingboard'"));

	if (tempmesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempmesh.Object);
	}

	attachBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttachBox"));
	attachBox-> SetupAttachment(BoxComp);
	attachBox->SetBoxExtent(FVector(75, 75, 32));
	attachBox ->SetRelativeLocation(FVector(0, 0, 75));
	attachBox ->SetRelativeScale3D(FVector(1,1,0.25));

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> tempDashEffect(TEXT("/Script/Niagara.NiagaraSystem'/Game/KHB/Models/NS_Dust.NS_Dust'"));
	if (tempDashEffect.Succeeded())
	{
		CuttingEffect = tempDashEffect.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundWave>tempCuttingSound(TEXT("/Script/Engine.SoundWave'/Game/LGJ/Sound/Chopping.Chopping'"));
	if (tempCuttingSound.Succeeded())
	{
		CuttingSound = tempCuttingSound.Object;
	}

	bReplicates = true;
	bIsInteractable = true;
}

void ACuttingBoard::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);

	AFoodIngredient* temp = nullptr;

	// 만약에 플레이어가 isGrab 상태이면
	if (player -> isGrab)
	{
		if (moveObject) return;
		// moveObject을 Grad 하고 있으면
		auto food = Cast<AFoodIngredient>(player->OwnedObj);
		if (food)
		{
			temp = food;
			player->DetachGrabObj(false);
		}
	}
	
	NetMulticast_Interact(player, temp);
}

void ACuttingBoard::NetMulticast_Interact_Implementation(class ANotEvenPlayer* player, class AFoodIngredient* food)
{
	// 만약에 플레이어가 isGrab 상태이면
	if (food)
	{
		moveObject = food;
		moveObject->AttachToComponent(attachBox,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		isOnCuttingBoard = true;
	}
	else
	{
		if (moveObject == nullptr) return;
		
		isOnCuttingBoard = false;
		moveObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		player->AttachGrabObj(moveObject);
		moveObject = nullptr;
	}
}

void ACuttingBoard::NetMulticast_Cutting_Implementation(class ANotEvenPlayer* player)
{
	// 만약 도마 위에 음식이 존재하면
	if (isOnCuttingBoard == true)
	{
		if (moveObject-> GetIngredientState()!=EIngredientState::None)
			return;

		UAnimInstance* animInst = player->GetMesh()->GetAnimInstance();
		if (animInst && InteractMontage && !animInst->IsAnyMontagePlaying())
		{
			animInst->Montage_Play(InteractMontage);
		}
		
		if (CuttingSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), CuttingSound);
		}
		
		auto effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CuttingEffect, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 60.f));
	}
}

void ACuttingBoard::Cutting(class ANotEvenPlayer* player)
{
	if (isOnCuttingBoard == true)
	{
		if (moveObject-> GetIngredientState()!=EIngredientState::None)
			return;
		// 실행 될 때마다 currentCount를 올리고 싶다
		moveObject->AddCookingProgress(10);
		// 카운트가 maxCount랑 같아지면
		if (moveObject->GetCurrentCookingProgress() >= moveObject->GetMaxCookingProgress())
		{
			// 음식을 손질을 완료하고 싶다
			// -> 기존에 있던 음식(None)의 상태를
			// -> 음식(Slice) 상태를 변환하고 싶다
			moveObject->SetState(EIngredientState::Sliced);
		}
	}
	NetMulticast_Cutting(player);
}
