// Fill out your copyright notice in the Description page of Project Settings.


#include "Plate.h"

#include "NotEvenPlayer.h"
#include "SubmitFood.h"
#include "Components/WidgetComponent.h"
#include "CookingProgress.h"
#include "Net/UnrealNetwork.h"

APlate::APlate() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/plate.plate'"));
	if (tempMesh.Succeeded())
	{
		MeshComp -> SetStaticMesh(tempMesh.Object);
	}
	MeshComp->SetRelativeLocation(FVector(0, 0, -7));

	BoxComp->SetBoxExtent(MeshComp->GetStaticMesh()->GetBounds().BoxExtent);
	
	attachPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("attachPoint"));
	attachPoint->SetupAttachment(BoxComp);
	attachPoint->SetBoxExtent(FVector(35, 35, 10));
	attachPoint->SetRelativeLocation(FVector(0, 0, 20));



	WashWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WashWidgetComp"));
	
	ConstructorHelpers::FClassFinder<UCookingProgress> progressClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KHB/UI/WBP_CookingProgress.WBP_CookingProgress_C'"));

	if (progressClass.Succeeded())
	{
		WashWidgetComp->SetWidgetClass(progressClass.Class);
	}
	
	bReplicates = true;
}

void APlate::BeginPlay()
{
	Super::BeginPlay();

	SetState(EPlatestate::Clean);
	BoxComp->SetSimulatePhysics(false);
}

void APlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void APlate::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
	
	NetMulticast_Interact(player);
}

void APlate::NetMulticast_Interact_Implementation(class ANotEvenPlayer* player)
{
	// 플레이어가 isGrab 이고, 플레이어한테 OwnedObj가 있으면
	if (player && player -> isGrab && player -> OwnedObj)
	{
		if (EPlatestate::Dirty == Platestate)
			return;
		if (AFoodIngredient* food = Cast<AFoodIngredient>(player->OwnedObj))
		{
			if (!food->GetIngredientPlaceData().PlacementRules.ContainsByPredicate([food](FIngredientPlaceRule& rule)
			{
				return rule.State == food->GetIngredientState() && rule.Requirement.Contains(EPlacementRequirement::Plate);
			}))
			{
				return;
			}
			// food 를 OnPlate
			OnPlate(food);
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
	if (foodObj)
	{
		if (submitFood == nullptr)
		{
			FVector attachLocation = attachPoint->GetComponentLocation();
			FRotator attachRotation = attachPoint->GetComponentRotation();
			
			ASubmitFood* sbfood = GetWorld()->SpawnActor<ASubmitFood>(attachLocation,attachRotation);
			sbfood->AddIngredient(foodObj->GetIngredientData(),foodObj->GetIngredientState(),foodObj->GetCurrentCookingProgress(), foodObj->GetIngredientPlaceData());
			sbfood->BoxComp->SetSimulatePhysics(false);
			sbfood->AttachToComponent(MeshComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("AttachPoint"));
			submitFood = sbfood;
			return;
		}
		submitFood -> AddIngredient(foodObj->GetIngredientData(),foodObj->GetIngredientState(),foodObj->GetCurrentCookingProgress(), foodObj->GetIngredientPlaceData());
	}
}

void APlate::SetState(EPlatestate nextState)
{
	Platestate=nextState;
	if(auto mesh =  StateMeshMap.Find(Platestate))
	{
		if (*mesh)
		{
			MeshComp->SetStaticMesh(*mesh);
		}
	}
}

void APlate::Rep_CurrentWashingProgress()
{
	if (CurrentWashingProgress <= 0.01f)
	{
		WashWidgetComp->SetVisibility(false);
		return;
	}

	WashWidgetComp->SetVisibility(true);
	WashingProgress->Progress = CurrentWashingProgress / MaxWashingProgress;
}

void APlate::AddWashingProgress(float addProgress)
{
	CurrentWashingProgress += addProgress;
	if (HasAuthority())
	{
		Rep_CurrentWashingProgress();
	}
}

float APlate::GetCurrentWashingProgress() const
{
	return CurrentWashingProgress;
}

float APlate::GetMaxWashingProgress() const
{
	return MaxWashingProgress;
}

void APlate::SetMaxWashingProgress(float progress)
{
	MaxWashingProgress = progress;
}

void APlate::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlate, CurrentWashingProgress);
	DOREPLIFETIME(APlate, CurrentState);
}
