// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodIngredient.h"

#include "IngredientActorIcon.h"
#include "NotEvenPlayer.h"
#include "CookingProgress.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
}

void AFoodIngredient::BeginPlay()
{
	Super::BeginPlay();

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
	
	if (ProgressWidget->GetVisibility() != ESlateVisibility::Hidden)
	{
		ProgressWidgetComp->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, CurrentState != EIngredientState::None ? GetActorLocation().Z + 350.f : GetActorLocation().Z + 250.f));
		ProgressWidgetComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(ProgressWidgetComp->GetComponentLocation(), PlayerCameraManager->GetCameraLocation()));	
	}
}

void AFoodIngredient::InitializeIngredient(FIngredientData data, FIngredientPlaceData place)
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

	SetState(EIngredientState::None);

	if (IconWidget)
	{
		IconWidget->SetIconImage(nullptr);
	}

	BoxComp->SetMassScale(NAME_None, 15.f);
	BoxComp -> SetLinearDamping(2.f);
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

	ProgressWidget->SetVisibility(ESlateVisibility::Visible);
	ProgressWidget->Progress = CurrentCookingProgress / MaxCookingProgress;
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

	if (CurrentState != EIngredientState::None)
	{
		if (IconWidget)
		{
			if (UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *Data.IconAssetPath))
			{
				IconWidget->SetIconImage(texture);
			}
		}
	}

	if (auto mesh = IngredientMeshes.Find(newState))
	{
		if (*mesh)
		{
			MeshComp->SetStaticMesh(*mesh);
			BoxComp->SetBoxExtent(MeshComp->GetStaticMesh()->GetBounds().BoxExtent);
		}
	}
	
	CurrentCookingProgress = 0.0f;
	ProgressWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AFoodIngredient::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);

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

