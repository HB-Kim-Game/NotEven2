// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Pot.h"

#include "FoodIngredient.h"
#include "NotEvenPlayer.h"
#include "SubmitFood.h"
#include "Net/UnrealNetwork.h"

APot::APot()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/pot_A.pot_A'"));

	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}

	MeshComp->SetRelativeLocation(FVector(0, 0, -10.f));

	bReplicates = true;
}

void APot::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->SetSimulatePhysics(false);
}

void APot::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);

	NetMulticast_Interact(player);
}

void APot::NetMulticast_Interact_Implementation(class ANotEvenPlayer* player)
{
	// Plate 처럼
	// 플레이어가 isGrab 이고, 플레이어한테 OwnedObj가 있으면
	if (player && player -> isGrab && player -> OwnedObj)
	{
		if (AFoodIngredient* food = Cast<AFoodIngredient>(player->OwnedObj))
		{
			if (bISBurned) return;
			
			if (!food->GetIngredientPlaceData().PlacementRules.ContainsByPredicate([food](FIngredientPlaceRule& rule)
			{
				return rule.State == food->GetIngredientState() && rule.Requirement.Contains(EPlacementRequirement::Pot);
			}))
			{
				return;
			}
			// food 를 OnPot
			OnPot(food);
			// 플레이어가 들고 있는 오브젝트를 Detach
			player->DetachGrabObj(false);
			auto destroyObj = food;
			destroyObj->Destroy();
			
		}
	}
	else
	{
		SetGrab(true);
		// 플레이어의 SkeletalMesh 내에 있는 Socket에 붙이기
		AttachToComponent(player->GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,TEXT("GrabPoint"));
	}
}

void APot::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APot, SubmitFood);
	DOREPLIFETIME(APot, bISBurned);
	DOREPLIFETIME(APot, bIsBoiled);
}

void APot::AddProgress(float progress)
{
	if (SubmitFood->GetCurrentCookingProgress() >= SubmitFood->GetMaxCookingProgress() && !bIsBoiled)
	{
		bIsBoiled = true;
		UE_LOG(LogTemp,Warning,TEXT("Boiled"));
		OnRep_Boiled();
		return;
	}
	
	if (SubmitFood->GetCurrentCookingProgress() / SubmitFood->GetMaxCookingProgress() >= 1.5f)
	{
		bISBurned = true;
		UE_LOG(LogTemp,Warning,TEXT("Burned"));
		OnRep_Burned();
		return;
	}
	
	SubmitFood->AddProgress(progress);
}

bool APot::HasSubmitFood()
{
	return SubmitFood != nullptr;
}

void APot::SetGrab(bool bGrab)
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

void APot::OnPot(class AFoodIngredient* food)
{
	if (food)
	{
		if (SubmitFood == nullptr)
		{
			ASubmitFood* sbfood = GetWorld()->SpawnActor<ASubmitFood>();
			sbfood->AddIngredient(food->GetIngredientData(),food->GetIngredientState(),food->GetCurrentCookingProgress(), food->GetIngredientPlaceData());
			sbfood->BoxComp->SetSimulatePhysics(false);
			sbfood->AttachToComponent(MeshComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachPoint"));
			SubmitFood = sbfood;
			return;
		}
		SubmitFood -> AddIngredient(food->GetIngredientData(),food->GetIngredientState(),food->GetCurrentCookingProgress(), food->GetIngredientPlaceData());
		bIsBoiled = false;
	}
}

void APot::OnRep_Burned()
{
	if (bISBurned) SubmitFood->SetState(EIngredientState::Burned);
}

void APot::OnRep_Boiled()
{
	if (bIsBoiled) SubmitFood->SetState(EIngredientState::Boiled);
}
