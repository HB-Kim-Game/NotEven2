// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodIngredient.h"

#include "IngredientActorIcon.h"
#include "NotEvenPlayer.h"
#include "CookingProgress.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

AFoodIngredient::AFoodIngredient()
{
	PrimaryActorTick.bCanEverTick = true;
	
	IconWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("IconWidgetComp"));
	IconWidgetComp->SetupAttachment(BoxComp);

	ProgressWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ProgressWidgetComp"));
	ProgressWidgetComp->SetupAttachment(BoxComp);

	BoxComp->SetCollisionProfileName(FName("Food"));
	ConstructorHelpers::FClassFinder<UIngredientActorIcon> iconClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KHB/UI/WBP_IngredientActorIcon.WBP_IngredientActorIcon_C'"));

	if (iconClass.Succeeded())
	{
		IconClass = iconClass.Class;
	}
	
	IconWidgetComp->SetWidgetClass(IconClass);
	
	ConstructorHelpers::FClassFinder<UCookingProgress> progressClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KHB/UI/WBP_CookingProgress.WBP_CookingProgress_C'"));

	if (progressClass.Succeeded())
	{
		ProgressClass = progressClass.Class;
	}

	ProgressWidgetComp->SetWidgetClass(ProgressClass);
	
	bReplicates = true;
}

void AFoodIngredient::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->SetSimulatePhysics(false);

	IconWidget = Cast<UIngredientActorIcon>(IconWidgetComp->GetWidget());
	IconWidgetComp->SetDrawSize(FVector2D(45.f, 45.f));
	IconWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	ProgressWidget = Cast<UCookingProgress>(ProgressWidgetComp->GetWidget());
	
	ProgressWidgetComp->SetDrawSize(FVector2D(75.f, 30.f));
	ProgressWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	
	PlayerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
}

void AFoodIngredient::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentState != EIngredientState::None)
	{
		IconWidgetComp->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 250.f));
		IconWidgetComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(IconWidgetComp->GetComponentLocation(), PlayerCameraManager->GetCameraLocation()));
	}
	
	if (ProgressWidgetComp->GetVisibleFlag())
	{
		ProgressWidgetComp->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, CurrentState != EIngredientState::None ? GetActorLocation().Z + 350.f : GetActorLocation().Z + 250.f));
		ProgressWidgetComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(ProgressWidgetComp->GetComponentLocation(), PlayerCameraManager->GetCameraLocation()));	
	}
}

void AFoodIngredient::InitializeIngredient_Implementation(FIngredientData data, FIngredientPlaceData place)
{
	Data = data;
	PlaceData = place;
	
	NetMulticast_InitializeIngredient(Data, PlaceData);
}

void AFoodIngredient::NetMulticast_InitializeIngredient_Implementation(FIngredientData data, FIngredientPlaceData place)
{
	Data = data;
	PlaceData = place;
	
	if (Data.MeshPaths.Num() <= 0) return;
	
	for (auto path : Data.MeshPaths)
	{
		if (UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *path.StaticMeshPath))
		{
			IngredientMeshes.Add(path.State, mesh);	
		}
	}

	SetMaxCookingProgress(Data.MaxCookingProgress);

	CurrentState = EIngredientState::None;
	IconWidgetComp->SetVisibility(CurrentState != EIngredientState::None);
	
	if (auto mesh = IngredientMeshes.Find(CurrentState))
	{
		if (*mesh)
		{
			MeshComp->SetStaticMesh(*mesh);
			BoxComp->SetBoxExtent(MeshComp->GetStaticMesh()->GetBounds().BoxExtent);
			if ((*mesh)->GetBounds().BoxExtent.Z > MeshComp->Bounds.BoxExtent.X) MeshComp->SetRelativeRotation(FRotator(90.f, 0, 0));
			else MeshComp->SetRelativeRotation(FRotator(0, 0, 0));
		}
	}
	
	if (IconWidget)
	{
		if (UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *Data.IconAssetPath))
		{
			IconWidget->SetIconImage(texture);
		}
	}
	
	CurrentCookingProgress = 0.0f;
	ProgressWidgetComp->SetVisibility(false);
	
	BoxComp->SetMassOverrideInKg(NAME_None, 150.f);
	BoxComp->SetLinearDamping(2.f);
}

FIngredientData AFoodIngredient::GetIngredientData() const
{
	return Data;
}

EIngredientState AFoodIngredient::GetIngredientState() const
{
	return CurrentState;
}

FIngredientPlaceData AFoodIngredient::GetIngredientPlaceData() const
{
	return PlaceData;
}

void AFoodIngredient::AddCookingProgress(float addProgress)
{
	CurrentCookingProgress += addProgress;
	if (HasAuthority())
	{
		Rep_CurrentCookingProgress();	
	}
}

float AFoodIngredient::GetCurrentCookingProgress() const
{
	return CurrentCookingProgress;
}

float AFoodIngredient::GetMaxCookingProgress() const
{
	return MaxCookingProgress;
}

void AFoodIngredient::SetState(EIngredientState newState)
{
	CurrentState = newState;

	NetMulticast_SetCurrentState(CurrentState);
}

void AFoodIngredient::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
	NetRPC_Interact(player);
}

void AFoodIngredient::SetGrab(bool bGrab)
{
	Super::SetGrab(bGrab);

	// 잡았을 경우
	if (bGrab)
	{
		BoxComp->SetSimulatePhysics(false);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	// 놓았을 경우
	else
	{
		BoxComp->SetSimulatePhysics(true);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	}
}

void AFoodIngredient::SetMaxCookingProgress(float progress)
{
	MaxCookingProgress = progress;
}

void AFoodIngredient::Rep_CurrentCookingProgress()
{
	if (CurrentCookingProgress <= 0.01f)
	{
		ProgressWidgetComp->SetVisibility(false);
		return;
	}

	ProgressWidgetComp->SetVisibility(true);
	ProgressWidget->Progress = CurrentCookingProgress / MaxCookingProgress;
	
}

void AFoodIngredient::NetMulticast_SetCurrentState_Implementation(EIngredientState next)
{
	CurrentState = next;
	IconWidgetComp->SetVisibility(CurrentState != EIngredientState::None);
	
	if (auto mesh = IngredientMeshes.Find(CurrentState))
	{
		if (*mesh)
		{
			MeshComp->SetStaticMesh(*mesh);
			BoxComp->SetBoxExtent(MeshComp->GetStaticMesh()->GetBounds().BoxExtent);
			if ((*mesh)->GetBounds().BoxExtent.Z > MeshComp->Bounds.BoxExtent.X) MeshComp->SetRelativeRotation(FRotator(90.f, 0, 0));
			else MeshComp->SetRelativeRotation(FRotator(0, 0, 0));
		}
	}
	
	if (IconWidget)
	{
		if (UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *Data.IconAssetPath))
		{
			IconWidget->SetIconImage(texture);
		}
	}
	
	CurrentCookingProgress = 0.0f;
	ProgressWidgetComp->SetVisibility(false);
}

void AFoodIngredient::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFoodIngredient, CurrentCookingProgress);
}

void AFoodIngredient::NetRPC_Interact_Implementation(class ANotEvenPlayer* player)
{
	switch (CurrentState)
	{
	case EIngredientState::None:
		SetGrab(true);
		AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabPoint"));
		break;
	case EIngredientState::Sliced:
		SetGrab(true);
		AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabPoint"));
		break;
	default:
		break;
	}
}
